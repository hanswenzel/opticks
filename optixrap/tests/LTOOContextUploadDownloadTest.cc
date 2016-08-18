#include "Opticks.hh"

#include "OptiXTest.hh"
#include "OContext.hh"

#include "NPY.hpp"

#include "OBuf.hh"

#include "BOpticksEvent.hh"
#include "BOpticksResource.hh"

#include "OpticksEvent.hh"
#include "OpticksBufferControl.hh"
#include "BOpticksResource.hh"

#include "OXRAP_LOG.hh"
#include "PLOG.hh"


int main( int argc, char** argv ) 
{
    PLOG_(argc, argv);
    OXRAP_LOG__ ; 

    Opticks* m_opticks = new Opticks(argc, argv);
    m_opticks->configure();

    optix::Context context = optix::Context::create();

    OContext::Mode_t mode = OContext::COMPUTE ;

    OContext* m_ocontext = new OContext(context, mode, false );

    unsigned entry = m_ocontext->addEntry("LTminimalTest.cu.ptx", "minimal", "exception");

    // LT: Try to load some events
    NPY<float>* npy = NULL;
    
    const char* gensteps_dir = BOpticksResource::GenstepsDir();
    BOpticksEvent::SetOverrideEventBase(gensteps_dir);
    std::string path = BOpticksEvent::path("juno","cerenkov","1","",".npy"); 
    BOpticksEvent::SetOverrideEventBase(NULL);

    npy = NPY<float>::load(path.c_str()) ;
    npy->dump("NPY::dump::before", 2);
    npy->setBufferControl(OpticksBufferControl::Parse("OPTIX_INPUT_OUTPUT,OPTIX_SETSIZE"));

    optix::Buffer buffer = m_ocontext->createBuffer<float>( npy, "demo");
    context["output_buffer"]->set(buffer);
    OBuf* genstep_buf = new OBuf("genstep", buffer, npy);

    OContext::upload(buffer, npy);

    genstep_buf->dump<unsigned int>("LT::OBuf test: ", 6*4, 3, 6*4*10);
    LOG(info) << "check OBuf begin.";
    // LT: check OBuf
    npy->zero();
    genstep_buf->download(npy);
    npy->dump("NPY::dump::after", 2);
    LOG(info) << "check OBuf end.";

    unsigned ni = 10 ; 
    m_ocontext->launch( OContext::VALIDATE,  entry, ni, 1);
    genstep_buf->dump<unsigned int>("LT::OBuf test after VALIDATE: ", 6*4, 3, 6*4*10);
    m_ocontext->launch( OContext::COMPILE,   entry, ni, 1);
    genstep_buf->dump<unsigned int>("LT::OBuf test after COMPILE: ", 6*4, 3, 6*4*10);
    m_ocontext->launch( OContext::PRELAUNCH, entry, ni, 1);
    genstep_buf->dump<unsigned int>("LT::OBuf test after PRELAUNCH: ", 6*4, 3, 6*4*10);
    m_ocontext->launch( OContext::LAUNCH,    entry, ni, 1);
    genstep_buf->dump<unsigned int>("LT::OBuf test after LAUNCH: ", 6*4, 3, 6*4*10);

    npy->zero();

    OContext::download( buffer, npy );

    NPYBase::setGlobalVerbose();

    // npy->dump();
    npy->save("$TMP/OOContextUploadDownloadTest_1.npy");

    return 0;
}