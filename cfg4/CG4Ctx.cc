#include <sstream>

#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4Event.hh"

#include "Opticks.hh"
#include "CTrack.hh"
#include "CG4Ctx.hh"

#include "PLOG.hh"


CG4Ctx::CG4Ctx(Opticks* ok)
{
    init();

    _dbgrec = ok->isDbgRec() ;   // machinery debugging 
    _dbgseq = ok->getDbgSeqhis() || ok->getDbgSeqmat() ;  // content debugging 
    _dbgzero = ok->isDbgZero() ; 
}

bool CG4Ctx::is_dbg() const 
{
    return _dbgrec || _dbgseq || _dbgzero ;
}

unsigned CG4Ctx::step_limit() const 
{
    return 1 + 2*( _steps_per_photon > _bounce_max ? _steps_per_photon : _bounce_max ) ;
}


void CG4Ctx::init()
{
    _dbgrec = false ; 
    _dbgseq = false ; 
    _dbgzero = false ; 

    _photons_per_g4event = 0 ; 
    _steps_per_photon = 0 ; 
    _gen = 0 ; 
    _record_max = 0 ;
    _bounce_max = 0 ; 


    _event = NULL ; 
    _event_id = -1 ; 
    _event_total = 0 ; 
    _event_track_count = 0 ; 

    _track = NULL ; 
    _track_id = -1 ; 
    _track_total = 0 ;
    _track_step_count = 0 ;  

    _parent_id = -1 ; 
    _optical = false ; 
    _pdg_encoding = 0 ; 

    _primary_id = -1 ; 
    _photon_id = -1 ; 
    _record_id = -1 ; 
    _reemtrack = false ; 

    _rejoin_count = 0 ; 
    _primarystep_count = 0 ; 
    _stage = CStage::UNKNOWN ; 

    _record_id = -1 ; 
    _debug = false ; 
    _other = false ; 
    _dump = false ; 


    _step = NULL ; 
    _step_id = -1 ;
    _step_total = 0 ; 
 
}

void CG4Ctx::setEvent(const G4Event* event)
{
    _event = const_cast<G4Event*>(event) ; 
    _event_id = event->GetEventID() ;

     // moved from CSteppingAction::setEvent
    _event_total += 1 ; 
    _event_track_count = 0 ; 
}

void CG4Ctx::setTrack(const G4Track* track)
{
    G4ParticleDefinition* particle = track->GetDefinition();

    _track = const_cast<G4Track*>(track) ; 
    _track_id = CTrack::Id(track) ;

     // moved from CSteppingAction::setTrack
    _track_step_count = 0 ; 
    _event_track_count += 1 ; 
    _track_total += 1 ;
    
    _parent_id = CTrack::ParentId(track) ;
    _optical = particle == G4OpticalPhoton::OpticalPhotonDefinition() ;
    _pdg_encoding = particle->GetPDGEncoding();

    if(_optical) setTrackOptical();
}

void CG4Ctx::setStep(const G4Step* step)
{
    _step = const_cast<G4Step*>(step) ; 
    _step_id = CTrack::StepId(_track);
    _step_total += 1 ; 
    _track_step_count += 1 ; 

    if(_step_id == 0)
    {
        const G4StepPoint* pre = _step->GetPreStepPoint() ;
        _step_origin = pre->GetPosition();
    }

    if(_optical) setStepOptical();
}


std::string CG4Ctx::desc_step() const 
{
    G4TrackStatus track_status = _track->GetTrackStatus(); 

    std::stringstream ss ; 
    ss << "CG4Ctx::desc_step" 
       << " step_total " << _step_total
       << " event_id " << _event_id
       << " track_id " << _track_id
       << " track_step_count " << _track_step_count
       << " step_id " << _step_id
       << " trackStatus " << CTrack::TrackStatusString(track_status)
       ;

    return ss.str();
}



void CG4Ctx::setTrackOptical()
{
     LOG(debug) << "CTrackingAction::setTrack setting UseGivenVelocity for optical " ; 

     _track->UseGivenVelocity(true);

     // NB without this BoundaryProcess proposed velocity to get correct GROUPVEL for material after refraction 
     //    are trumpled by G4Track::CalculateVelocity 

     _primary_id = CTrack::PrimaryPhotonID(_track) ;    // layed down in trackinfo by custom Scintillation process
     _photon_id = _primary_id >= 0 ? _primary_id : _track_id ; 
     _reemtrack = _primary_id >= 0 ? true        : false ; 

     // retaining original photon_id from prior to reemission effects the continuation

    _record_id = _photons_per_g4event*_event_id + _photon_id ; 


    // moved from  CSteppingAction::setPhotonId
    // essential for clearing counts otherwise, photon steps never cleared 
    _rejoin_count = 0 ; 
    _primarystep_count = 0 ; 

}

void CG4Ctx::setStepOptical()
{
    if( !_reemtrack )     // primary photon, ie not downstream from reemission 
    {
        _stage = _primarystep_count == 0  ? CStage::START : CStage::COLLECT ;
        _primarystep_count++ ; 
    } 
    else 
    {
        _stage = _rejoin_count == 0  ? CStage::REJOIN : CStage::RECOLL ;   
        _rejoin_count++ ; 
        // rejoin count is zeroed in setTrackOptical, so each remission generation trk will result in REJOIN 
    }
}



std::string CG4Ctx::desc() const 
{
    std::stringstream ss ; 
    ss 
        << ( _dbgrec ? " [--dbgrec] " : "" )
        << ( _dbgseq ? " [--dbgseqmat 0x.../--dbgseqhis 0x...] " : "" )
        << ( _debug ? " --dindex " : "" )
        << ( _other ? " --oindex " : "" )
        << " record_id " << _record_id
        << " event_id " << _event_id
        << " track_id " << _track_id
        << " photon_id " << _photon_id
        << " parent_id " << _parent_id
        << " primary_id " << _primary_id
        << " reemtrack " << _reemtrack
        ;
    return ss.str();
}



