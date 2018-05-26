#pragma once

#include <vector>
#include <string>
#include <map>
#include <cstring>

#include "NPY_API_EXPORT.hh"
#include "NPY_HEAD.hh"

/**

Times
=======

Labeled vector of (string,double) pairs corresponding to 
a table "column".

The string identifies to what the number refers and 
allows matching with quantities in other *Times* instances.

The vector is persisted to json::

    simon:1 blyth$ cat t_absolute.ini 
    _seqhisMakeLookup=5.9437209999996412
    seqhisMakeLookup=5.9510969999992085
    seqhisApplyLookup=5.9511210000000574
    _seqmatMakeLookup=5.9511220000003959
    seqmatMakeLookup=5.9562640000003739
    seqmatApplyLookup=5.9562779999996565
    indexSequenceInterop=5.9864919999999984
    indexBoundaries=6.0250880000003235
    indexPresentationPrep=6.0276389999999083
    _save=6.1372140000003128
    save=6.3327879999997094


**/


class NPY_API Times {
  public:
     typedef std::pair<std::string, double>  SD ; 
     typedef std::vector<SD>                VSD ; 
  public:
     static void compare(const std::vector<Times*>&, unsigned int nwid=25, unsigned int twid=10, unsigned int tprec=3);
     static std::string name(const char* type, const char* tag);
     std::string name();
  public:
     Times(const char* label="nolabel");
     void setLabel(const char* label);
     Times* clone(const char* label);
     void add(const char* name, double t );
     unsigned int getNumEntries();
  public:
     void setScale(double s);
     double getScale(); 
     const char* getLabel();
  public:
     void dump(const char* msg="Times::dump");
     unsigned int getSize();
     std::vector<std::pair<std::string, double> >& getTimes();
     std::pair<std::string, double>&  getEntry(unsigned int i);
  public:
     void save(const char* dir);
     void load(const char* dir);
     void load(const char* dir, const char* name);
  public:
     static Times* load(const char* label, const char* dir, const char* name);
  private:
     VSD         m_times ;  
     double      m_scale ; 
     const char* m_label ; 

};

#include "NPY_TAIL.hh"

