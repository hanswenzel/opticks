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

// TEST=OOtexTest om-t

#include "NPY.hpp"
#include "OXPPNS.hh"
#include "OContext.hh"
#include "OConfig.hh"
#include "OPTICKS_LOG.hh"


/**
texTest
==========

Technical tex roundtrip test.


**/


const char* TMPDIR = "$TMP/optixrap/texTest" ; 

int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv);    

    LOG(info) << " ok " ; 

    int nx = 16 ; 
    int ny = 16 ; 
    int nz = 4 ; 

    NPY<float>* inp = NPY<float>::make(nx, ny, nz);
    inp->zero();
    float* values = inp->getValues();
    for(int i=0 ; i < nx ; i++){
    for(int j=0 ; j < ny ; j++){
    for(int k=0 ; k < nz ; k++)
    {
       int index = i*ny*nz + j*nz + k ;
       *(values + index) = float(index) ;
    }
    }
    }
    //inp->dump();
    inp->save(TMPDIR,"texTest_inp.npy");

    OContext::SetupOptiXCachePathEnvvar(); 
    optix::Context context = optix::Context::create();
    context->setRayTypeCount(1); 
    context->setExceptionEnabled( RT_EXCEPTION_ALL , true ); 

    optix::Buffer outBuffer = context->createBuffer(RT_BUFFER_OUTPUT, RT_FORMAT_FLOAT4, nx, ny);
    optix::Buffer texBuffer = context->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT4, nx, ny);

    memcpy( texBuffer->map(), inp->getBytes(), inp->getNumBytes(0) );
    texBuffer->unmap(); 

    optix::TextureSampler tex = context->createTextureSampler();

#ifdef TEXT_TEST_MANUAL
    tex->setWrapMode(0, RT_WRAP_REPEAT);
    tex->setWrapMode(1, RT_WRAP_REPEAT);
    tex->setFilteringModes(RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE);

    //RTtextureindexmode indexingmode = RT_TEXTURE_INDEX_ARRAY_INDEX ; 
    RTtextureindexmode indexingmode = RT_TEXTURE_INDEX_NORMALIZED_COORDINATES ; 
    tex->setIndexingMode(indexingmode);  

    //RTtexturereadmode readmode = RT_TEXTURE_READ_NORMALIZED_FLOAT ;
    RTtexturereadmode readmode = RT_TEXTURE_READ_ELEMENT_TYPE ;    // No conversion
    tex->setReadMode(readmode);

    tex->setMaxAnisotropy(1.0f);
    tex->setMipLevelCount(1);
    tex->setArraySize(1);
    tex->setBuffer(0, 0, texBuffer);
#else
    OConfig::configureSampler(tex, texBuffer);
#endif
    //context["some_texture"]->setTextureSampler(tex);
    context["out_buffer"]->setBuffer(outBuffer);   
    context["tex_param"]->setInt(optix::make_int4(tex->getId(), 0, 0, 0 ));


    const char* cmake_target = "texTest" ; 
    const char* ptxrel = "tests" ; 
    OConfig* cfg = new OConfig(context, cmake_target, ptxrel ); 

    bool defer = true ;
    unsigned int entry = cfg->addEntry("texTest.cu", "texTest", "exception", defer);

    context->setEntryPointCount(1) ;

    cfg->dump();
    cfg->apply();     


    context->setPrintEnabled(true);
    context->setPrintBufferSize(8192);

    context->validate();
    context->compile();
    context->launch(entry,  0, 0);
    context->launch(entry, nx, ny);


    NPY<float>* out = NPY<float>::make(nx, ny, nz);
    out->read( outBuffer->map() );
    outBuffer->unmap(); 

    //out->dump();
    out->save(TMPDIR,"texTest_out.npy");

    float maxdiff = inp->maxdiff(out);
    LOG(info) << "maxdiff " << maxdiff  ; 

    assert(maxdiff < 1e-6 );

    return 0 ;     
}

