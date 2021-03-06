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

#pragma once
/**
OPropertyLib
===============

Base class of property libs providing buffer upload and dumping.
Subclasses include:

* :doc:`OBndLib`
* :doc:`OSourceLib`
* :doc:`OScintillatorLib`

**/


#include "OXPPNS.hh"
template <typename T> class NPY ;

#include "OXRAP_API_EXPORT.hh"

class OXRAP_API OPropertyLib  {
    public:
        OPropertyLib(optix::Context& ctx, const char* name);
    public:
    protected:
        void upload(optix::Buffer& optixBuffer, NPY<float>* buffer);
        void dumpVals( float* vals, unsigned int n);
    protected:
        optix::Context       m_context ; 
        const char*          m_name ; 

};


