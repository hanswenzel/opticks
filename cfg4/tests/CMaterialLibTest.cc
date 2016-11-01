// op --cmat 
// op --cmat 0
// op --cmat GdDopedLS

#include "Opticks.hh"
#include "OpticksHub.hh"
#include "OpticksMode.hh"
#include "CMaterialLib.hh"
#include "CFG4_BODY.hh"
#include "CFG4_LOG.hh"

#include "PLOG.hh"

int main(int argc, char** argv)
{
    PLOG_(argc, argv);

    CFG4_LOG__ ;  

    LOG(info) << argv[0] ; 

    Opticks ok(argc, argv);
    
    ok.setModeOverride( OpticksMode::CFG4_MODE );  // override COMPUTE/INTEROP mode, as those do not apply to CFG4

    OpticksHub hub(&ok); 

    CMaterialLib* clib = new CMaterialLib(&hub); 

    LOG(info) << argv[0] << " convert " ; 

    clib->convert();

    LOG(info) << argv[0] << " dump " ; 

    clib->dump();

    //clib->dumpMaterials();



    return 0 ; 
}