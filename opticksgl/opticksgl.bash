opticksgl-rel(){      echo opticksgl ; }
opticksgl-src(){      echo opticksgl/opticksgl.bash ; }
opticksgl-source(){   echo ${BASH_SOURCE:-$(opticks-home)/$(opticksgl-src)} ; }
opticksgl-vi(){       vi $(opticksgl-source) ; }
opticksgl-usage(){ cat << EOU

OpticksGL
==========

Classes depending on OptiX optixrap- and OpenGL

Classes
--------

OpViz
    High level connector between opticksop-/OpEngine and oglrap-/Scene
    with *render* method that performs OptiX ray trace and 
    pushes results to OpenGL texture

ORenderer
    connection between OpenGL renderer and OptiX ray trace

OFrame
    mechanics of OpenGL texture and OptiX buffer handling 
    and pushing from buffer to texture
    TODO: maybe dont provide public header for this


EOU
}


opticksgl-sdir(){ echo $(opticks-home)/opticksgl ; }
opticksgl-tdir(){ echo $(opticks-home)/opticksgl/tests ; }
opticksgl-idir(){ echo $(opticks-idir) ; }
opticksgl-bdir(){ echo $(opticks-bdir)/$(opticksgl-rel) ; }

opticksgl-c(){    cd $(opticksgl-sdir); }
opticksgl-cd(){   cd $(opticksgl-sdir); }
opticksgl-scd(){  cd $(opticksgl-sdir); }
opticksgl-tcd(){  cd $(opticksgl-tdir); }
opticksgl-icd(){  cd $(opticksgl-idir); }
opticksgl-bcd(){  cd $(opticksgl-bdir); }


opticksgl-env(){  
   olocal- 
   opticks-
}


opticksgl-name(){ echo OpticksGL ; }
opticksgl-tag(){ echo OKGL ; }

opticksgl-wipe(){ local bdir=$(opticksgl-bdir) ; rm -rf $bdir ; } 

opticksgl-apihh(){  echo $(opticksgl-sdir)/$(opticksgl-tag)_API_EXPORT.hh ; }
opticksgl---(){     touch $(opticksgl-apihh) ; opticksgl--  ; } 



opticksgl--(){                   opticks-- $(opticksgl-bdir) ; } 
opticksgl-ctest(){               opticks-ctest $(opticksgl-bdir) $* ; } 
opticksgl-genproj() { opticksgl-scd ; opticks-genproj $(opticksgl-name) $(opticksgl-tag) ; } 
opticksgl-gentest() { opticksgl-tcd ; opticks-gentest ${1:-Example} $(opticksgl-tag) ; } 
opticksgl-txt(){ vi $(opticksgl-sdir)/CMakeLists.txt $(opticksgl-tdir)/CMakeLists.txt ; } 






