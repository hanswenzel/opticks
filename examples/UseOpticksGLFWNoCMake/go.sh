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


opticks-
oe-

sdir=$(pwd)
name=$(basename $sdir)
bdir=/tmp/$USER/opticks/$name/build 

rm -rf $bdir && mkdir -p $bdir && cd $bdir && pwd 

pkg=OpticksGLFW
pc=glfw3    

echo gcc -c $sdir/Use$pkg.cc $(oc -cflags $pc)
     gcc -c $sdir/Use$pkg.cc $(oc -cflags $pc)
echo gcc Use$pkg.o -o Use$pkg $(oc -libs $pc --static) -lstdc++ 
     gcc Use$pkg.o -o Use$pkg $(oc -libs $pc --static) -lstdc++
echo DISPLAY=:0 ./Use$pkg
     DISPLAY=:0 ./Use$pkg

printf "\n\n%s\n"  "known to fail when headless"


