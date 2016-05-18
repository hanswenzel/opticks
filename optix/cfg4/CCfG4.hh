#pragma once

#include <cstring>

class Opticks ; 
template <typename T> class OpticksCfg ;
class GCache ; 
class GBndLib ;
class GGeoTestConfig ; 
class TorchStepNPY ;
class NumpyEvt ; 
class CDetector ; 
class Recorder ; 
class Rec ; 

class G4RunManager ; 
class G4VisManager ; 
class G4UImanager ; 
class G4UIExecutive ; 

class CCfG4 
{
   public:
        CCfG4(int argc, char** argv);
        virtual ~CCfG4();
   private:
        void init(int argc, char** argv);
        void configure(int argc, char** argv);
   public:
        void interactive(int argc, char** argv);
        void propagate();
        void save();
   private:
        Opticks*              m_opticks ;  
        OpticksCfg<Opticks>*  m_cfg ;
        GGeoTestConfig*       m_testconfig ; 
        GCache*               m_cache ; 
        TorchStepNPY*         m_torch ; 
        NumpyEvt*             m_evt ; 
   private:
        CDetector*             m_detector ; 
        Recorder*             m_recorder ; 
        Rec*                  m_rec ; 
        G4RunManager*         m_runManager ;
   private:
        bool                  m_g4ui ; 
        G4VisManager*         m_visManager ; 
        G4UImanager*          m_uiManager ; 
        G4UIExecutive*        m_ui ; 
   private:
        unsigned int          m_num_g4event ; 
        unsigned int          m_num_photons ; 

};

inline CCfG4::CCfG4(int argc, char** argv) 
   :
     m_opticks(NULL),
     m_cfg(NULL),
     m_testconfig(NULL),
     m_cache(NULL),
     m_torch(NULL),
     m_evt(NULL),
     m_detector(NULL),
     m_recorder(NULL),
     m_rec(NULL),
     m_runManager(NULL),
     m_g4ui(false),
     m_visManager(NULL),
     m_uiManager(NULL),
     m_ui(NULL),
     m_num_g4event(0),
     m_num_photons(0)
{
    init(argc, argv);
    configure(argc, argv);
}

