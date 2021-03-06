/*
 * Copyright (c) 2019 Opticks Team. All Rights Reserved.
 *
 * This file is part of Opticks
 * (see https://bitbucket.org/simoncblyth/opticks).
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */

#pragma once

#include "NNode.hpp"
#include "NQuad.hpp"
#include "NGLM.hpp"

struct nplane ; 
struct ndisk ; 
struct npart ;
struct nbbox ; 
struct nuv ; 

#include "NPY_API_EXPORT.hh"

struct NPY_API nsphere : nnode {

    float costheta(float z);

    float operator()(float x, float y, float z) const ;

    void resizeToFit(const nbbox& container_bb, float scale, float delta);


    nbbox bbox() const ;

    npart part() const ;
    static ndisk* intersect(const nsphere* a, const nsphere* b);

    // result of intersect allows partitioning 
    npart zrhs(const ndisk* dsc); // +z to the right  
    npart zlhs(const ndisk* dsc);  

    glm::vec3 gseedcenter() const ;

    unsigned  par_nsurf() const ; 
    glm::vec3 par_pos_model(const nuv& uv) const  ;
    void par_posnrm_model( glm::vec3& pos, glm::vec3& nrm, unsigned s, float fu, float fv ) const  ;


    int       par_euler() const ; 
    unsigned  par_nvertices(unsigned nu, unsigned nv) const ; 

    static void _par_pos_body(glm::vec3& pos,  const nuv& uv, const float r_ ) ;

    void pdump(const char* msg="nsphere::pdump") const ;
 
    float     x() const  ; 
    float     y() const  ; 
    float     z() const  ; 
    float     radius() const  ; 
    glm::vec3 center() const  ; 

};

inline NPY_API float nsphere::x() const { return param.f.x ; }
inline NPY_API float nsphere::y() const { return param.f.y ; }
inline NPY_API float nsphere::z() const { return param.f.z ; }
inline NPY_API float nsphere::radius() const { return param.f.w ; }
inline NPY_API glm::vec3 nsphere::center() const { return glm::vec3(x(),y(),z())  ; }



inline NPY_API void init_sphere(nsphere* s, const nquad& param)
{
    s->param = param ; 
}
inline NPY_API nsphere* make_sphere(const nquad& param)
{
    nsphere* n = new nsphere ; 
    nnode::Init(n,CSG_SPHERE) ; 
    init_sphere(n, param);
    return n ; 
}
inline NPY_API nsphere* make_sphere(float x, float y, float z, float w)
{
    nquad param ; 
    param.f = {x,y,z,w} ;
    return make_sphere(param);
}
inline NPY_API nsphere* make_sphere(float radius=100.f)
{
    return make_sphere(0.f,0.f,0.f,radius);
}


