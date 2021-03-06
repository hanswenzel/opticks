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

#include <vector>
#include <string>
#include "PLOG.hh"

#include "BStr.hh"
#include "OFunc.hh"
#include "OContext.hh"


OFunc::OFunc(OContext* ocontext, const char* ptxname, const char* ctxname, const char* funcnames)
     : 
     m_ocontext(ocontext),
     m_context(ocontext->getContext()),
     m_ptxname(strdup(ptxname)),
     m_ctxname(strdup(ctxname)),
     m_funcnames(strdup(funcnames))
{
}


void OFunc::convert()
{
    std::vector<std::string> names ; 
    BStr::split(names, m_funcnames, ','); 

    unsigned num_funcs = names.size() ; 

    LOG(info) << "OFunc::convert"
              << " ptxname " << m_ptxname
              << " ctxname " << m_ctxname
              << " funcnames  " << m_funcnames
              << " num_funcs " << num_funcs
              ;

    optix::Buffer callable = m_context->createBuffer( RT_BUFFER_INPUT, RT_FORMAT_PROGRAM_ID, num_funcs );
    int* callable_id = static_cast<int*>( callable->map() );

    for(unsigned i=0 ; i < num_funcs ; i++)
    {
        const char* name = names[i].c_str() ; 
        optix::Program callableProg = m_ocontext->createProgram(m_ptxname,  name  ); 
        callable_id[i] = callableProg->getId();

         LOG(info) << "OFunc::convert"
                   << " id " << callable_id[i]
                   << " name " << name
                   ;

    }
    callable->unmap();

    m_context[m_ctxname]->set( callable );
}





