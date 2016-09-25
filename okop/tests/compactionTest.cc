#include "SSys.hh"

#include "NGLM.hpp"
#include "NPY.hpp"
#include "DummyPhotonsNPY.hpp"

#include "OContext.hh"
#include "OBuf.hh"
#include "TBuf.hh"
#include "OXPPNS.hh"

#include "PLOG.hh"

#include "OXRAP_LOG.hh"
#include "NPY_LOG.hh"


/**

compactionTest
=================

Objective: download part of a GPU photon buffer (N,4,4) ie N*4*float4 
with minimal hostside memory allocation.

Thrust based approach:

* determine number of photons passing some criteria (eg with an associated PMT identifier)
* allocate temporary GPU hit_buffer and use thrust::copy_if to fill it  
* allocate host side hit_buffer sized appropriately and pull back the hits into it 

See thrustrap-/tests/TBuf4x4Test.cu for development of the machinery 
based on early demo code from env-;optixthrust- with the simplification 
of using a float4x4 type for the Thrust photons buffer description.

**/


int main(int argc, char** argv)
{
    PLOG_(argc, argv);

    OXRAP_LOG__ ; 
    NPY_LOG__ ; 

    unsigned num_photons = 100 ; 
    unsigned PNUMQUAD = 4 ;

    NPY<float>* pho = DummyPhotonsNPY::make(num_photons);

    optix::Context context = optix::Context::create();
    OContext ctx(context, OContext::COMPUTE, false );  // with_top:false

    int entry = ctx.addEntry("compactionTest.cu.ptx", "compactionTest", "exception");

    optix::Buffer photon_buffer = context->createBuffer( RT_BUFFER_INPUT );
    photon_buffer->setFormat(RT_FORMAT_FLOAT4);
    photon_buffer->setSize(num_photons*PNUMQUAD) ; 

    OBuf* pbuf = new OBuf("photon",photon_buffer);

    context[ "PNUMQUAD" ]->setUint( PNUMQUAD );   // quads per photon
    context["photon_buffer"]->setBuffer(photon_buffer);  

    ctx.launch( OContext::VALIDATE|OContext::COMPILE|OContext::PRELAUNCH,  entry,  0, 0, NULL);


    OContext::upload<float>( photon_buffer, pho );

    ctx.launch( OContext::LAUNCH, entry, num_photons , 1, NULL ); 


    CBufSpec cpho = pbuf->bufspec();   // getDevicePointer happens here with OBufBase::bufspec

    if(cpho.size != 4*num_photons)
        LOG(fatal) << " MISMATCH " 
                   << " cpho.size " <<  cpho.size
                   << " 4*num_photons " <<  4*num_photons
                   ;

    assert(cpho.size == 4*num_photons );
    cpho.size = num_photons ;   //  decrease size by factor of 4 in order to increase "item" from 1*float4 to 4*float4 

    TBuf tpho("tpho", cpho );
    //tpho.dump<unsigned>("tpho.dump<unsigned>(16,4*3+0,16*num_photons)", 16, 4*3+0, 16*num_photons );
    //tpho.dump<unsigned>("tpho.dump<unsigned>(16,4*3+3,16*num_photons)", 16, 4*3+3, 16*num_photons );

    NPY<float>* hit = NPY<float>::make(0,4,4);

    tpho.downloadSelection4x4("thit<float4x4>", hit );

    const char* path = "$TMP/compactionTest.npy";
    hit->save(path);
    SSys::npdump(path, "np.int32");
    SSys::npdump(path, "np.float32");


    return 0 ; 
}

