# === func-gen- : cuda/cuda fgp cuda/cuda.bash fgn cuda fgh cuda
cuda-src(){      echo cuda/cuda.bash ; }
cuda-source(){   echo ${BASH_SOURCE:-$(env-home)/$(cuda-src)} ; }
cuda-vi(){       vi $(cuda-source) ; }
cuda-env(){      elocal- ; cuda-path ; }
cuda-usage(){ cat << EOU

CUDA
======

See Also
---------

* cudainstall-
* cudatoolkit-



tips to free up GPU memory
---------------------------

#. minimise the number of apps and windows running
#. put the machine to sleep and go and have a coffee

#. dont use dedicated GPU mode, this avoids most GPU memory concerns
   presumably as it reduces prsssure on the GPU


CUDA Driver and Runtime API interop
------------------------------------

* http://stackoverflow.com/questions/20539349/cuda-runtime-api-and-dynamic-kernel-definition

  * http://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__DRIVER.html#group__CUDART__DRIVER


Bit packing with CUDA vector types
-------------------------------------

* http://nvlabs.github.io/cub/index.html


nvcc compilation for newer architectures
------------------------------------------

Some PTX instructions are only supported on devices of higher compute
capabilities. For example, warp shuffle instructions are only supported on
devices of compute capability 3.0 and above. The -arch compiler option
specifies the compute capability that is assumed when compiling C to PTX code.
So, code that contains warp shuffle, for example, must be compiled with
-arch=sm_30 (or higher).

PTX code produced for some specific compute capability can always be compiled
to binary code of greater or equal compute capability.


::

    delta:~ blyth$ nvcc -o /usr/local/env/cuda/texture/cuda_texture_object -arch=sm_30 /Users/blyth/env/cuda/texture/cuda_texture_object.cu
    delta:~ blyth$ nvcc -o /usr/local/env/cuda/texture/cuda_texture_object  /Users/blyth/env/cuda/texture/cuda_texture_object.cu
    /Users/blyth/env/cuda/texture/cuda_texture_object.cu(10): error: type name is not allowed

    /Users/blyth/env/cuda/texture/cuda_texture_object.cu(10): warning: expression has no effect

    1 error detected in the compilation of "/tmp/tmpxft_00017bac_00000000-6_cuda_texture_object.cpp1.ii".
    delta:~ blyth$ 


syslog : Understanding XID Errors
------------------------------------

* http://docs.nvidia.com/deploy/xid-errors/index.html



deviceQuery
-------------

::

    delta:~ blyth$ cuda-samples-bin-deviceQuery
    running /usr/local/env/cuda/NVIDIA_CUDA-5.5_Samples/bin/x86_64/darwin/release/deviceQuery
    /usr/local/env/cuda/NVIDIA_CUDA-5.5_Samples/bin/x86_64/darwin/release/deviceQuery Starting...

     CUDA Device Query (Runtime API) version (CUDART static linking)

    Detected 1 CUDA Capable device(s)

    Device 0: "GeForce GT 750M"
      CUDA Driver Version / Runtime Version          5.5 / 5.5
      CUDA Capability Major/Minor version number:    3.0
      Total amount of global memory:                 2048 MBytes (2147024896 bytes)
      ( 2) Multiprocessors, (192) CUDA Cores/MP:     384 CUDA Cores
      GPU Clock rate:                                926 MHz (0.93 GHz)
      Memory Clock rate:                             2508 Mhz
      Memory Bus Width:                              128-bit
      L2 Cache Size:                                 262144 bytes
      Maximum Texture Dimension Size (x,y,z)         1D=(65536), 2D=(65536, 65536), 3D=(4096, 4096, 4096)
      Maximum Layered 1D Texture Size, (num) layers  1D=(16384), 2048 layers
      Maximum Layered 2D Texture Size, (num) layers  2D=(16384, 16384), 2048 layers
      Total amount of constant memory:               65536 bytes
      Total amount of shared memory per block:       49152 bytes
      Total number of registers available per block: 65536
      Warp size:                                     32
      Maximum number of threads per multiprocessor:  2048
      Maximum number of threads per block:           1024
      Max dimension size of a thread block (x,y,z): (1024, 1024, 64)
      Max dimension size of a grid size    (x,y,z): (2147483647, 65535, 65535)
      Maximum memory pitch:                          2147483647 bytes
      Texture alignment:                             512 bytes
      Concurrent copy and kernel execution:          Yes with 1 copy engine(s)
      Run time limit on kernels:                     Yes
      Integrated GPU sharing Host Memory:            No
      Support host page-locked memory mapping:       Yes
      Alignment requirement for Surfaces:            Yes
      Device has ECC support:                        Disabled
      Device supports Unified Addressing (UVA):      Yes
      Device PCI Bus ID / PCI location ID:           1 / 0
      Compute Mode:
         < Default (multiple host threads can use ::cudaSetDevice() with device simultaneously) >

    deviceQuery, CUDA Driver = CUDART, CUDA Driver Version = 5.5, CUDA Runtime Version = 5.5, NumDevs = 1, Device0 = GeForce GT 750M
    Result = PASS



bandwidthTest
--------------

::

    delta:~ blyth$ cuda-samples-bin-bandwidthTest
    running /usr/local/env/cuda/NVIDIA_CUDA-5.5_Samples/bin/x86_64/darwin/release/bandwidthTest
    [CUDA Bandwidth Test] - Starting...
    Running on...

     Device 0: GeForce GT 750M
     Quick Mode

     Host to Device Bandwidth, 1 Device(s)
     PINNED Memory Transfers
       Transfer Size (Bytes)    Bandwidth(MB/s)
       33554432         1566.0

     Device to Host Bandwidth, 1 Device(s)
     PINNED Memory Transfers
       Transfer Size (Bytes)    Bandwidth(MB/s)
       33554432         3182.6

     Device to Device Bandwidth, 1 Device(s)
     PINNED Memory Transfers
       Transfer Size (Bytes)    Bandwidth(MB/s)
       33554432         17074.5

    Result = PASS
    delta:~ blyth$



Profiling
-----------

nvvp and nsight
~~~~~~~~~~~~~~~~~~

::

    delta:doc blyth$ nvvp
    Unable to find any JVMs matching architecture "i386".
    No Java runtime present, try --request to install.
    No Java runtime present, requesting install.

    delta:doc blyth$ which nsight
    /Developer/NVIDIA/CUDA-5.5/bin/nsight
    delta:doc blyth$ nsight
    Unable to find any JVMs matching architecture "i386".
    No Java runtime present, try --request to install.
    No Java runtime present, requesting install.
    delta:doc blyth$ 




FindCUDA.cmake
---------------

* http://www.cmake.org/cmake/help/v3.0/module/FindCUDA.html

* http://stackoverflow.com/questions/13683575/cuda-5-0-separate-compilation-of-library-with-cmake


CUDA CMAKE Mavericks
-----------------------

Informative explanation of Mavericks CUDA challenges wrt compiler flags...
 
https://github.com/cudpp/cudpp/wiki/BuildingCUDPPWithMavericks

See::

   cudawrap-
   thrustrap-
   thrusthello-
   thrust-



CUDA OSX libc++
-----------------






CUDA Release Notes
-------------------

* http://docs.nvidia.com/cuda/cuda-toolkit-release-notes/

On Mac OS X, libc++ is supported with XCode 5.x. Command-line option -Xcompiler
-stdlib=libstdc++ is no longer needed when invoking NVCC. Instead, NVCC uses
the default library that Clang chooses on Mac OS X. Users are still able to
choose between libc++ and libstdc++ by passing -Xcompiler -stdlib=libc++ or
-Xcompiler -stdlib=libstdc++ to NVCC.

The Runtime Compilation library (nvrtc) provides an API to compile CUDA-C++
device source code at runtime. The resulting compiled PTX can be launched on a
GPU using the CUDA Driver API. More details can be found in the libNVRTC User
Guide.

Added C++11 support. The new nvcc flag -std=c++11 turns on C++11 features in
the CUDA compiler as well as the host compiler and linker. The flag is
supported by host compilers Clang and GCC versions 4.7 and newer. In addition,
any C++11 features that are enabled by default by a supported host compiler are
also allowed to be used in device code. Please see the CUDA Programming Guide
for further details.



FUNCTIONS
---------


cuda-get

EOU
}

cuda-export(){
   echo -n
}

cuda-nvcc-flags(){
    case $NODE_TAG in 
       D) echo -ccbin /usr/bin/clang --use_fast_math ;;
       *) echo --use_fast_math ;; 
    esac 
}


cuda-version(){      echo ${CUDA_VERSION:-5.5} ; }
#cuda-version(){      echo ${CUDA_VERSION:-7.0} ; }
cuda-download-dir(){ echo $(local-base)/env/cuda ; }
cuda-dir(){          echo $(cuda-dir-$(uname)) ; }
cuda-dir-Linux(){    echo /usr/local/cuda-$(cuda-version) ; }
cuda-dir-Darwin(){   echo /Developer/NVIDIA/CUDA-$(cuda-version) ; }
cuda-idir(){         echo $(cuda-dir)/include ; }
cuda-writable-dir(){ echo $(local-base)/env/cuda ; } 
cuda-samples-dir(){  echo $(cuda-writable-dir)/NVIDIA_CUDA-$(cuda-version)_Samples ; }

cuda-cd(){           cd $(cuda-dir); }
cuda-icd(){          cd $(cuda-idir); }
cuda-dcd(){          cd $(cuda-download-dir); }
cuda-wcd(){          cd $(cuda-writable-dir); }
cuda-samples-cd(){   cd $(cuda-samples-dir) ; }

cuda-find(){ find $(cuda-idir) -name '*.h' -exec grep -H ${1:-cudaGraphics} {} \; ; }

cuda-libdir(){
   case $NODE_TAG in 
      D) echo /usr/local/cuda/lib ;;
      G1) echo /usr/local/cuda/lib64 ;;
   esac
}

cuda-path(){
    local dir=$(cuda-dir)
    [ ! -d $dir ] && return 1
    export PATH=$dir/bin:$PATH
    local libdir=$(cuda-libdir)

    if [ "$(uname)" == "Darwin" ]; then 
       export DYLD_LIBRARY_PATH=$libdir:$DYLD_LIBRARY_PATH      # not documented ??? links from this to $dir/lib
    else
       export LD_LIBRARY_PATH=$libdir:$LD_LIBRARY_PATH
    fi
    # these are not seen by the pycuda build
}


cuda-url(){ echo $(cuda-url-$(uname)) ; }
cuda-url-Darwin(){
   case $(cuda-version) in 
       5.5) echo http://developer.download.nvidia.com/compute/cuda/5_5/rel/installers/cuda-mac-5.5.28_10.9_64.pkg ;;
       7.0) echo http://developer.download.nvidia.com/compute/cuda/7_0/Prod/local_installers/cuda_7.0.29_mac.pkg ;;
   esac
}
cuda-pkg(){  echo $(basename $(cuda-url)) ; }
cuda-get(){
   local dir=$(cuda-download-dir) &&  mkdir -p $dir && cd $dir
   local url=$(cuda-url-$(uname))
   local pkg=$(cuda-pkg)
   [ -d $(cuda-dir) ] && echo $msg CUDA is already installed at $(cuda-dir) && return 0 
   [ ! -f "$pkg" ] && curl -L -O $url 
}

cuda-gui-install(){
   local pkg=$(cuda-pkg)
   open $pkg   
}


cuda-osx-pkginfo(){         installer -pkginfo -pkg $(dirname $(cuda-download-dir))/$(cuda-pkg) ; }
cuda-osx-getting-started(){ open $(cuda-dir)/doc/html/cuda-getting-started-guide-for-mac-os-x/index.html ; }
cuda-guide(){               open $(cuda-dir)/doc/html/cuda-c-programming-guide/index.html ; }
cuda-doc(){                 open $(cuda-dir)/doc/html/index.html ; }
cuda-osx-kextstat(){        kextstat | grep -i cuda ; }


cuda-samples-make(){
   cuda-samples-cd
   make $*
}
cuda-samples-install(){
   local iwd=$PWD
   local dir=$(cuda-writable-dir)
   [ ! -d "$dir" ] && mkdir -p $dir
  
   # cuda-install-samples-5.5.sh $dir
   # have to do it manually to skip the doc folder which goes over afs quota

   local src=$(dirname $(dirname $(which cuda-install-samples-$(cuda-version).sh)))/samples
   local dst=$(cuda-samples-dir)
   local path

   cd $src
   local name
   ls -1 $src | while read path ; do
      name=$(basename $path)
      if [ "$name" == "doc" ]; then
         echo skip $path
      else
         cp -R $path $dst/
      fi
   done
   cd $iwd

}
cuda-samples-bin-dir(){ echo $(cuda-samples-dir)/bin/$(uname -m)/$(uname | tr '[:upper:]' '[:lower:]')/release ; }
cuda-samples-bin-cd(){  cd $(cuda-samples-bin-dir) ; }

cuda-samples-bin-run(){  
    local bin=$(cuda-samples-bin-dir)/$1 
    shift   
    local cmd="$bin $*"
    [ ! -x $bin ] && echo $msg bin $bin not found OR not executable && return 1 
    [ -x $bin ] && echo $msg running $cmd
    eval $cmd
}

cuda-samples-bin-deviceQuery(){    cuda-samples-bin-run deviceQuery $* ; }
cuda-samples-bin-bandwidthTest(){  cuda-samples-bin-run bandwidthTest $* ; }
cuda-samples-bin-smokeParticles(){ cuda-samples-bin-run smokeParticles $* ; }
cuda-samples-bin-fluidsGL(){       cuda-samples-bin-run fluidsGL $* ; }

cuda-deviceQuery(){ cuda-samples-bin-run deviceQuery $* ; } 
