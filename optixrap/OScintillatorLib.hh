#pragma once

/**
OScintillatorLib
===================

Used to upload GScintillatorLib reemission data
into GPU reemission texture.  


**/


#include "OXPPNS.hh"
#include "plog/Severity.h"
#include <optixu/optixu_math_namespace.h>

class GScintillatorLib ;
template <typename T> class NPY ;

#include "OPropertyLib.hh"
#include "OXRAP_API_EXPORT.hh"

class OXRAP_API OScintillatorLib : public OPropertyLib {
    public:
        static const plog::Severity LEVEL ; 
    public:
        OScintillatorLib(optix::Context& ctx, GScintillatorLib* lib);
    public:
        void convert(const char* slice=NULL);
    private:
        void makeReemissionTexture(NPY<float>* buf);
    private:
        GScintillatorLib*    m_lib ;
        NPY<float>*          m_placeholder ; 
};






