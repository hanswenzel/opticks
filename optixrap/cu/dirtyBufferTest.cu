#include <optix_world.h>

using namespace optix;

rtBuffer<unsigned>   in_buffer ;
rtBuffer<unsigned>  out_buffer ;
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );
rtDeclareVariable(uint2, launch_dim,   rtLaunchDim, );


RT_PROGRAM void dirtyBufferTest()
{
    unsigned long long index = launch_index.x ;
    unsigned val = in_buffer[index] ; 

    //rtPrintf("dirtyBufferTest.cu  %u  \n", val );

    out_buffer[index] = val*2 ; 
}

RT_PROGRAM void exception()
{
    rtPrintExceptionDetails();
}


