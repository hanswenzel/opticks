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


sdir=$(pwd)
bdir=/tmp/$USER/opticks/$(basename $sdir)/build 

rm -rf $bdir && mkdir -p $bdir && cd $bdir && pwd 


oc-

om-
om-export
om-export-info

pkg=Geant4
om-export-find $pkg


echo gcc -c $sdir/Use$pkg.cc $(oc-cflags $pkg)
     gcc -c $sdir/Use$pkg.cc $(oc-cflags $pkg)
echo gcc Use$pkg.o -o Use$pkg $(oc-libs $pkg)  #-lstdc++
     gcc Use$pkg.o -o Use$pkg $(oc-libs $pkg) #-lstdc++

# on Darwin with clang should be -lc++ but I think there is some compat to make -lstdc++ work 

./Use$pkg



