#include "OpIndexer.hh"
#include "Opticks.hh"

#include <algorithm>
#include <cassert>

// npy-
#include "NLog.hpp"
#include "Timer.hpp"
#include "NumpyEvt.hpp"  
#include "NPY.hpp"  


// thrustrap-
#include "TBuf.hh"
#include "TSparse.hh"
#include "TUtil.hh"


// optixrap-
#include "OBuf.hh"
#include "OContext.hh"
#include "OPropagator.hh"

// cudawrap-
#include "CResource.hh"
#include "CBufSpec.hh"

// thrust 
#include <thrust/device_vector.h>


#define TIMER(s) \
    { \
       if(m_evt)\
       {\
          Timer& t = *(m_evt->getTimer()) ;\
          t((s)) ;\
       }\
    }



void OpIndexer::update()
{
    assert(m_evt) ;
    m_phosel = m_evt->getPhoselData(); 
    m_recsel = m_evt->getRecselData();
    m_maxrec = m_evt->getMaxRec(); 

    setNumPhotons(m_evt->getNumPhotons());

    OBuf* seq = m_propagator ? m_propagator->getSequenceBuf() : NULL ;
    setSeq(seq);

    OBuf* pho = m_propagator ? m_propagator->getPhotonBuf() : NULL ;
    setPho(pho);
}



void OpIndexer::setNumPhotons(unsigned int num_photons)
{
    assert(num_photons == m_evt->getSequenceData()->getShape(0));
    m_num_photons = num_photons ; 
}





void OpIndexer::indexBoundaries()
{
    update();

    bool hexkey = false ; 

    if(!m_pho)
    {
        LOG(warning) << "OpIndexer::indexBoundaries OBuf m_pho is NULL : SKIPPING " ; 
        return ;  
    }

    TSparse<int> boundaries(Opticks::BNDIDX_NAME_, m_pho->slice(4*4,4*3+0), hexkey); // stride,begin  hexkey effects Index and dumping only 

    m_evt->setBoundaryIdx(boundaries.getIndex());

    boundaries.make_lookup();

    if(m_verbose)
    boundaries.dump("OpIndexer::indexBoundaries TSparse<int>::dump");

    TIMER("indexBoundaries"); 
}




void OpIndexer::indexSequence()
{
    if(m_evt->isIndexed())
    {
        LOG(info) << "OpIndexer::indexSequence"
                  << " already indexed SKIP "
                  ;
        return ;
    }

    bool loaded = m_evt->isLoaded();
    bool compute = m_ocontext ? m_ocontext->isCompute() : false ;
   

    if(loaded)
    {
        indexSequenceLoaded();
    }
    else if(compute)
    {
        indexSequenceCompute();
    }
    else
    {
        indexSequenceInterop();
    }

}

void OpIndexer::indexSequenceCompute()
{
    update();

    assert(m_seq);

    LOG(info) << "OpIndexer::indexSequenceCompute" ; 
    CBufSlice seqh = m_seq->slice(2,0) ;  // stride, begin
    CBufSlice seqm = m_seq->slice(2,1) ;

    TSparse<unsigned long long> seqhis(Opticks::SEQHIS_NAME_, seqh );
    TSparse<unsigned long long> seqmat(Opticks::SEQMAT_NAME_, seqm ); 

    m_evt->setHistorySeq(seqhis.getIndex());
    m_evt->setMaterialSeq(seqmat.getIndex());  // the indices are populated by the make_lookup below

    indexSequenceViaThrust(seqhis, seqmat, m_verbose );

    TIMER("indexSequenceCompute"); 
}


void OpIndexer::indexSequenceInterop()
{
    // used by standard indexing from ggv- ie with OptiX OpenGL and Thrust all in play 
    //
    // OptiX OBuf provides access to the sequence buffer
    // and OpenGL mapping to CUDA gives access to the output recsel/phosel
    // as these are used by OpenGL

    update();
    if(m_evt->isIndexed()) return ;  

    assert(m_seq);

    LOG(info) << "OpIndexer::indexSequenceInterop" ; 
    CBufSlice seqh = m_seq->slice(2,0) ;  // stride, begin
    CBufSlice seqm = m_seq->slice(2,1) ;

    TSparse<unsigned long long> seqhis(Opticks::SEQHIS_NAME_, seqh );
    TSparse<unsigned long long> seqmat(Opticks::SEQMAT_NAME_, seqm ); 

    m_evt->setHistorySeq(seqhis.getIndex());
    m_evt->setMaterialSeq(seqmat.getIndex());  // the indices are populated by the make_lookup below

    indexSequenceViaOpenGL(seqhis, seqmat, m_verbose );

    TIMER("indexSequenceInterop"); 
}


void OpIndexer::indexSequenceLoaded()
{
    // starts from host based index

    LOG(info) << "OpIndexer::indexSequenceLoaded" ; 
    update();
    if(m_evt->isIndexed()) return ;  

    NPY<unsigned long long>* ph = m_evt->getSequenceData(); 
    thrust::device_vector<unsigned long long> dph(ph->begin(),ph->end());
    CBufSpec cph = make_bufspec<unsigned long long>(dph); 

    TBuf tph("tph", cph);
    tph.dump<unsigned long long>("OpIndexer::indexSequenceLoaded tph dump", 2, 0, 10 );  
    
    CBufSlice phh = tph.slice(2,0) ; // stride, begin  
    CBufSlice phm = tph.slice(2,1) ;

    TSparse<unsigned long long> seqhis(Opticks::SEQHIS_NAME_,  phh );
    TSparse<unsigned long long> seqmat(Opticks::SEQMAT_NAME_, phm ); 

    m_evt->setHistorySeq(seqhis.getIndex());
    m_evt->setMaterialSeq(seqmat.getIndex());  // the indices are populated by the make_lookup below

    assert(m_phosel != 0 && m_recsel != 0);

    indexSequenceViaThrust(seqhis, seqmat, m_verbose );

    TIMER("indexSequenceLoaded"); 
}






void OpIndexer::dumpHis(const TBuf& tphosel, const TSparse<unsigned long long>& seqhis)
{
    OBuf* seq = m_seq ; 
    if(seq)
    {
        unsigned int nsqa = seq->getNumAtoms(); 
        unsigned int nsqd = std::min(nsqa,100u); 
        seq->dump<unsigned long long>("OpIndexer::dumpHis seq(2,0)", 2, 0, nsqd);
    }

    unsigned int nphosel = tphosel.getSize() ; 
    unsigned int npsd = std::min(nphosel,100u) ;
    tphosel.dumpint<unsigned char>("tphosel.dumpint<unsigned char>(4,0)", 4,0, npsd) ;
    LOG(info) << seqhis.dump_("OpIndexer::dumpHis seqhis");
}

void OpIndexer::dumpMat(const TBuf& tphosel, const TSparse<unsigned long long>& seqmat)
{
    OBuf* seq = m_seq ; 
    if(seq) 
    {
        unsigned int nsqa = seq->getNumAtoms(); 
        unsigned int nsqd = std::min(nsqa,100u); 
        seq->dump<unsigned long long>("OpIndexer::dumpMat OBuf seq(2,1)", 2, 1, nsqd);
    }

    unsigned int nphosel = tphosel.getSize() ; 
    unsigned int npsd = std::min(nphosel,100u) ;
    tphosel.dumpint<unsigned char>("tphosel.dumpint<unsigned char>(4,1)", 4,1, npsd) ;
    LOG(info) << seqmat.dump_("OpIndexer::dumpMat seqmat");
}

void OpIndexer::dump(const TBuf& tphosel, const TBuf& trecsel)
{
    OBuf* seq = m_seq ; 

    unsigned int nphosel = tphosel.getSize() ; 
    unsigned int nrecsel = trecsel.getSize() ; 

    LOG(info) << "OpIndexer::dump"
              << " nphosel " << nphosel
              << " nrecsel " << nrecsel
              ; 

    if(seq)
    {
        unsigned int nsqa = seq->getNumAtoms(); 
        assert(nphosel == 2*nsqa);
        assert(nrecsel == m_maxrec*2*nsqa);
    } 
}


