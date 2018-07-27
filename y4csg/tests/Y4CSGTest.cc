#include "OPTICKS_LOG.hh"
#include "Y4CSG.hh"
#include "X4Solid.hh"
#include "NCSG.hpp"


// start of portion to be generated ----------------
#include "G4Orb.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4UnionSolid.hh"

G4VSolid* make_solid()
{
    G4VSolid* b = new G4Orb("orb1",1) ;
    G4VSolid* d = new G4Orb("orb2",2) ;
    G4RotationMatrix* A = new G4RotationMatrix(G4ThreeVector(0.707107,-0.707107,0.000000),G4ThreeVector(0.707107,0.707107,0.000000),G4ThreeVector(0.000000,0.000000,1.000000));
    G4ThreeVector B(1.000000,0.000000,0.000000);
    G4VSolid* a = new G4UnionSolid("uni1",b , d , A , B) ;
    return a ; 
}
// end of portion to be generated ---------------------



int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv);

    Y4CSG yc ;  

    G4VSolid* so = make_solid() ; 
    std::cout << *so << std::endl ; 


    nnode* root = X4Solid::Convert(so) ; 
    assert( root ) ; 

    unsigned soIdx = 0 ; 
    unsigned lvIdx = 0 ;

    NCSG* csg = NCSG::FromNode( root, NULL, soIdx, lvIdx );
    assert( csg ) ;  




    return 0 ; 
}

