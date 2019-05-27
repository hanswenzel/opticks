#include "OptiXTest.hh"
#include "NPY.hpp"
#include "OPTICKS_LOG.hh"


int main( int argc, char** argv ) 
{
    OPTICKS_LOG(argc, argv);

    optix::Context context = optix::Context::create();

    const char* buildrel = "optixrap/tests" ; 
    const char* cmake_target = "minimalTest" ; 
    OptiXTest* test = new OptiXTest(context, "minimalTest.cu", "minimal", "exception", buildrel, cmake_target ) ;
    test->Summary(argv[0]);

    // for inknown reasons this has become slow, taking 30s for 512x512
    //unsigned width = 512 ; 
    //unsigned height = 512 ; 

    unsigned width = 16 ; 
    unsigned height = 16 ; 



    // optix::Buffer buffer = context->createBuffer( RT_BUFFER_OUTPUT, RT_FORMAT_FLOAT4, width, height );
    optix::Buffer buffer = context->createBuffer( RT_BUFFER_OUTPUT, RT_FORMAT_FLOAT4, width*height );
    context["output_buffer"]->set(buffer);

    context->validate();
    context->compile();

    context->launch(0, width, height);


    NPY<float>* npy = NPY<float>::make(width, height, 4) ;
    npy->zero();

    void* ptr = buffer->map() ; 
    npy->read( ptr );
    buffer->unmap(); 

    const char* path = "$TMP/minimalTest.npy";
    std::cerr << "save result npy to " << path << std::endl ; 
 
    npy->save(path);


    return 0;
}