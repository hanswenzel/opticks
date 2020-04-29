#!/bin/bash -l

opticks-

sdir=$(pwd)
name=$(basename $sdir) 
bdir=/tmp/$USER/opticks/$name/build 

rm   -rf $bdir && mkdir -p $bdir && cd $bdir && pwd 


prefix=/tmp/$USER/opticks

cmake $sdir \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=$prefix \
    -DOPTICKS_PREFIX=$(opticks-prefix)

# OPTICKS_PREFIX is used to find the plog header 

make
make install   

bin=$prefix/lib/ChainedApp
txt=ChainedApp.txt

case $(uname) in 
   Darwin) otool -L $bin ;;
   Linux) ldd $bin ;;
esac

echo $bin
$bin


ls -l $txt
cat $txt


