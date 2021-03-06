#!/bin/bash 
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


gogo-find-0(){ find . -name go.sh ; }
gogo-find-1(){ ls -1 *NoCMake/go.sh ; }              # NoCMake tests
gogo-find-2(){ ls -1 */go.sh | grep -v NoCMake ; }   # CMake based tests 
gogo-find-3(){ ls -1 */goc.sh ; }                    # opticks-goc tests building based on opticks-config 

gogo-find(){ 
   for arg in $* ; do 
       gogo-find-$arg
   done 
}

gogo-fails(){ cat << EON


./gogo.sh 1
   skipping UseGeant4

./gogo.sh 2

  macOS:
    UseOGLRapMinimal/go.sh                     1    
         link fail : Composition::setCenterExtent  GLM vec args  
         FIXED after clean install of Opticks 

    UseOptiXProgramPP/go.sh                  139    
         segmenting at the launch with OptiX 50001
         working fine with OptiX 650

EON
}



gogo-all(){
    local go
    gogo-find $* | while read go ; do 
        gogo-one $go
    done
}


gogo-one-()
{
    local go=$1
    local nam=$(basename $go)
    local rc
    ./$nam > /dev/null 2>&1
    rc=$?

    local note=""
    [ -f "NOTE" ] && note="$(head -1 NOTE)"

    printf "%-40s %d %s\n" $go $rc "$note"
}

gogo-one(){
    local go=$1 
    local iwd=$PWD

    local dir=$(dirname $go)
    cd $dir 

    printf "%-40s \n" $go 

    if [ -f "SKIP" ]; then
        printf "%-40s %s %s \n" $go "SKIP" "$(head -1 SKIP)"
    else 
        gogo-one- $go 
    fi 

    cd $iwd
}

gogo-all $*
#gogo-find $*


