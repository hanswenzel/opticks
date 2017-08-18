gleq-src(){      echo externals/gleq.bash ; }
gleq-source(){   echo ${BASH_SOURCE:-$(opticks-home)/$(gleq-src)} ; }
gleq-vi(){       vi $(gleq-source) ; }
gleq-env(){      olocal- ; opticks- ; }
gleq-usage(){ cat << EOU


From mountains- whilst trying to 
port from SFML to GLFW/GLEW/GLEQ get 
compilation issue (from compiling C as C++)::

    make: *** [all] Error 2
    [ 20%] Building CXX object CMakeFiles/Mountains.dir/main.cpp.o
    In file included from /usr/local/env/graphics/opengl/mountains/main.cpp:17:
    In file included from /usr/local/env/graphics/opengl/mountains/GLEQ.hh:7:
    /usr/local/opticks/externals/gleq/gleq.h:274:23: error: assigning to 'char **' from incompatible type 'void *'
        event->file.paths = malloc(count * sizeof(char*));
                          ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    1 error generated.


Fix easy, but why this has no effect in oglrap-:

    event->file.paths = (char**)malloc(count * sizeof(char*));


EOU
}
gleq-dir(){ echo $(opticks-prefix)/externals/gleq ; }
gleq-sdir(){ echo $(opticks-home)/graphics/gleq ; }
gleq-cd(){  cd $(gleq-dir); }
gleq-scd(){  cd $(gleq-sdir); }
gleq-edit(){ vi $(opticks-home)/cmake/Modules/FindGLEQ.cmake ; }

gleq-url(){ echo https://github.com/simoncblyth/gleq ; }

gleq-get(){
   local iwd=$PWD
   local dir=$(dirname $(gleq-dir)) &&  mkdir -p $dir && cd $dir
   [ ! -d gleq ] && git clone $(gleq-url)
   cd $iwd
}
gleq-hdr(){
   echo $(gleq-dir)/gleq.h
}

gleq--(){
   gleq-get
}

