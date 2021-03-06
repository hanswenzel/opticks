/*
 * Copyright (c) 2019 Opticks Team. All Rights Reserved.
 *
 * This file is part of Opticks
 * (see https://bitbucket.org/simoncblyth/opticks).
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */

#include "NPY.hpp"
#include "CPhotonCollector.hh"
#include "Opticks.hh"
#include "PLOG.hh"

CPhotonCollector* CPhotonCollector::INSTANCE = NULL ;

const plog::Severity CPhotonCollector::LEVEL = PLOG::EnvLevel("CPhotonCollector", "DEBUG") ; 


CPhotonCollector* CPhotonCollector::Instance()
{
   assert(INSTANCE && "CPhotonCollector has not been instanciated");
   return INSTANCE ;
}


CPhotonCollector::CPhotonCollector()    
    :
    m_photon(NPY<float>::make(0,4,4)),
    m_photon_itemsize(m_photon->getNumValues(1)),
    m_photon_values(new float[m_photon_itemsize]),
    m_photon_count(0)
{
    assert( m_photon_itemsize == 4*4 );
    INSTANCE = this ; 
}

/**
CPhotonCollector::reset
-------------------------

This *reset* clears the data setting the number of items back to zero.

**/

void CPhotonCollector::reset()
{
    m_photon->reset();  
    m_photon_count = 0 ; 
}


NPY<float>*  CPhotonCollector::getPhoton() const 
{
    return m_photon ; 
}

void CPhotonCollector::save(const char* path) const 
{
    m_photon->save(path) ; 
}
void CPhotonCollector::save(const char* dir, const char* name) const 
{
    m_photon->save(dir, name) ; 
}



std::string CPhotonCollector::desc() const
{
    std::stringstream ss ; 
    ss << " CPhotonCollector "
       << " photon_count " << m_photon_count
       << " photon_shape " << m_photon->getShapeString()
       << " content_version " << m_photon->getArrayContentVersion() 
       ;
    return ss.str();
}

void CPhotonCollector::Summary(const char* msg) const 
{ 
    LOG(info) << msg 
              << desc()
              ;
}


void CPhotonCollector::collectPhoton(

               double  pos_x,
               double  pos_y,
               double  pos_z,
               double  time,

               double  dir_x,
               double  dir_y,
               double  dir_z,
               double  weight,

               double  pol_x,
               double  pol_y,
               double  pol_z,
               double  wavelength,

               int flags_x,
               int flags_y,
               int flags_z,
               int flags_w
          )
{
     float* pr = m_photon_values ; 
     
     pr[0*4+0] = pos_x ;
     pr[0*4+1] = pos_y ;
     pr[0*4+2] = pos_z ;
     pr[0*4+3] = time ;

     pr[1*4+0] = dir_x ;
     pr[1*4+1] = dir_y ;
     pr[1*4+2] = dir_z ;
     pr[1*4+3] = weight  ;

     pr[2*4+0] = pol_x ;
     pr[2*4+1] = pol_y ;
     pr[2*4+2] = pol_z ;
     pr[2*4+3] = wavelength  ;

     uif_t flags[4] ;
     flags[0].i = flags_x ;   
     flags[1].i = flags_y ;   
     flags[2].i = flags_z ;   
     flags[3].i = flags_w ;   

     pr[3*4+0] = flags[0].f ;
     pr[3*4+1] = flags[1].f ;
     pr[3*4+2] = flags[2].f ;
     pr[3*4+3] = flags[3].f  ;

     m_photon->add(pr, m_photon_itemsize);

     m_photon_count += 1 ;

     if( m_photon_count % 10000 == 0 )
     {
         LOG(LEVEL) << " photon_count " << m_photon_count ;     
     }

}



