#include "NGLM.hpp"
#include "NPY.hpp"
#include "G4StepNPY.hpp"
#include "TrivialCheckNPY.hpp"

#include "PLOG.hh"

TrivialCheckNPY::TrivialCheckNPY(NPY<float>* photons, NPY<float>* gensteps)
    :
    m_photons(photons),
    m_gensteps(gensteps),
    m_g4step(new G4StepNPY(m_gensteps))
{
}

void TrivialCheckNPY::check(const char* msg)
{
    dump(msg);

    checkGensteps(m_gensteps);

    unsigned nstep = m_g4step->getNumSteps();

    unsigned photon_offset(0);
    for(unsigned istep=0 ; istep < nstep ; istep++)
    { 
        unsigned numPhotonsForStep = m_g4step->getNumPhotons(istep);
        unsigned gencode = m_g4step->getGencode(istep);

        checkPhotons(istep, m_photons, photon_offset, photon_offset+numPhotonsForStep, gencode, numPhotonsForStep);

        photon_offset += numPhotonsForStep ;  
    }
}

void TrivialCheckNPY::checkGensteps(NPY<float>* gs)
{
    unsigned ni = gs->getShape(0) ;
    unsigned nj = gs->getShape(1) ;
    unsigned nk = gs->getShape(2) ;

    assert(ni > 0 && nj == 6 && nk == 4);

    m_g4step->dump("TrivialCheckNPY::checkGensteps");
}


void TrivialCheckNPY::checkPhotons(unsigned istep, NPY<float>* photons, unsigned i0, unsigned i1, unsigned gencode, unsigned numPhotons )
{
    assert(0==checkItemValue( istep, photons, i0, i1, 2, 0, "(ghead.u.x)gencode"         , IS_UCONSTANT,     gencode, 0 ));
    assert(0==checkItemValue( istep, photons, i0, i1, 2, 3, "(ghead.u.w)numPhotons"      , IS_UCONSTANT,  numPhotons, 0 ));

    unsigned PNUMQUAD = 4 ; 
    unsigned GNUMQUAD = 6 ; 

    assert(0==checkItemValue( istep, photons, i0, i1, 3, 0, "(indices.u.x)photon_id"     , IS_UINDEX,              -1,        0 ));
    assert(0==checkItemValue( istep, photons, i0, i1, 3, 1, "(indices.u.y)photon_offset" , IS_UINDEX_SCALED,       -1, PNUMQUAD ));
    assert(0==checkItemValue( istep, photons, i0, i1, 3, 2, "(indices.u.z)genstep_id"    , IS_UCONSTANT,        istep,        0 ));
    assert(0==checkItemValue( istep, photons, i0, i1, 3, 3, "(indices.u.w)genstep_offset", IS_UCONSTANT_SCALED, istep, GNUMQUAD ));

}

void TrivialCheckNPY::dump(const char* msg)
{
    LOG(info) << msg 
              << " photons " << m_photons->getShapeString()
              << " gensteps " << m_gensteps->getShapeString()
              ;

}


int TrivialCheckNPY::checkItemValue(unsigned istep, NPY<float>* npy, unsigned i0, unsigned i1, unsigned jj, unsigned kk, const char* label, int expect, int constant, int scale )
{
    unsigned uconstant(constant);

    unsigned ni = npy->getShape(0) ;
    unsigned nj = npy->getShape(1) ;
    unsigned nk = npy->getShape(2) ;

    assert(i0 < i1 && i0 < ni && i1 <= ni );
    assert(nj == 4 && nk == 4);  // photon buffer

    float* values = npy->getValues();

    uif_t uif ; 

    int fail(0);
    for(unsigned i=i0 ; i<i1 ; i++ )
    {
        unsigned index = i*nj*nk + jj*nk + kk ;
        uif.f = values[index];

        unsigned u = uif.u ;   

        //LOG(info) << " i " << std::setw(6) << i << " u " << std::setw(6) << u ;  

        if(     expect == IS_UINDEX)    
        {
            if(u != i )
            {
                LOG(warning) << "FAIL checkItemValue IS_UINDEX " << u << " " << i ; 
                fail += 1 ;   
            }
        }
        else if(expect == IS_UINDEX_SCALED)
        {
            if(u != i*scale )
            {
                LOG(warning) << "FAIL checkItemValue IS_UINDEX_SCALED " << u << " " << i*scale ; 
                fail += 1 ;   
            }
        }
        else if(expect == IS_UCONSTANT)  
        {
            if(u != uconstant )
            {
                LOG(warning) << "FAIL checkItemValue IS_UCONSTANT " << u << " " << i << " " << uconstant ; 
                fail += 1 ;   
            }
        }
        else if(expect == IS_UCONSTANT_SCALED)  
        {
            if(u != uconstant*scale )
            {
                LOG(warning) << "FAIL checkItemValue IS_UCONSTANT_SCALED " << u << " " << i << " " << uconstant*scale ; 
                fail += 1 ;   
            }
        }


    }
    LOG(info) 
            << " step " << istep
            << "[:," << jj << "," << kk << "] " 
            << std::setw(30) << label 
            << ( fail == 0 ? " OK " : " FAIL " ) << fail  ; 

    return fail ; 
}


