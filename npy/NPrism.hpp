#pragma once

#include "NQuad.hpp"

struct npart ;
struct nbbox ; 


//TODO: change into nnode substruct 

struct NPY_API nprism 
{
    nprism(float apex_angle_degrees=90.f, float height_mm=100.f, float depth_mm=100.f, float fallback_mm=100.f);
    nprism(const nquad& param_);

    nbbox bbox();

    float height();
    float depth();
    float hwidth();

    npart part();
    void dump(const char* msg);

    nquad param ; 
};


