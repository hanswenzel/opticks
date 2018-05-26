#!/bin/bash -l

opticks-

sdir=$(pwd)
name=$(basename $sdir)

bdir=$(opticks-prefix)/build/$name
#rm -rf $bdir 
mkdir -p $bdir && cd $bdir && pwd 

cmake $sdir \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH=$(opticks-prefix)/externals \
    -DCMAKE_INSTALL_PREFIX=$(opticks-prefix) \
    -DCMAKE_MODULE_PATH=$(opticks-home)/cmake/Modules 

# not needed when not downstream from OptiX
#    -DOptiX_INSTALL_DIR=$(opticks-optix-install-dir)

make
make install   
