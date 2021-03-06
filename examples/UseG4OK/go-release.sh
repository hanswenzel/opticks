#!/bin/bash -l
##
## Copyright (c) 2019 Opticks Team. All Rights Reserved.
##
## This file is part of Opticks
## (see https://bitbucket.org/simoncblyth/opticks).
##
## Licensed under the Apache License, Version 2.0 (the "License"); 
## you may not use this file except in compliance with the License.  
## You may obtain a copy of the License at
##
##   http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software 
## distributed under the License is distributed on an "AS IS" BASIS, 
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
## See the License for the specific language governing permissions and 
## limitations under the License.
##


pfx=$(opticks-release-prefix)
g4d=$(opticks-envg4-Geant4_DIR)

echo pfx $pfx
echo g4d $g4d


sdir=$(pwd)
bdir=/tmp/$USER/opticks/$(basename $sdir)/build 
idir=$HOME

rm -rf $bdir && mkdir -p $bdir && cd $bdir && pwd 
 

explicit()
{
    local spfx=$(opticks-envg4-prefix)    
    # use of source prefix here is naughty : but it avoids having to install geant4 again
    # just to mimic a users separately managed geant4 
    cmake3 $sdir \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_PREFIX_PATH="$pfx;$pfx/externals" \
        -DCMAKE_MODULE_PATH=$pfx/cmake/Modules \
        -DCMAKE_INSTALL_PREFIX=$idir \
        -DGeant4_DIR=$g4d \
        -DXERCESC_LIBRARY=$spfx/externals/lib/libxerces-c.so \
        -DXERCESC_INCLUDE_DIR=$spfx/externals/include
}

implicit()
{
    cmake3 $sdir \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_PREFIX_PATH="$pfx;$pfx/externals" \
        -DCMAKE_MODULE_PATH=$pfx/cmake/Modules \
        -DCMAKE_INSTALL_PREFIX=$idir \
        -DGeant4_DIR=$g4d 
}


#explicit
implicit



make
make install   

