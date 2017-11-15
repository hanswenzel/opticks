// g4-

#include "CFG4_PUSH.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "CFG4_POP.hh"

// okc-
#include "Opticks.hh"

// cg4-
#include "CG4Ctx.hh"
#include "CG4.hh"
#include "CRecorder.hh"
#include "CTrack.hh"
#include "CSteppingAction.hh"
#include "CTrackingAction.hh"

#include "PLOG.hh"


/**
CTrackingAction
=================

Canonical instance (m_ta) is ctor resident of CG4 

**/

CTrackingAction::CTrackingAction(CG4* g4)
   : 
   G4UserTrackingAction(),
   m_g4(g4),
   m_ctx(g4->getCtx()),
   m_ok(g4->getOpticks()),
   m_recorder(g4->getRecorder()),
   m_sa(g4->getSteppingAction()),
   m_dump(false)
{ 
}

CTrackingAction::~CTrackingAction()
{ 
}

void CTrackingAction::setTrack(const G4Track* track)
{
    m_ctx.setTrack(track);
 
    m_ctx._debug = m_ok->isDbgPhoton(m_ctx._record_id) ; // from option: --dindex=1,100,1000,10000 
    m_ctx._other = m_ok->isOtherPhoton(m_ctx._record_id) ; // from option: --oindex=1,100,1000,10000 

    m_dump = m_ctx._debug || m_ctx._other ; 

    if(m_ctx._optical) m_sa->setPhotonId() ;   // essential for clearing counts otherwise, photon steps never cleared 
}


void CTrackingAction::dump(const char* msg )
{
    LOG(info) << "." ;
    LOG(info) << msg  
              << " ctx " << m_ctx.desc()
               ; 
}

void CTrackingAction::postinitialize()
{
    assert(m_ctx._track_id == -1);
    assert(m_ctx._parent_id == -1);
    LOG(trace) << "CTrackingAction::postinitialize" 
              << brief()
               ;
}

std::string CTrackingAction::brief()
{
    std::stringstream ss ; 
    ss  
       << " track_id " << m_ctx._track_id
       << " parent_id " << m_ctx._parent_id
       ;
    return ss.str();
}

void CTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    setTrack(track);

    LOG(trace) << "CTrackingAction::PreUserTrackingAction"
              << brief()  
               ;
}

void CTrackingAction::PostUserTrackingAction(const G4Track* track)
{
    int track_id = CTrack::Id(track) ;
    assert( track_id == m_ctx._track_id );
    assert( track == m_ctx._track );

    LOG(trace) << "CTrackingAction::PostUserTrackingAction" 
              << brief() 
              ;

    if(m_ctx._optical)
    {
        m_recorder->posttrack();
    } 
}

