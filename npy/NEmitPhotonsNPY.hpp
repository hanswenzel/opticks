#pragma once

template <typename T> class NPY ; 

#include "NPY_API_EXPORT.hh"

class NCSG ; 
class NRngDiffuse ; 

class FabStepNPY ; 

struct nnode ; 
struct NEmitConfig ; 

/**
NEmitPhotonsNPY
=================

Prepares input photon buffer for an NCSG instance.

Canonical m_emitter instance is ctor resident of OpticksGen
which in turn is ctor resident of OpticksHub.

TODO:

* dependency sink gencode (OpticksPhoton.h) 
  to avoid the gencode EMITSOURCE having to be passed 
  down from higher levels (perhaps a lowest level OpticksBase package ?)

**/

class NPY_API NEmitPhotonsNPY 
{
   public:
      NEmitPhotonsNPY(NCSG* csg, unsigned gencode, unsigned seed, bool emitdbg, NPY<unsigned>* mask);

      NPY<float>* getPhotons() const ;
      NPY<float>* getPhotonsRaw() const  ;

      FabStepNPY* getFabStep() const ;
      FabStepNPY* getFabStepRaw() const ;

      NPY<float>* getFabStepData() const ;
      NPY<float>* getFabStepRawData() const ;

   public:
      std::string desc() const  ;
   private:
      void init(); 
   private:
      NCSG*          m_csg ; 
      unsigned       m_gencode ; 
      unsigned       m_seed ; 
      bool           m_emitdbg ; 
      NPY<unsigned>* m_mask ; 
   private:
      int            m_emit ; 
      const char*    m_emitcfg_ ;
      NEmitConfig*   m_emitcfg  ;       
      nnode*         m_root ; 
   private:
      // products 
      NPY<float>*   m_photons ; 
      NPY<float>*   m_photons_masked ; 

      FabStepNPY*   m_fabstep ; 
      FabStepNPY*   m_fabstep_masked ;
 
      NRngDiffuse*  m_diffuse ; 

};


