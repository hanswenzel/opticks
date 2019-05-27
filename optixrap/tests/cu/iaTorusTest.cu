#include <optix_world.h>
rtBuffer<rtCallableProgramId<unsigned(double,double,double,double*,unsigned)> > solve_callable ;

#include "cu/quad.h"
#include "cu/bbox.h"

typedef double Solve_t ;
#include "cu/Solve.h"

#define CSG_INTERSECT_TORUS_TEST 1
typedef double Torus_t ;
#include "cu/csg_intersect_torus.h"


using namespace optix;

rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );

rtBuffer<float4>  output_buffer;

RT_PROGRAM void iaTorusTest()
{
    unsigned long long photon_id = launch_index.x ;  
    unsigned int photon_offset = photon_id*4 ; 

#ifdef CSG_INTERSECT_TORUS_TEST
    rtPrintf("// iaTorusTest %llu\n", photon_id);
#endif



    //  calling the below double laden function twice is prone to segv in createPTXFromFile
    //csg_intersect_torus_scale_test(photon_id, false);
    csg_intersect_torus_scale_test(photon_id, true );

    
    output_buffer[photon_offset+0] = make_float4(40.f, 40.f, 40.f, 40.f);
    output_buffer[photon_offset+1] = make_float4(41.f, 41.f, 41.f, 41.f);
    output_buffer[photon_offset+2] = make_float4(42.f, 42.f, 42.f, 42.f);
    output_buffer[photon_offset+3] = make_float4(43.f, 43.f, 43.f, 43.f);
}

RT_PROGRAM void exception()
{
    rtPrintExceptionDetails();

    unsigned long long photon_id = launch_index.x ;  
    unsigned int photon_offset = photon_id*4 ; 
    
    output_buffer[photon_offset+0] = make_float4(-40.f, -40.f, -40.f, -40.f);
    output_buffer[photon_offset+1] = make_float4(-41.f, -41.f, -41.f, -41.f);
    output_buffer[photon_offset+2] = make_float4(-42.f, -42.f, -42.f, -42.f);
    output_buffer[photon_offset+3] = make_float4(-43.f, -43.f, -43.f, -43.f);
}
