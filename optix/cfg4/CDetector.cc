// cfg4-

//
//  ggv-;ggv-pmt-test --cfg4
//  ggv-;ggv-pmt-test --cfg4 --load 1
//


#include "CDetector.hh"
#include <map>

// npy-
#include "NLog.hpp"
#include "GLMFormat.hpp"

// cfg4-
#include "CMaker.hh"
#include "CPropLib.hh"

// ggeo-
#include "GMaker.hh"
#include "GCache.hh"

#include "GCSG.hh"

#include "GMaterial.hh"
#include "GGeoTestConfig.hh"

// g4-
#include "G4RunManager.hh"

#include "G4NistManager.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4UImanager.hh"

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


void CDetector::init()
{
    m_lib = new CPropLib(m_cache);
    m_maker = new CMaker(m_cache);
}

bool CDetector::isPmtInBox()
{
    const char* mode = m_config->getMode();
    return strcmp(mode, "PmtInBox") == 0 ;
}
bool CDetector::isBoxInBox()
{
    const char* mode = m_config->getMode();
    return strcmp(mode, "BoxInBox") == 0 ;
}


G4VPhysicalVolume* CDetector::Construct()
{
   // analagous to ggeo-/GGeoTest::CreateBoxInBox
   // but need to translate from a surface based geometry spec into a volume based one
   //
   // creates Russian doll geometry layer by layer, starting from the outermost 
   // hooking up mother volume to prior 
   //

    bool is_pib = isPmtInBox() ;
    bool is_bib = isBoxInBox() ;

    if(m_verbosity > 0)
    LOG(info) << "CDetector::Construct"
              << " pib " << is_pib
              << " bib " << is_bib
              ;

    assert( is_pib || is_bib && "CDetector::Construct mode not recognized");

    if(m_verbosity > 0)
    m_config->dump("CDetector::Construct");

    unsigned int n = m_config->getNumElements();

    G4VPhysicalVolume* top = NULL ;  
    G4LogicalVolume* mother = NULL ; 

    for(unsigned int i=0 ; i < n ; i++)
    {   
        const char* spec = m_config->getBoundary(i);
        const G4Material* material = m_lib->makeInnerMaterial(spec);

        glm::vec4 param = m_config->getParameters(i);
        char shapecode = m_config->getShape(i) ;
        const char* shapename = GMaker::ShapeName(shapecode);

        std::string lvn = CMaker::LVName(shapename);
        std::string pvn = CMaker::PVName(shapename);

        if(m_verbosity > 0)
        LOG(info) << "CDetector::Construct" 
                  << std::setw(2) << i 
                  << std::setw(2) << shapecode 
                  << std::setw(15) << shapename
                  << std::setw(30) << spec
                  << std::setw(20) << gformat(param)
                  ;   

        G4VSolid* solid = m_maker->makeSolid(shapecode, param);  

        G4LogicalVolume* lv = new G4LogicalVolume(solid, const_cast<G4Material*>(material), lvn.c_str(), 0,0,0);

        G4VPhysicalVolume* pv = new G4PVPlacement(0,G4ThreeVector(), lv, pvn.c_str(),mother,false,0);
        
        m_pvm[pvn] = pv ;  
 
        if(top == NULL)
        {
            top = pv ; 
            setCenterExtent(param.x, param.y, param.z, param.w );
        }
        mother = lv ; 
    }   

    if(is_pib)
    {
        makePMT(mother);
    }

    m_lib->dumpMaterials("CDetector::Construct CPropLib::dumpMaterials");
    return top ;  
}


void CDetector::makePMT(G4LogicalVolume* container)
{
    // try without creating an explicit node tree 

    NSlice* slice = m_config->getSlice();

    GCSG* csg = m_lib->getPmtCSG(slice);
    if(m_verbosity > 1)
    csg->dump();

    unsigned int ni = csg->getNumItems();

    if(m_verbosity > 0)
    LOG(info) << "CDetector::makePMT" 
              << " csg items " << ni 
              ; 

    G4LogicalVolume* mother = container ; 

    std::map<unsigned int, G4LogicalVolume*> lvm ; 

    for(unsigned int index=0 ; index < ni ; index++)
    {
        unsigned int nix = csg->getNodeIndex(index); 
        if(nix == 0) continue ;
        // skip non-lv with nix:0, as those are constituents of the lv that get recursed over

        unsigned int pix = csg->getParentIndex(index); 
        const char* pvn = csg->getPVName(nix-1);

        if(m_verbosity > 0)
        LOG(info) << "CDetector::makePMT" 
                  << " csg items " << ni 
                  << " index " << std::setw(3) << index 
                  << " nix " << std::setw(3) << nix 
                  << " pix " << std::setw(3) << pix 
                  << " pvn " << pvn 
                  ; 

        G4LogicalVolume* logvol = makeLV(csg, index );

        lvm[nix-1] = logvol ;

        G4LogicalVolume* mother = NULL ; 

        if(nix - 1 == 0)
        { 
            mother = container ;
        }
        else
        {
            assert( pix > 0 && lvm.count(pix-1) == 1  );
            mother = lvm[pix-1] ;
        }
              
        G4RotationMatrix* rot = 0 ; 
        G4ThreeVector tlate(csg->getX(index), csg->getY(index), csg->getZ(index));  

        // pv translation, for DYB PMT only non-zero for pvPmtHemiBottom, pvPmtHemiDynode
        // suspect that G4DAE COLLADA export omits/messes this up somehow, for the Bottom at least

        if(m_verbosity > 0)
        LOG(info) << "CDetector::makePMT"
                  << " index " << index 
                  << " x " << tlate.x()
                  << " y " << tlate.y()
                  << " z " << tlate.z()
                  ;

        G4bool many = false ; 
        G4int copyNo = 0 ; 

        G4VPhysicalVolume* physvol = new G4PVPlacement(rot, tlate, logvol, pvn, mother, many, copyNo);

        m_pvm[pvn] = physvol ;  

    }

    kludgePhotoCathode();
}


G4VPhysicalVolume* CDetector::getPV(const char* name)
{
    return m_pvm.count(name) == 1 ? m_pvm[name] : NULL ; 
}

void CDetector::kludgePhotoCathode()
{
    LOG(info) << "CDetector::kludgePhotoCathode" ;

    float effi = 0.f ; 
    float refl = 0.f ; 
    {
        const char* name = "kludgePhotoCathode_PyrexBialkali" ; 
        G4VPhysicalVolume* pv1 = getPV("pvPmtHemi") ; 
        G4VPhysicalVolume* pv2 = getPV("pvPmtHemiCathode") ; 
        assert(pv1 && pv2);
        //G4LogicalBorderSurface* lbs = m_lib->makeCathodeSurface(name, pv1, pv2, effi, refl );
        G4LogicalBorderSurface* lbs = m_lib->makeCathodeSurface(name, pv1, pv2);
    }
    {
        const char* name = "kludgePhotoCathode_PyrexVacuum" ; 
        G4VPhysicalVolume* pv1 = getPV("pvPmtHemi") ; 
        G4VPhysicalVolume* pv2 = getPV("pvPmtHemiVacuum") ; 
        assert(pv1 && pv2);
        G4LogicalBorderSurface* lbs = m_lib->makeConstantSurface(name, pv1, pv2, effi, refl );
    }
}


G4LogicalVolume* CDetector::makeLV(GCSG* csg, unsigned int i)
{
    unsigned int ix = csg->getNodeIndex(i); 

    assert(ix > 0);

    const char* matname = csg->getMaterialName(ix - 1) ;

    const char* lvn = csg->getLVName(ix - 1)  ;  

    const G4Material* material = m_lib->makeMaterial(matname) ;

    G4VSolid* solid = m_maker->makeSolid(csg, i );

    G4LogicalVolume* logvol = new G4LogicalVolume(solid, const_cast<G4Material*>(material), lvn);

    if(m_verbosity > 0)
    LOG(info) 
           << "CDetector::makeLV "
           << "  i " << std::setw(2) << i  
           << " ix " << std::setw(2) << ix  
           << " lvn " << std::setw(2) << lvn
           << " matname " << matname  
           ;

    return logvol ; 
}


void CDetector::dumpPV(const char* msg)
{
    LOG(info) << msg ; 

    typedef std::map<std::string, G4VPhysicalVolume*> MSV ; 

    for(MSV::const_iterator it=m_pvm.begin() ; it != m_pvm.end() ; it++)
    {
         std::string pvn = it->first ; 
         G4VPhysicalVolume* pv = it->second ;  

         std::cout << std::setw(40) << pvn 
                   << std::setw(40) << pv->GetName() 
                   << std::endl 
                   ;

    }
}




