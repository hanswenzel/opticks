#include "SSys.hh"
#include "PLOG.hh"

#include "BFile.hh"
#include "BStr.hh"

#include "NPart.h"
#include "NPY.hpp"
#include "NParameters.hpp"
#include "NGLMExt.hpp"
#include "NCSGData.hpp"
#include "GLMFormat.hpp"


#define TREE_NODES(height) ( (0x1 << (1+(height))) - 1 )

// must match opticks/analytic/csg.py 
const char* NCSGData::FILENAME  = "csg.txt" ; 
const char* NCSGData::TREE_META = "meta.json" ;
const char* NCSGData::NODE_META = "nodemeta.json" ;
const char* NCSGData::PLANES    = "planes.npy" ; 
const char* NCSGData::SRC_FACES = "srcfaces.npy" ; 
const char* NCSGData::SRC_VERTS = "srcverts.npy" ; 
const char* NCSGData::IDX       = "idx.npy" ; 

const unsigned NCSGData::NTRAN = 3 ; 


bool NCSGData::Exists(const char* treedir)
{
    return ExistsDir(treedir);
}

bool NCSGData::ExistsDir(const char* treedir)
{
    if(!treedir) return false ; 
    if(!BFile::ExistsDir(treedir)) return false ; 
    return true ; 
}

std::string NCSGData::TxtPath(const char* treedir)
{
    std::string txtpath = BFile::FormPath(treedir, FILENAME) ;
    return txtpath ; 
}

bool NCSGData::ExistsTxt(const char* treedir)
{
    if(!ExistsDir(treedir)) return false ;  
    std::string txtpath = TxtPath(treedir) ; 
    bool exists = BFile::ExistsFile(txtpath.c_str() ); 
    return exists ; 
}

std::string NCSGData::MetaPath(const char* treedir, int idx)
{
    std::string metapath = idx == -1 ? BFile::FormPath(treedir, TREE_META) : BFile::FormPath(treedir, BStr::itoa(idx), NODE_META) ;
    return metapath ; 
}

bool NCSGData::ExistsMeta(const char* treedir, int idx)
{
    std::string metapath = MetaPath(treedir, idx) ;
    return BFile::ExistsFile(metapath.c_str()) ;
}



NCSGData::NCSGData()  
   :
   m_verbosity(1),
   m_meta(new NParameters),
   m_nodes(NULL),
   m_transforms(NULL),
   m_gtransforms(NULL),
   m_planes(NULL),
   m_srcverts(NULL),
   m_srcfaces(NULL),
   m_idx(NULL),
   m_height(0),
   m_num_nodes(0),
   m_num_transforms(0),
   m_num_planes(0),
   m_num_srcverts(0),
   m_num_srcfaces(0)
{
}




unsigned NCSGData::NumNodes(unsigned height)
{
   return TREE_NODES(height);
}
unsigned NCSGData::getHeight() const 
{
    return m_height ; 
}
unsigned NCSGData::getNumNodes() const 
{
    return m_num_nodes ; 
}
NPY<float>* NCSGData::getNodeBuffer() const 
{
    return m_nodes ; 
}
NPY<float>* NCSGData::getTransformBuffer() const 
{
    return m_transforms ; 
}

NPY<float>* NCSGData::getGTransformBuffer() const
{
    return m_gtransforms ; 
}
NPY<float>* NCSGData::getPlaneBuffer() const 
{
    return m_planes ; 
}
NPY<float>* NCSGData::getSrcVertsBuffer() const 
{
    return m_srcverts ; 
}
NPY<int>* NCSGData::getSrcFacesBuffer() const 
{
    return m_srcfaces ; 
}



NPY<unsigned>* NCSGData::getIdxBuffer() const 
{
    return m_idx ; 
}

NParameters* NCSGData::getMetaParameters(int idx) const
{
    return idx < 0 ? m_meta : getNodeMetadata(unsigned(idx)) ; 
}

NParameters* NCSGData::getNodeMetadata(unsigned idx) const 
{
    return m_nodemeta.count(idx) == 1 ? m_nodemeta.at(idx) : NULL ; 
}


void NCSGData::init_buffers(unsigned height)
{
   m_height = height ; 
 
   unsigned num_nodes = NumNodes(height); // number of nodes for a complete binary tree of the needed height, with no balancing 
   m_num_nodes = num_nodes ; 

   m_nodes = NPY<float>::make( m_num_nodes, NJ, NK);
   m_nodes->zero();

   m_transforms = NPY<float>::make(0,NTRAN,4,4) ;  
   m_transforms->zero();

   m_gtransforms = NPY<float>::make(0,NTRAN,4,4) ; 
   m_gtransforms->zero();

   m_planes = NPY<float>::make(0,4);
   m_planes->zero();

   m_idx = NPY<unsigned>::make(1,4);
   m_idx->zero();
}


void NCSGData::setIdx( unsigned index, unsigned soIdx, unsigned lvIdx, unsigned height )
{
    if(m_idx == NULL)
    {
        m_idx = NPY<unsigned>::make(1, 4);
        m_idx->zero() ;  
    } 
    assert( height == m_height ); 
    glm::uvec4 uidx(index, soIdx, lvIdx, height); 
    m_idx->setQuad(uidx, 0u );     
}






std::string NCSGData::smry() const 
{
    std::stringstream ss ; 
    ss 
       << " ht " << std::setw(2) << m_height 
       << " nn " << std::setw(4) << m_num_nodes
       << " nd " << ( m_nodes ? m_nodes->getShapeString() : "NULL" )
       << " tr " << ( m_transforms ? m_transforms->getShapeString() : "NULL" )
       << " gtr " << ( m_gtransforms ? m_gtransforms->getShapeString() : "NULL" )
       << " pln " << ( m_planes ? m_planes->getShapeString() : "NULL" )
       ;

    return ss.str();
}


std::string NCSGData::desc() const
{
    std::string node_sh = m_nodes ? m_nodes->getShapeString() : "-" ;    
    std::string tran_sh = m_transforms ? m_transforms->getShapeString() : "-" ;    
    std::stringstream ss ; 
    ss << "NCSGData " 
       << " node_sh " << node_sh  
       << " tran_sh " << tran_sh  
       << " meta " << m_meta->desc()
       ;
    return ss.str();  
}


void NCSGData::save(const char* treedir ) const 
{
    saveMetadata(treedir, -1 ); 
    saveNodes(treedir); 
    saveNodeMetadata(treedir);
    saveTransforms(treedir);
    savePlanes(treedir);   
    saveSrcVerts(treedir);   
    saveSrcFaces(treedir);   
}

void NCSGData::load(const char* treedir)
{
    loadMetadata(treedir);     // m_meta
    loadNodes(treedir);        // m_nodes, m_num_nodes, m_height 
    loadNodeMetadata(treedir);
    loadTransforms(treedir);   // m_transforms, m_num_transforms
    loadPlanes(treedir);       // m_planes, m_num_planes 
    loadSrcVerts(treedir);     // m_srcverts m_num_srcverts
    loadSrcFaces(treedir);     // m_srcfaces, m_num_srcfaces
}



void NCSGData::saveMetadata(const char* treedir, int idx) const 
{
    NParameters* meta = getMetaParameters(idx) ; 
    if(!meta) return ; 

    std::string metapath = MetaPath(treedir, idx) ;
    meta->save(metapath.c_str()); 
}

void NCSGData::loadMetadata(const char* treedir)
{
    m_meta = LoadMetadata( treedir, -1 );  // -1:treemeta
    if(!m_meta)
    { 
        LOG(fatal) << "NCSGData::loadMetadata"
                    << " treedir " << treedir 
                    ;
    } 
    assert(m_meta);
}



NParameters* NCSGData::LoadMetadata(const char* treedir, int idx )
{
    // TODO:
    //    per-node metadata, using metaNN.json ?
    //    where NN is the inorder tree index 
    //    which then gets associated to the appropriate node 
    //
    //    Need for access to Trd srcmeta

    NParameters* meta = NULL  ; 
    std::string metapath = MetaPath(treedir, idx) ;

    meta = new NParameters ; 

    if(BFile::ExistsFile(metapath.c_str()))
    {
         meta->load_( metapath.c_str() );
    } 
    else
    {
        // lots missing as are looping over complete tree node count
        // see notes/issues/axel_GSceneTest_fail.rst
        LOG(trace) << "NCSGData::LoadMetadata"
                     << " missing metadata "
                     << " treedir " << treedir  
                     << " idx " << idx
                     << " metapath " << metapath
                     ;
        //std::cerr << metapath << std::endl ; 
    }
    return meta ; 
}




void NCSGData::saveNodeMetadata(const char* treedir) const 
{
    for(unsigned idx=0 ; idx < m_num_nodes ; idx++)
    {
        saveMetadata(treedir, idx); 
    } 
}

void NCSGData::loadNodeMetadata(const char* treedir)
{
    // FIX: this idx is not same as real complete binary tree node_idx ?


    // just looping over the number of nodes in the complete tree
    // so it is not surprising that many of them are missing metadata

    std::vector<unsigned> missmeta ;
    assert(m_num_nodes > 0);
    for(unsigned idx=0 ; idx < m_num_nodes ; idx++)
    {
        NParameters* nodemeta = LoadMetadata(treedir, idx);
        if(nodemeta)
        { 
            m_nodemeta[idx] = nodemeta ; 
        }
        else
        {
            missmeta.push_back(idx+1);  // make 1-based 
        }
    } 

    LOG(debug) << "NCSGData::loadNodeMetadata" 
              << " treedir " << treedir
              << " m_height " << m_height
              << " m_num_nodes " << m_num_nodes
              << " missmeta " << missmeta.size()
               ; 

/*
    if(missmeta.size() > 0)
    {
        std::cerr << " missmeta 1-based (" << missmeta.size() << ") : " ; 
        for(unsigned i=0 ; i < missmeta.size() ; i++) std::cerr << " " << missmeta[i]  ; 
        std::cerr << std::endl  ; 
    }
*/

}



void NCSGData::saveNodes(const char* treedir) const
{
    std::string nodepath = BFile::FormPath(treedir, "nodes.npy") ;
    m_nodes->save(nodepath.c_str());
} 

void NCSGData::loadNodes(const char* treedir)
{
    std::string nodepath = BFile::FormPath(treedir, "nodes.npy") ;
    m_nodes = NPY<float>::load(nodepath.c_str());

    if(!m_nodes)
    {
         LOG(fatal) << "NCSGData::loadNodes"
                    << " failed to load " 
                    << " nodepath [" << nodepath << "]"
                    ;
    }

    assert(m_nodes);

    m_num_nodes  = m_nodes->getShape(0) ;  
    unsigned nj = m_nodes->getShape(1);
    unsigned nk = m_nodes->getShape(2);

    assert( nj == NJ );
    assert( nk == NK );

    m_height = UINT_MAX ; 
    int h = MAX_HEIGHT*2 ;   // <-- dont let exceeding MAXHEIGHT, mess up determination of height 
    while(h--)
    {
        unsigned complete_nodes = TREE_NODES(h) ;
        if(complete_nodes == m_num_nodes) m_height = h ; 
    }

    bool invalid_height = m_height == UINT_MAX ; 

    if(invalid_height)
    {
        LOG(fatal) << "NCSGData::loadNodes"
                   << " INVALID_HEIGHT "
                   << " m_nodes " << m_nodes->getShapeString()
                   << " num_nodes " << m_num_nodes
                   << " MAX_HEIGHT " << MAX_HEIGHT
                   ;

    }

    assert(!invalid_height); // must be complete binary tree sized 1, 3, 7, 15, 31, ...
}



void NCSGData::saveTransforms(const char* treedir) const
{
    std::string path = BFile::FormPath(treedir, "transforms.npy") ;
    m_transforms->save(path.c_str());
    // gtransforms not saved, they are constructed on load
} 

void NCSGData::loadTransforms(const char* treedir)
{
    std::string tranpath = BFile::FormPath(treedir, "transforms.npy") ;
    if(!BFile::ExistsFile(tranpath.c_str())) return ; 

    NPY<float>* src = NPY<float>::load(tranpath.c_str());
    assert(src); 
    bool valid_src = src->hasShape(-1,4,4) ;
    if(!valid_src) 
    {
        LOG(fatal) << "NCSGData::loadTransforms"
                   << " invalid src transforms "
                   << " tranpath " << tranpath
                   << " src_sh " << src->getShapeString()
                   ;
    }

    assert(valid_src);
    unsigned ni = src->getShape(0) ;

              
    assert(NTRAN == 2 || NTRAN == 3);

    NPY<float>* transforms = NTRAN == 2 ? NPY<float>::make_paired_transforms(src) : NPY<float>::make_triple_transforms(src) ;
    assert(transforms->hasShape(ni,NTRAN,4,4));

    m_transforms = transforms ; 
    m_gtransforms = NPY<float>::make(0,NTRAN,4,4) ;  // for collecting unique gtransforms

    m_num_transforms  = ni  ;  

    LOG(trace) << "NCSGData::loadTransforms"
              << " tranpath " << tranpath 
              << " num_transforms " << ni
              ;

}


void NCSGData::saveSrcVerts(const char* treedir) const
{
    std::string path = BFile::FormPath(treedir, SRC_VERTS) ;
    if(!m_srcverts) return ; 
    m_srcverts->save(path.c_str());
} 

void NCSGData::loadSrcVerts(const char* treedir)
{
    std::string path = BFile::FormPath(treedir,  SRC_VERTS ) ;
    if(!BFile::ExistsFile(path.c_str())) return ; 

    NPY<float>* a = NPY<float>::load(path.c_str());
    assert(a); 
    bool valid = a->hasShape(-1,3) ;
    if(!valid) 
    {
        LOG(fatal) << "NCSGData::loadVerts"
                   << " invalid verts  "
                   << " path " << path
                   << " shape " << a->getShapeString()
                   ;
    }

    assert(valid);
    m_srcverts = a ; 
    m_num_srcverts = a->getShape(0) ; 

    LOG(info) << " loaded " << m_num_srcverts ;
}



void NCSGData::saveSrcFaces(const char* treedir) const
{
    std::string path = BFile::FormPath(treedir, SRC_FACES) ;
    if(!m_srcfaces) return ; 
    m_srcfaces->save(path.c_str());
} 

void NCSGData::loadSrcFaces(const char* treedir)
{
    std::string path = BFile::FormPath(treedir,  SRC_FACES ) ;
    if(!BFile::ExistsFile(path.c_str())) return ; 

    NPY<int>* a = NPY<int>::load(path.c_str());
    assert(a); 
    bool valid = a->hasShape(-1,4) ;
    if(!valid) 
    {
        LOG(fatal) << "NCSGData::loadFaces"
                   << " invalid faces  "
                   << " path " << path
                   << " shape " << a->getShapeString()
                   ;
    }
    assert(valid);
    m_srcfaces = a ; 
    m_num_srcfaces = a->getShape(0) ; 

    LOG(info) << " loaded " << m_num_srcfaces ;
}



void NCSGData::savePlanes(const char* treedir) const
{
    std::string path = BFile::FormPath(treedir, PLANES) ;
    if(!m_planes) return ; 
    m_planes->save(path.c_str());
} 

void NCSGData::loadPlanes(const char* treedir)
{
    std::string path = BFile::FormPath(treedir,  PLANES ) ;
    if(!BFile::ExistsFile(path.c_str())) return ; 

    NPY<float>* a = NPY<float>::load(path.c_str());
    assert(a); 
    bool valid = a->hasShape(-1,4) ;
    if(!valid) 
    {
        LOG(fatal) << "NCSGData::loadPlanes"
                   << " invalid planes  "
                   << " path " << path
                   << " shape " << a->getShapeString()
                   ;
    }
    assert(valid);

    m_planes = a ; 
    m_num_planes = a->getShape(0) ; 
}



void NCSGData::saveIdx(const char* treedir) const
{
    std::string path = BFile::FormPath(treedir, IDX) ;
    if(!m_idx) return ; 
    m_idx->save(path.c_str());
} 

void NCSGData::loadIdx(const char* treedir)
{
    std::string path = BFile::FormPath(treedir, IDX ) ;
    //if(!BFile::ExistsFile(path.c_str())) return ; 

    NPY<unsigned>* a = NPY<unsigned>::load(path.c_str());
    bool valid = a->hasShape(1,4) ;
    if(!valid) 
    {
        LOG(fatal) << "NCSGData::loadIdx"
                   << " invalid idx  "
                   << " path " << path
                   << " shape " << a->getShapeString()
                   ;
    }
    assert(valid);
    m_idx = a ;
}

// metadata from the root nodes of the CSG trees for each solid
// pmt-cd treebase.py:Node._get_meta
//
template<typename T>
T NCSGData::getMeta(const char* key, const char* fallback ) const 
{
    return m_meta ? m_meta->get<T>(key, fallback) : BStr::LexicalCast<T>(fallback) ;
}

template<typename T>
void NCSGData::setMeta(const char* key, T value )
{
    assert( m_meta ) ; 
    return m_meta->set<T>(key, value) ;
}







unsigned NCSGData::getTypeCode(unsigned idx)
{
    return m_nodes->getUInt(idx,TYPECODE_J,TYPECODE_K,0u);
}
unsigned NCSGData::getTransformIndex(unsigned idx)
{
    unsigned raw = m_nodes->getUInt(idx,TRANSFORM_J,TRANSFORM_K,0u);
    return raw & SSys::OTHERBIT32 ;   // <-- strip the sign bit  
}
bool NCSGData::isComplement(unsigned idx)
{
    unsigned raw = m_nodes->getUInt(idx,TRANSFORM_J,TRANSFORM_K,0u);
    return raw & SSys::SIGNBIT32 ;   // pick the sign bit 
}
nquad NCSGData::getQuad(unsigned idx, unsigned j)
{
    nquad qj ; 
    qj.f = m_nodes->getVQuad(idx, j) ;
    return qj ;
}



nmat4pair* NCSGData::import_transform_pair(unsigned itra)
{
    // itra is a 1-based index, with 0 meaning None

    assert(NTRAN == 2);
    if(itra == 0 || m_transforms == NULL ) return NULL ; 

    unsigned idx = itra - 1 ; 

    assert( idx < m_num_transforms );
    assert(m_transforms->hasShape(-1,NTRAN,4,4));

    nmat4pair* pair = m_transforms->getMat4PairPtr(idx);

    return pair ; 
}

nmat4triple* NCSGData::import_transform_triple(unsigned itra)
{
    // itra is a 1-based index, with 0 meaning None

    assert(NTRAN == 3);
    if(itra == 0 || m_transforms == NULL ) return NULL ; 

    unsigned idx = itra - 1 ; 

    assert( idx < m_num_transforms );
    assert(m_transforms->hasShape(-1,NTRAN,4,4));

    nmat4triple* triple = m_transforms->getMat4TriplePtr(idx);

    return triple ; 
}



template NPY_API void NCSGData::setMeta<float>(const char*, float);
template NPY_API void NCSGData::setMeta<int>(const char*, int);
template NPY_API void NCSGData::setMeta<bool>(const char*, bool);
template NPY_API void NCSGData::setMeta<std::string>(const char*, std::string);

template NPY_API std::string NCSGData::getMeta<std::string>(const char*, const char*) const ;
template NPY_API int         NCSGData::getMeta<int>(const char*, const char*) const ;
template NPY_API float       NCSGData::getMeta<float>(const char*, const char*) const ;
template NPY_API bool        NCSGData::getMeta<bool>(const char*, const char*) const ;







unsigned NCSGData::addUniqueTransform( const nmat4triple* gtransform )
{
    /*
    std::cout << "NCSGData::addUniqueTransform"
              << " gtransform " << *gtransform
              << std::endl 
              ;
    */

    NPY<float>* gtmp = NPY<float>::make(1,NTRAN,4,4);
    gtmp->zero();
    gtmp->setMat4Triple( gtransform, 0);

    unsigned gtransform_idx = 1 + m_gtransforms->addItemUnique( gtmp, 0 ) ; 
    delete gtmp ; 

    return gtransform_idx ; 
}




void NCSGData::dump_gtransforms() const 
{
    unsigned ni =  m_gtransforms ? m_gtransforms->getNumItems() : 0  ;

    for(unsigned i=0 ; i < ni ; i++)
    {
        const nmat4triple* u_gtran = m_gtransforms->getMat4TriplePtr(i);
        std::cout 
                  << "[" << std::setw(2) << i << "] " 
                  << *u_gtran 
                  << std::endl ; 
    }
}


void NCSGData::getPlanes(std::vector<glm::vec4>& _planes, unsigned idx, unsigned num_plane ) const 
{
    assert( idx < m_num_planes );
    assert( idx + num_plane - 1 < m_num_planes );

    assert( m_planes->hasShape(-1,4) );

    for(unsigned i=idx ; i < idx + num_plane ; i++)
    {
        glm::vec4 plane = m_planes->getQuad(i) ;

        _planes.push_back(plane);    

        if(m_verbosity > 3)
        std::cout << " plane " << std::setw(3) << i 
                  << gpresent(plane)
                  << std::endl ; 

    }
}



