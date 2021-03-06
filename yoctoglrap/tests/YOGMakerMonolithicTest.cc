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

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "OPTICKS_LOG.hh"
#include "BStr.hh"
#include "BFile.hh"
#include "NPY.hpp"

#include "YGLTF.h"

#include "YOG.hh"
#include "YOGMaker.hh"
#include "YOGMakerImpl.hh"
#include "YOGGeometry.hh"


using ygltf::glTF_t ; 
using ygltf::node_t ; 
using ygltf::scene_t ; 
using ygltf::mesh_t ; 
using ygltf::material_t ; 
using ygltf::material_pbrMetallicRoughness_t ; 
using ygltf::mesh_primitive_t ; 
using ygltf::buffer_t ; 
using ygltf::buffer_data_t ; 
using ygltf::bufferView_t ; 
using ygltf::accessor_t ; 

using YOG::Geometry ; 
using YOG::Sc ; 
using YOG::Maker ; 


const char* TMPDIR = "$TMP/yoctoglrap/YOGMakerMonolithicTest" ; 

namespace YOG {

void demo_create_monolithic( Maker& mk, const Geometry& geom )
{

    // Maker methods avoid having to do things in this monlithic manner, see demo_create  
    assert( geom.vtx ) ; 
    assert( geom.idx ) ; 

    NBufferSpec vtx_spec = geom.vtx->getBufferSpec();
    NBufferSpec idx_spec = geom.idx->getBufferSpec();


    ygltf::glTF_t* gltf = mk.impl->gltf ; 

    std::vector<scene_t>& scenes = gltf->scenes ; 
    std::vector<node_t>& nodes = gltf->nodes ; 
    std::vector<buffer_t>& buffers = gltf->buffers ; 
    std::vector<bufferView_t>& bufferViews = gltf->bufferViews ; 
    std::vector<accessor_t>& accessors = gltf->accessors ; 
    std::vector<mesh_t>& meshes = gltf->meshes ; 
    std::vector<material_t>& materials = gltf->materials ; 

    node_t no0 ; 
    no0.mesh = 0 ; 

    node_t no1 ; 
    no1.mesh = 0 ; 
    no1.translation = {{1,0,0}} ; 

    nodes = {no0, no1} ; 
    int no0_ = 0 ; 
    int no1_ = 1 ; 

    scene_t sc ;   
    sc.nodes = {no0_, no1_} ;   
    scenes = {sc} ;  

    material_pbrMetallicRoughness_t mr ; 
    mr.baseColorFactor =  {{ 1.000, 0.766, 0.336, 1.0 }} ;
    mr.metallicFactor = 0.5 ;
    mr.roughnessFactor = 0.1 ;

    material_t mt ; 
    mt.pbrMetallicRoughness = mr ; 

    materials = { mt } ; 
    int mt_ = 0 ; 

    buffer_t vtx_bu ; 
    vtx_bu.uri = "vtx.npy" ; 
    vtx_bu.byteLength = vtx_spec.bufferByteLength ; 
    geom.vtx->saveToBuffer( vtx_bu.data ) ; 

    buffer_t idx_bu ; 
    idx_bu.uri = "idx.npy" ; 
    idx_bu.byteLength = idx_spec.bufferByteLength ; 
    geom.idx->saveToBuffer( idx_bu.data ) ; 

    buffers = {vtx_bu, idx_bu} ; 
    int vtx_bu_ = 0 ; 
    int idx_bu_ = 1 ; 
    int count = geom.count ;  

    bufferView_t vtx_bv ;  
    vtx_bv.buffer = vtx_bu_  ;
    vtx_bv.byteOffset = vtx_spec.headerByteLength ; // offset to skip the NPY header 
    vtx_bv.byteLength = vtx_spec.dataSize() ;
    vtx_bv.target = bufferView_t::target_t::array_buffer_t ;
    vtx_bv.byteStride = 0 ; 
 
    bufferView_t idx_bv ;  
    idx_bv.buffer = idx_bu_  ;
    idx_bv.byteOffset = idx_spec.headerByteLength ; // offset to skip the NPY header
    idx_bv.byteLength = sizeof(unsigned)*count ;  // geom.idx_spec->dataSize() ;
    idx_bv.target = bufferView_t::target_t::element_array_buffer_t ;
    idx_bv.byteStride = 0 ; 
 
    bufferViews = {vtx_bv, idx_bv} ;  
    int vtx_bv_ = 0 ; 
    int idx_bv_ = 1 ; 

    accessor_t vtx_ac ; 
    vtx_ac.bufferView = vtx_bv_ ; 
    vtx_ac.byteOffset = 0 ;
    vtx_ac.componentType = accessor_t::componentType_t::float_t ; 
    vtx_ac.count = count ; 
    vtx_ac.type = accessor_t::type_t::vec4_t ; 
    vtx_ac.min = geom.vtx_minf ; 
    vtx_ac.max = geom.vtx_maxf ; 

    accessor_t idx_ac ; 
    idx_ac.bufferView = idx_bv_ ; 
    idx_ac.byteOffset = 0 ;
    idx_ac.componentType = accessor_t::componentType_t::unsigned_int_t ; 
    idx_ac.count = count ; 
    idx_ac.type = accessor_t::type_t::scalar_t ; 
    // hmm a deficiency with ygltf model, always expects vector<float> no matter the componentType
    idx_ac.min = geom.idx_minf ; 
    idx_ac.max = geom.idx_maxf ; 

    accessors = {vtx_ac, idx_ac} ;
    int vtx_ac_ = 0 ; 
    int idx_ac_ = 1 ; 
 
    mesh_primitive_t mp ; 
    mp.attributes = {{"POSITION", vtx_ac_ }} ; 
    mp.indices = idx_ac_ ; 
    mp.material = mt_ ; 
    mp.mode = mesh_primitive_t::mode_t::triangles_t ; 
 
    mesh_t mh ; 
    mh.primitives = { mp } ; 

    meshes = {mh} ; 
}

} // namespace




int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv); 

    Geometry geom(3) ; 
    geom.make_triangle();

    Sc* sc = new Sc(0) ;  
    Maker ym(sc) ; 

    demo_create_monolithic(ym, geom);

    std::string path = BFile::FormPath(TMPDIR, "YOGMakerMonolithicTest.gltf");

    ym.convert();

    ym.save(path.c_str());

    return 0 ; 
}

