#pragma once

#include <set>
#include <string>
#include <vector>

struct guint4 ; 
template <typename T> class GPropertyMap ; 

#include "GGEO_API_EXPORT.hh"


/**
GSur
=====

A single GSur can correspond to 
multiple G4LogicalBorderSurface each 
with different volume pairs OR 
multiple G4LogicalSkinSurface each with 
different logical volumes.

Hmm seems very overcomplicated...


**/

class GGEO_API GSur 
{
         friend class GSurLib ; 
    public:
         GSur(GPropertyMap<float>* pm, char type);

         GPropertyMap<float>* getPMap();
         const char* getName();

         std::string brief();
         std::string pvpBrief();
         std::string lvBrief();
         std::string check();
         void dump(const char* msg="GSur::dump");

         char getType();

         bool isBorder();
         bool isSkin();
         bool isUnused();

         void setBorder();
         void setSkin();
         void setUnused();

         void assignType();  // sets Unused 
    private:
         void setType(char type);
    public:
         // BorderSurfaces
         unsigned getNumVolumePair();
         guint4 getVolumePair(unsigned index );
    private:
         void addVolumePair(unsigned pv1, unsigned pv2);
    public:
         // SkinSurfaces
         unsigned getNumLV();
         const char* getLV(unsigned index);
    private:
         void addLV(const char* lv);

    private:
         // adders used first, in analysis stage 
         void addInner(unsigned vol, unsigned bnd);
         void addOuter(unsigned vol, unsigned bnd);
         const std::set<unsigned>& getIBnd();
         const std::set<unsigned>& getOBnd();

    private:
         GPropertyMap<float>*  m_pmap  ;
         char                  m_type ; 
    private:
         // vector of node indices which use the surface property map as inner/outer
         std::vector<unsigned> m_ivol ; 
         std::vector<unsigned> m_ovol ; 

         // set of unique boundary indices which use the surface property map as inner/outer
         std::set<unsigned> m_ibnd ;  // 
         std::set<unsigned> m_obnd ;  //  

         std::set<std::pair<unsigned,unsigned> >       m_pvp ; 
         std::set<std::string>                         m_slv ; 

};




