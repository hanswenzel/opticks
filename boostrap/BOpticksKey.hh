#pragma once

#include <string>
#include "BRAP_API_EXPORT.hh"

/** 
BOpticksKey
============

Used to communicate the geometry identity digest
to be used by Opticks instanciation (especually OpticksResource)
without changing the Opticks interface.

This is for example used with direct G4 to Opticks running, 
where the geometry  


**/

class BRAP_API BOpticksKey 
{
    public:
        static const char* G4LIVE ; 
        static const char* IDFILE ; 
        static int         LAYOUT ; 
        static BOpticksKey* GetKey();
        static bool         SetKey(const char* spec) ;  
    public:
        const char* getSpec() const ; 
        const char* getExename() const ; 
        const char* getClass() const ; 
        const char* getVolname() const ; 
        const char* getDigest() const ; 
    public:
        const char* getIdname() const ; 
        const char* getIdfile() const ; 
        int         getLayout() const ; 
    public:
        std::string desc() const ; 
    private:
        BOpticksKey(const char* spec); 
    private:
        const char* m_spec   ; 
        const char* m_exename ; 
        const char* m_class   ; 
        const char* m_volname   ; 
        const char* m_digest ;

        const char* m_idname ; // eg OpNovice_World_g4live
        const char* m_idfile ; // eg g4ok.gltf
        int         m_layout ; 
 
        static BOpticksKey* fKey ; 

};