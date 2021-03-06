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

#include "SSys.hh"
#include "OPTICKS_LOG.hh"

#include "BFile.hh"
#include "DummyGenstepsNPY.hpp"
#include "NPY.hpp"

const char* TMPDIR = "$TMP/npy/DummyGenstepsNPYTest" ; 

int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv);

    NPY<float>* npy = DummyGenstepsNPY::Make(100);


    std::string p = BFile::FormPath(TMPDIR, "DummyGenstepsNPYTest.npy"); 
    const char* path = p.c_str() ;
    npy->save(path);

    SSys::npdump(path);

    return 0 ; 
}
