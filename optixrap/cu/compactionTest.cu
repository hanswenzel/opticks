#include <optix_world.h>
#include "quad.h"

using namespace optix;

rtBuffer<float4>   photon_buffer ;
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );
rtDeclareVariable(uint2, launch_dim,   rtLaunchDim, );
rtDeclareVariable(unsigned int,  PNUMQUAD, , );


RT_PROGRAM void compactionTest()
{
    unsigned long long photon_id = launch_index.x ;  
    unsigned int photon_offset = unsigned(photon_id)*PNUMQUAD ; 
    union quad q0,q1,q2,q3 ;

    q0.f = photon_buffer[photon_offset+0] ;   
    q1.f = photon_buffer[photon_offset+1] ;   
    q2.f = photon_buffer[photon_offset+2] ;   
    q3.f = photon_buffer[photon_offset+3] ;   

    rtPrintf("compactionTest.cu  %u fffu(%10f, %10f, %10f, %u) \n", photon_id, q0.f.x, q1.f.y, q2.f.z, q3.u.w );
}

RT_PROGRAM void exception()
{
    rtPrintExceptionDetails();
}


