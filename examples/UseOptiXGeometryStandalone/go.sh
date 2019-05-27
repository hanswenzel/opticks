#!/bin/bash -l


sdir=$(pwd)
name=$(basename $sdir)

prefix=/tmp/$USER/opticks/$name

export PREFIX=$prefix
export PATH=$PREFIX/bin:$PATH

bdir=$prefix/build 
echo bdir $bdir name $name prefix $prefix



glm-dir(){  echo $prefix/externals/glm/$(glm-name) ; }
glm-version(){ echo 0.9.9.5 ; }
glm-name(){    echo glm-$(glm-version) ; }
glm-url(){    echo https://github.com/g-truc/glm/releases/download/$(glm-version)/$(glm-name).zip ; }
glm-dist(){    echo $(dirname $(glm-dir))/$(basename $(glm-url)) ; }

glm-get(){
   local dir=$(dirname $(glm-dir)) &&  mkdir -p $dir && cd $dir
   local url=$(glm-url)
   local zip=$(basename $url)
   local nam=$(glm-name)
   local opt=$( [ -n "${VERBOSE}" ] && echo "" || echo "-q" )

   [ ! -f "$zip" ] && curl -L -O $url
   [ ! -d "$nam" ] && unzip $opt $zip -d $nam
   ln -sfnv $(glm-name)/glm glm 
   echo symbolic link for access without version in path
}


glm-get


optix-install-dir(){ echo ${OPTIX_INSTALL_DIR:-/usr/local/OptiX_600} ; }


echo optix-install-dir : $(optix-install-dir)

rm -rf $bdir && mkdir -p $bdir 
cd $bdir && pwd 
ls -l 

cmake $sdir \
   -DCMAKE_BUILD_TYPE=Debug \
   -DCMAKE_PREFIX_PATH=$prefix/externals \
   -DCMAKE_INSTALL_PREFIX=$prefix \
   -DCMAKE_MODULE_PATH=$(optix-install-dir)/SDK/CMake \
   -DOptiX_INSTALL_DIR=$(optix-install-dir) 


rm -rf $prefix/ptx
rm -rf $prefix/bin
rm -rf $prefix/ppm

mkdir -p $prefix/{ptx,bin,ppm} 
make
make install   

echo running $(which $name)
$name

rc=$?
[ ! $rc -eq 0 ] && echo non-zero RC && exit


ppm=$prefix/ppm/$name.ppm

[ ! -f "$ppm" ] && echo failed to write ppm $ppm && exit


echo ppm $ppm
ls -l $ppm
open $ppm    ## create an open function such as "gio open" if using gnome

