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

#include <csignal>
#include <vector>
#include <climits>
#include <iostream>
#include <iomanip>

#include "BFile.hh"
#include "BTimeKeeper.hh"

// npy-
#include "NSensor.hpp"
#include "NPY.hpp"
#include "NSlice.hpp"
#include "GLMFormat.hpp"

// opticks-
#include "Opticks.hh"
#include "OpticksConst.hh"
#include "OpticksResource.hh"


#include "GGeo.hh"
#include "GVolume.hh"

#include "GParts.hh"
#include "GPt.hh"
#include "GPts.hh"

#include "GTree.hh"
#include "GMergedMesh.hh"
#include "GBBoxMesh.hh"

#include "PLOG.hh"

const plog::Severity GMergedMesh::LEVEL = PLOG::EnvLevel("GMergedMesh", "DEBUG") ; 

bool GMergedMesh::isSkip() const
{
   return m_geocode == OpticksConst::GEOCODE_SKIP ;  
}
bool GMergedMesh::isAnalytic() const 
{
   return m_geocode == OpticksConst::GEOCODE_ANALYTIC ;  
}
bool GMergedMesh::isTriangulated() const 
{
   return m_geocode == OpticksConst::GEOCODE_TRIANGULATED ;  
}


// expedient pass-thru ctor
GMergedMesh::GMergedMesh(
    unsigned index, 
    gfloat3* vertices, 
    unsigned num_vertices, 
    guint3*  faces, 
    unsigned num_faces, 
    gfloat3* normals, 
    gfloat2* texcoords
    )
    : 
    GMesh(index, vertices, num_vertices, faces, num_faces, normals, texcoords),
    m_cur_vertices(0),
    m_cur_faces(0),
    m_cur_volume(0),
    m_cur_mergedmesh(0),
    m_num_csgskip(0),
    m_cur_base(NULL),
    m_pts(GPts::Make()),
    m_ok(Opticks::Instance())
{
    init();
} 


GMergedMesh::GMergedMesh(unsigned index)
    : 
    GMesh(index, NULL, 0, NULL, 0, NULL, NULL),
    m_cur_vertices(0),
    m_cur_faces(0),
    m_cur_volume(0),
    m_cur_mergedmesh(0),
    m_num_csgskip(0),
    m_cur_base(NULL),
    m_pts(GPts::Make()),
    m_ok(Opticks::Instance())
{
    init();
} 

void GMergedMesh::init()
{
    assert(m_ok) ; 
}


std::string GMergedMesh::brief() const 
{
    std::stringstream ss ; 

    ss << "GMergedMesh::brief"
       << " index " << std::setw(6) << getIndex()
       << " num_csgskip " << std::setw(4) << m_num_csgskip
       << " isSkip " << std::setw(1) << isSkip()
       << " isAnalytic " << std::setw(1) << isAnalytic()
       << " isTriangulated " << std::setw(1) << isTriangulated()
       << " numVertices " << std::setw(7) << getNumVertices()
       << " numFaces " << std::setw(7) << getNumFaces()
       << " numVolumes " << std::setw(5) << getNumVolumes()
       << " numVolumesSelected " << std::setw(5) << getNumVolumesSelected()
  //     << " numComponents " << std::setw(5) << getNumComponents()
       ;

    return ss.str();
}



void GMergedMesh::setCurrentBase(GNode* base)
{
    m_cur_base = base ; 
}
GNode* GMergedMesh::getCurrentBase()
{
    return m_cur_base ; 
}


bool GMergedMesh::isGlobal()
{
    return m_cur_base == NULL ; 
}
bool GMergedMesh::isInstanced()
{
    return m_cur_base != NULL ; 
}



GMergedMesh* GMergedMesh::Combine(unsigned index, GMergedMesh* mm, GVolume* volume, unsigned verbosity ) // static
{
    std::raise(SIGINT); 
    std::vector<GVolume*> volumes ; 
    volumes.push_back(volume);
    return Combine(index, mm, volumes, verbosity );
}


/**
GMergedMesh::Combine
----------------------

Three stages:

1. count
2. allocate
3. merge

Seems to only be invoked from GGeoTest running, specifically GGeoTest::combineVolumes / GGeoTest::initCreateCSG

**/

GMergedMesh* GMergedMesh::Combine(unsigned index, GMergedMesh* mm, const std::vector<GVolume*>& volumes, unsigned verbosity ) // static
{
    //std::raise(SIGINT);   
    unsigned numVolumes = volumes.size(); 
    LOG(info)
        << " making new mesh "
        << " index " << index 
        << " volumes " << numVolumes
        << " verbosity " << verbosity 
        ; 

    if(verbosity > 1)
    GVolume::Dump(volumes, "GMergedMesh::combine (source volumes)");


    GMergedMesh* com = new GMergedMesh( index ); 
    com->setVerbosity(mm ? mm->getVerbosity() : 0 );

    if(mm) com->countMergedMesh(mm, true);
    for(unsigned i=0 ; i < numVolumes ; i++) com->countVolume(volumes[i], true, verbosity ) ;

    com->allocate(); 
 
    if(mm) com->mergeMergedMesh(mm, true, verbosity );
    for(unsigned i=0 ; i < numVolumes ; i++) com->mergeVolume(volumes[i], true, verbosity ) ;

    com->updateBounds();

    if(verbosity > 1)
    com->dumpVolumes("GMergedMesh::combine (combined result) ");
   
    return com ; 
}



/**
GMergedMesh::Create
---------------------

For instanced meshes the base is set to the first occurence of the 
instance eg invoked from GScene::makeMergedMeshAndInstancedBuffers

The base is used as the frame into which the subframes are transformed, when
base is NULL global transforms and not base relative transforms are used.

For vertices to be merged into the GMergedMesh they must pass a selection
based upon the repeatIdx and isSelected status of the GVolume(GNode).

The GNode::setSelection is invoked from::

    AssimpGGeo::convertStructure
    GScene::transferMetadata    

**/

GMergedMesh* GMergedMesh::Create(unsigned ridx, GNode* base, GNode* root, unsigned verbosity ) // static
{
    assert(root && "root node is required");

    LOG(LEVEL) 
        << " ridx " << ridx 
        << " base " << base
        << " root " << root
        << " verbosity " << verbosity
        ;


    OKI_PROFILE("_GMergedMesh::Create"); 

    GMergedMesh* mm = new GMergedMesh( ridx ); 
    mm->setCurrentBase(base);  // <-- when NULL it means will use global not base relative transforms

    GNode* start = base ? base : root ; 

    if(verbosity > 1)
    LOG(info)
        << " ridx " << ridx 
        << " starting from " << start->getName() ;
        ; 

    mm->traverse_r( start, 0, PASS_COUNT, verbosity  );  // 1st pass traversal : counts vertices and faces

    OKI_PROFILE("GMergedMesh::Create::Count"); 

    unsigned nvs = mm->getNumVolumesSelected() ; 
    if( nvs == 0 )
    {
        LOG(fatal)
           << "THERE ARE NO SELECTED VOLUMES FOR ridx " << ridx
           <<  mm->brief()
           ;
    }

    if(verbosity > 1) LOG(info) << mm->brief() ; 

    OKI_PROFILE("_GMergedMesh::Create::Allocate"); 

    mm->allocate();  // allocate space for flattened arrays

    OKI_PROFILE("GMergedMesh::Create::Allocate"); 

    mm->traverse_r( start, 0, PASS_MERGE, verbosity );  // 2nd pass traversal : merge copy GMesh into GMergedMesh 

    OKI_PROFILE("GMergedMesh::Create::Merge"); 

    mm->updateBounds();

    OKI_PROFILE("GMergedMesh::Create::Bounds"); 

    //mm->postcreate(); 

    return mm ;
}



GPts* GMergedMesh::getPts() const { return m_pts ; } 
void  GMergedMesh::setPts(GPts* pts) { m_pts = pts ; }

void GMergedMesh::postcreate()
{
    LOG(info) << "[ " << getIndex() ; 
    m_pts->dump(); 
    LOG(info) << "] " << getIndex() ; 
}


/**
GMergedMesh::traverse_r
--------------------------

Pre-order traversal of the node tree, so root is index zero

**/

void GMergedMesh::traverse_r( GNode* node, unsigned depth, unsigned pass, unsigned verbosity )
{
    GVolume* volume = dynamic_cast<GVolume*>(node) ;

    int idx = getIndex() ;
    assert(idx > -1 ) ; 

    unsigned uidx = idx > -1 ? idx : UINT_MAX ; 
    unsigned ridx = volume->getRepeatIndex() ;

    bool repsel =  idx == -1 || ridx == uidx ;   // RepeatIndex of volume same as index of mm 
    bool csgskip = volume->isCSGSkip() ;         // --csgskiplv
    bool selected_ =  volume->isSelected() && repsel ;
    bool selected = selected_ && !csgskip ;      // selection honoured by both triangulated and analytic 


    if(pass == PASS_COUNT)
    {
        if(selected_ && csgskip) m_num_csgskip++ ; 
    }
    
    if(verbosity > 1)
        LOG(info)
            << " verbosity " << verbosity
            << " node " << node 
            << " volume " << volume 
            << " volume.pts " << volume->getParts()
            << " depth " << depth 
            << " NumChildren " << node->getNumChildren()
            << " pass " << pass
            << " selected " << selected
            << " csgskip " << csgskip
            ; 


    switch(pass)
    {
        case PASS_COUNT:    countVolume(volume, selected, verbosity)  ;break;
        case PASS_MERGE:    mergeVolume(volume, selected, verbosity)  ;break;
                default:    assert(0)                                 ;break;
    }

    for(unsigned i = 0; i < node->getNumChildren(); i++) traverse_r(node->getChild(i), depth + 1, pass, verbosity );
}


void GMergedMesh::countVolume( GVolume* volume, bool selected, unsigned verbosity )
{
    const GMesh* mesh = volume->getMesh();
    m_num_volumes += 1 ; 

    if(selected)
    {
        m_num_volumes_selected += 1 ;
        countMesh( mesh ); 
    }

    //if(m_verbosity > 1)
    LOG(debug) 
        << " verbosity " << verbosity 
        << " selected " << selected
        << " num_volumes " << m_num_volumes 
        << " num_volumes_selected " << m_num_volumes_selected 
        ;
}

void GMergedMesh::countMesh( const GMesh* mesh )
{
    unsigned nface = mesh->getNumFaces();
    unsigned nvert = mesh->getNumVertices();
    unsigned meshIndex = mesh->getIndex();

    m_num_vertices += nvert ;
    m_num_faces    += nface ; 
    m_mesh_usage[meshIndex] += 1 ;  // which meshes contribute to the mergedmesh
}

/**
GMergedMesh::countMergedMesh
---------------------------------

NB what is appropriate for a merged mesh is not for a mesh ... wrt counting volumes
so cannot lump the below together using GMesh base class


**/

void GMergedMesh::countMergedMesh( GMergedMesh*  other, bool selected)
{
    unsigned nvolume = other->getNumVolumes();

    m_num_mergedmesh += 1 ; 

    m_num_volumes += nvolume ;

    if(selected)
    {
        m_num_volumes_selected += 1 ;
        countMesh( other );     // increment m_num_vertices, m_num_faces
    }

    if(m_verbosity > 1)
    LOG(info) 
        << "other GMergedMesh" 
        << " selected " << selected
        << " num_mergedmesh " << m_num_mergedmesh 
        << " num_volumes " << m_num_volumes 
        << " num_volumes_selected " << m_num_volumes_selected 
        ;
}

/**
GMergedMesh::mergeVolume
--------------------------

GMergedMesh::create invokes GMergedMesh::mergeVolume from node tree traversal 
via the recursive GMergedMesh::traverse_r 

GNode::getRelativeTransform
    relative transform calculated from the product of ancestor transforms
    after the base node (ie traverse ancestors starting from root to this node
    but only collect transforms after the base node : which is required to be 
    an ancestor of this node)

GNode::getTransform
    global transform, ie product of all transforms from root to this node
    When node == base the transform is identity


TO INVESTIGATE:

Currently the full tree is traversed for every merged mesh and 
only the ridx selected volumes are merged.  

Perhaps better to implement this from a higher level
class and contribute to multiple GMergedMeshes at once
allowing all to be populated with a single traverse 
of the volume tree ?


GInstancer is the driver of this in standard geocache creation::

    (gdb) bt
    #0  0x00007ffff44f449b in raise () from /lib64/libpthread.so.0
    #1  0x00007fffe5ce5a84 in GMergedMesh::mergeVolume (this=0xeafac2c0, volume=0x3374950, selected=true, verbosity=0) at /home/blyth/opticks/ggeo/GMergedMesh.cc:480
    #2  0x00007fffe5ce516c in GMergedMesh::traverse_r (this=0xeafac2c0, node=0x3374950, depth=0, pass=1, verbosity=0) at /home/blyth/opticks/ggeo/GMergedMesh.cc:337
    #3  0x00007fffe5ce4a44 in GMergedMesh::Create (ridx=0, base=0x0, root=0x3374950, verbosity=0) at /home/blyth/opticks/ggeo/GMergedMesh.cc:265
    #4  0x00007fffe5cc681d in GGeoLib::makeMergedMesh (this=0x26bf080, index=0, base=0x0, root=0x3374950, verbosity=0) at /home/blyth/opticks/ggeo/GGeoLib.cc:276
    #5  0x00007fffe5cdabce in GInstancer::makeMergedMeshAndInstancedBuffers (this=0x26bfd70, verbosity=0) at /home/blyth/opticks/ggeo/GInstancer.cc:560
    #6  0x00007fffe5cd8c5f in GInstancer::createInstancedMergedMeshes (this=0x26bfd70, delta=true, verbosity=0) at /home/blyth/opticks/ggeo/GInstancer.cc:98
    #7  0x00007fffe5cf1b65 in GGeo::prepareVolumes (this=0x26b8180) at /home/blyth/opticks/ggeo/GGeo.cc:1322
    #8  0x00007fffe5ceec17 in GGeo::prepare (this=0x26b8180) at /home/blyth/opticks/ggeo/GGeo.cc:688
    #9  0x0000000000405263 in main (argc=13, argv=0x7fffffffd748) at /home/blyth/opticks/okg4/tests/OKX4Test.cc:134


**/

void GMergedMesh::mergeVolume( GVolume* volume, bool selected, unsigned verbosity )
{
    GNode* node = static_cast<GNode*>(volume);
    GNode* base = getCurrentBase();
    unsigned ridx = volume->getRepeatIndex() ;  

    GMatrixF* transform = base ? volume->getRelativeTransform(base) : volume->getTransform() ;     // base or root relative global transform

    if( node == base ) assert( transform->isIdentity() ); 
    if( ridx == 0 ) assert( base == NULL && "expecting NULL base for ridx 0" ); 


    float* dest = getTransform(m_cur_volume);
    assert(dest);
    transform->copyTo(dest);

    const GMesh* mesh = volume->getMesh();   // triangulated
    unsigned num_vert = mesh->getNumVertices();
    unsigned num_face = mesh->getNumFaces();

    LOG(debug)
        << " m_cur_volume " << std::setw(6) << m_cur_volume
        << " selected " << ( selected ? "YES" : "NO" ) 
        << " num_vert " << std::setw(5) << num_vert
        << " num_face " << std::setw(5) << num_face
        ;  
 

    guint3* faces = mesh->getFaces();
    gfloat3* vertices = mesh->getTransformedVertices(*transform) ;
    gfloat3* normals  = mesh->getTransformedNormals(*transform);  

    if(verbosity > 3) mergeVolumeDump(volume);
    mergeVolumeBBox(vertices, num_vert);
    mergeVolumeIdentity(volume, selected );

    m_cur_volume += 1 ;    // irrespective of selection, as prefer absolute volume indexing 

    if(selected)
    {

        mergeVolumeVertices( num_vert, vertices, normals );

        unsigned* node_indices     = volume->getNodeIndices();
        unsigned* boundary_indices = volume->getBoundaryIndices();
        unsigned* sensor_indices   = volume->getSensorIndices();

        mergeVolumeFaces( num_face, faces, node_indices, boundary_indices, sensor_indices  );
   
#ifdef GPARTS_HOT 
        GParts* parts = volume->getParts();  // analytic 
        mergeVolumeAnalytic( parts, transform, verbosity );
#endif

        GPt* pt = volume->getPt();  // analytic 
        mergeVolumeAnalytic( pt, transform, verbosity );


        // offsets with the flat arrays
        m_cur_vertices += num_vert ;  
        m_cur_faces    += num_face ; 
    }
}


/**
GMergedMesh::mergeMergedMesh
--------------------------------

Volumes are present irrespective of selection as prefer absolute volume indexing 

Called from:

    GMergedMeshTest:test_GMergedMesh_MakeLODComposite...GMergedMesh::MakeComposite
    

**/

void GMergedMesh::mergeMergedMesh( GMergedMesh* other, bool selected, unsigned verbosity )
{
    //std::raise(SIGINT);  // seeing where this is called
    GParts* pts = other->getParts(); 

    if(pts && !m_parts) 
    {
        m_parts = new GParts() ; 
        if(pts->isPartList()) m_parts->setPartList();
    }

    if(pts)
    {
        m_parts->add( pts, verbosity );
    }

    unsigned int nvolume = other->getNumVolumes();

    //if(m_verbosity > 1)
    LOG(info) 
        << " m_cur_volume " << m_cur_volume
        << " m_cur_vertices " << m_cur_vertices
        << " m_cur_faces " << m_cur_faces
        << " other nvolume " << nvolume 
        << " selected " << selected
        ; 


    for(unsigned int i=0 ; i < nvolume ; i++)
    {
        gbbox bb = other->getBBox(i) ;
        gfloat4 ce = other->getCenterExtent(i) ;

        if(m_verbosity > 2)
        LOG(info) 
            << " m_cur_volume " << m_cur_volume  
            << " i " << i
            << " ce " <<  ce.description() 
            << " bb " <<  bb.description() 
            ;

        m_bbox[m_cur_volume] = bb ;  
        m_center_extent[m_cur_volume] = ce ;
        m_nodeinfo[m_cur_volume] = other->getNodeInfo(i) ; 
        m_identity[m_cur_volume] = other->getIdentity(i) ; 
        m_meshes[m_cur_volume] = other->getMeshIndice(i) ; 

        memcpy( getTransform(m_cur_volume), other->getTransform(i), 16*sizeof(float) ); 

        m_cur_volume += 1 ; 
    }

    unsigned int nvert = other->getNumVertices();
    unsigned int nface = other->getNumFaces();

    gfloat3* vertices = other->getVertices();
    gfloat3* normals = other->getNormals();
    guint3*  faces = other->getFaces();

    unsigned* node_indices = other->getNodes();
    unsigned* boundary_indices = other->getBoundaries();
    unsigned* sensor_indices = other->getSensors();


    // needs to be here, prior to incrementing m_cur_faces, m_cur_vertices
    glm::uvec4 eidx(m_cur_faces, nface, m_cur_vertices, nvert ); 
    setComponent(eidx, m_cur_mergedmesh ); 
    m_cur_mergedmesh++ ; 


    if(selected)
    {
        mergeVolumeVertices( nvert, vertices, normals );
        mergeVolumeFaces(    nface, faces, node_indices, boundary_indices, sensor_indices );

        m_cur_vertices += nvert ;
        m_cur_faces    += nface ;
        // offsets within the flat arrays
    }
}


void GMergedMesh::mergeVolumeDump( GVolume* volume)
{
    const char* pvn = volume->getPVName() ;
    const char* lvn = volume->getLVName() ;
    guint4 _identity = volume->getIdentity();
    unsigned ridx = volume->getRepeatIndex() ;  

    LOG(info) 
        << " m_cur_volume " << m_cur_volume
        << " idx " << volume->getIndex()
        << " ridx " << ridx
        << " id " << _identity.description()
        << " pv " << ( pvn ? pvn : "-" )
        << " lv " << ( lvn ? lvn : "-" )
        ;
}

void GMergedMesh::mergeVolumeBBox( gfloat3* vertices, unsigned nvert )
{
    // needs to be outside the selection branch for the all volume center extent
    gbbox* bb = GMesh::findBBox(vertices, nvert) ;
    if(bb == NULL) LOG(fatal) << "NULL bb " ; 
    assert(bb); 

    m_bbox[m_cur_volume] = *bb ;  
    m_center_extent[m_cur_volume] = bb->center_extent() ;
}

void GMergedMesh::mergeVolumeIdentity( GVolume* volume, bool selected )
{
    const GMesh* mesh = volume->getMesh();

    unsigned nvert = mesh->getNumVertices();
    unsigned nface = mesh->getNumFaces();

    guint4 _identity = volume->getIdentity();

    unsigned nodeIndex = volume->getIndex();
    unsigned meshIndex = mesh->getIndex();
    unsigned boundary = volume->getBoundary();

    NSensor* sensor = volume->getSensor();
    unsigned sensorIndex = NSensor::RefIndex(sensor) ; 

    assert(_identity.x == nodeIndex);
    assert(_identity.y == meshIndex);
    assert(_identity.z == boundary);
    //assert(_identity.w == sensorIndex);   this is no longer the case, now require SensorSurface in the identity
    
    LOG(debug) 
        << " m_cur_volume " << m_cur_volume 
        << " nodeIndex " << nodeIndex
        << " boundaryIndex " << boundary
        << " sensorIndex " << sensorIndex
        << " sensor " << ( sensor ? sensor->description() : "NULL" )
        ;


    GNode* parent = volume->getParent();
    unsigned int parentIndex = parent ? parent->getIndex() : UINT_MAX ;

    m_meshes[m_cur_volume] = meshIndex ; 

    // face and vertex counts must use same selection as above to be usable 
    // with the above filled vertices and indices 

    m_nodeinfo[m_cur_volume].x = selected ? nface : 0 ; 
    m_nodeinfo[m_cur_volume].y = selected ? nvert : 0 ; 
    m_nodeinfo[m_cur_volume].z = nodeIndex ;  
    m_nodeinfo[m_cur_volume].w = parentIndex ; 

    if(isGlobal())
    {
         if(nodeIndex != m_cur_volume)
             LOG(fatal) << "mismatch" 
                        <<  " nodeIndex " << nodeIndex 
                        <<  " m_cur_volume " << m_cur_volume
                        ; 

         //assert(nodeIndex == m_cur_volume);  // trips ggv-pmt still needed ?
    } 
    m_identity[m_cur_volume] = _identity ; 
}

void GMergedMesh::mergeVolumeVertices( unsigned nvert, gfloat3* vertices, gfloat3* normals )
{
    for(unsigned i=0 ; i < nvert ; ++i )
    {
        m_vertices[m_cur_vertices+i] = vertices[i] ; 
        m_normals[m_cur_vertices+i] = normals[i] ; 
    }
}

void GMergedMesh::mergeVolumeFaces( unsigned nface, guint3* faces, unsigned* node_indices, unsigned* boundary_indices, unsigned* sensor_indices )
{
    assert(node_indices);
    assert(boundary_indices);
    assert(sensor_indices);

    for(unsigned i=0 ; i < nface ; ++i )
    {
        m_faces[m_cur_faces+i].x = faces[i].x + m_cur_vertices ;  
        m_faces[m_cur_faces+i].y = faces[i].y + m_cur_vertices ;  
        m_faces[m_cur_faces+i].z = faces[i].z + m_cur_vertices ;  

        // TODO: consolidate into uint4 
        m_nodes[m_cur_faces+i]      = node_indices[i] ;
        m_boundaries[m_cur_faces+i] = boundary_indices[i] ;
        m_sensors[m_cur_faces+i]    = sensor_indices[i] ;
    }

}


/**
GMergedMesh::mergeVolumeAnalytic
----------------------------------

Analytic CSG combined at node level.

NB this is a very rare/unsual piece of code, because of the meeting 
between the CSG tree of transforms and the structure tree of transforms/
  
* doing the applyPlacementTransform inside GMergedMesh seems mal-placed ? 
  Always have to search to find it.

Aiming to allow postcache deferred analytic merging from a bunch of NCSG 
and the GPt collected into GPts


**/

#ifdef GPARTS_HOT
void GMergedMesh::mergeVolumeAnalytic( GParts* parts, GMatrixF* transform, unsigned verbosity )
{
    if(!parts)
    {
        LOG(fatal) << "parts NULL  " ;
    }
    assert(parts);

    if(transform && !transform->isIdentity())
    {
        LOG(debug) << "applyPlacementTransform" ; 
        parts->applyPlacementTransform(transform, verbosity );
    }

    if(!m_parts) m_parts = new GParts() ; 

    m_parts->add(parts, verbosity); 
}
#endif


void GMergedMesh::mergeVolumeAnalytic( GPt* pt, GMatrixF* transform, unsigned /*verbosity*/ )
{
    if(!pt) return ;  // this happens with AssimpRapTest

    const float* data = static_cast<float*>(transform->getPointer());

    glm::mat4 placement = glm::make_mat4( data ) ;  

    pt->setPlacement(placement);  

    m_pts->add( pt );   
}



void GMergedMesh::reportMeshUsage(GGeo* ggeo, const char* msg)
{
     LOG(info) << msg ; 
     typedef std::map<unsigned int, unsigned int>::const_iterator MUUI ; 

     unsigned int tv(0) ; 
     for(MUUI it=m_mesh_usage.begin() ; it != m_mesh_usage.end() ; it++)
     {
         unsigned int meshIndex = it->first ; 
         unsigned int nodeCount = it->second ; 
 
         const GMesh* mesh = ggeo->getMesh(meshIndex);
         const char* meshName = mesh->getName() ; 
         unsigned int nv = mesh->getNumVertices() ; 
         unsigned int nf = mesh->getNumFaces() ; 

         printf("  %4d (v%5d f%5d) : %6d : %7d : %s \n", meshIndex, nv, nf, nodeCount, nodeCount*nv, meshName);

         tv += nodeCount*nv ; 
     }
     printf(" tv : %7d \n", tv);
}




GMergedMesh* GMergedMesh::Load(Opticks* opticks, unsigned int ridx, const char* version)  // static
{
    std::string mmpath = opticks->getResource()->getMergedMeshPath(ridx);
    GMergedMesh* mm = GMergedMesh::Load(mmpath.c_str(), ridx, version);
    return mm ; 
}



GMergedMesh* GMergedMesh::Load(const char* dir, unsigned int index, const char* version) // static
{
    GMergedMesh* mm(NULL);

    std::string cachedir = BFile::FormPath(dir, NULL, NULL);
    bool existsdir = BFile::ExistsDir(dir, NULL, NULL);

    LOG(debug) 
        << " dir " << dir 
        << " -> cachedir " << cachedir
        << " index " << index
        << " version " << version
        << " existsdir " << existsdir
        ;
 

    if(!existsdir)
    {
        LOG(warning) << "directory DOES NOT EXIST " <<  dir ;
    }
    else
    {
        mm = new GMergedMesh(index);
        if(index == 0) mm->setVersion(version);  // mesh versioning applies to  global buffer 
        mm->loadBuffers(cachedir.c_str());
    }
    return mm ; 
}



void GMergedMesh::dumpVolumes(const char* msg) const 
{
    gfloat4 ce0 = getCenterExtent(0) ;
    LOG(info) 
         << msg 
         << " ce0 " << ce0.description() 
         << " NumVolumes " << getNumVolumes()
         << " NumVolumesSelected " << getNumVolumesSelected()
         ; 


    for(unsigned int index=0 ; index < getNumVolumes() ; ++index)
    {
        gfloat4 ce = getCenterExtent(index) ;
        gbbox bb = getBBox(index) ; 
        std::cout 
             << std::setw(5)  << index         
             << " ce " << std::setw(64) << ce.description()       
             << " bb " << std::setw(64) << bb.description()       
             << std::endl 
             ;
    }

    for(unsigned int index=0 ; index < getNumVolumes() ; ++index)
    {
        guint4* ni = getNodeInfo() + index ; 
        guint4* id = getIdentity() + index ; 
        std::cout 
             << std::setw(5)  << index         
             << " ni[nf/nv/nidx/pidx]"  << ni->description()
             << " id[nidx,midx,bidx,sidx] " << id->description() 
             << std::endl 
             ;
    }
}






void GMergedMesh::dumpVolumesSelected(const char* msg) const 
{
    gfloat4 ce0 = getCenterExtent(0) ;
    LOG(info) 
         << msg 
         << " ce0 " << ce0.description() 
         << " NumVolumes " << getNumVolumes()
         << " NumVolumesSelected " << getNumVolumesSelected()
         ; 

    unsigned count(0u) ;  
    for(unsigned int index=0 ; index < getNumVolumes() ; ++index)
    {
        gfloat4 ce = getCenterExtent(index) ;
        //gbbox bb = getBBox(index) ; 
        guint4* ni = getNodeInfo() + index ; 
        guint4* id = getIdentity() + index ; 

        unsigned nf = ni->x ; 
        unsigned nv = ni->y ; 
        if( nf == 0 && nv == 0 ) continue ; 

        

        std::cout 
             << " count " << std::setw(5) << count 
             << " idx " << std::setw(5)  << index         
             << " ce " << std::setw(64) << ce.description()       
           // << " bb " << std::setw(64) << bb.description()       
             << " ni[nf/nv/nidx/pidx]"  << std::setw(30) << ni->description()
             << " id[nidx,midx,bidx,sidx] " << std::setw(30) << id->description() 
             << std::endl 
             ;

        count += 1 ;  
    }


}








void GMergedMesh::dumpVolumesFaces(const char* msg) const  // migrated from OGeo
{
    const GMergedMesh* mm = this ; 
    unsigned numVolumes = mm->getNumVolumes();
    unsigned numFaces = mm->getNumFaces();

    LOG(info) << msg 
              << " numVolumes " << numVolumes
              << " numFaces " << numFaces
              << " mmIndex " << mm->getIndex()
             ; 

    unsigned numFacesTotal = 0 ;  
    for(unsigned i=0 ; i < numVolumes ; i++)
    {
         guint4 ni = mm->getNodeInfo(i) ;
         numFacesTotal += ni.x ; 
         guint4 id = mm->getIdentity(i) ;
         guint4 ii = mm->getInstancedIdentity(i) ;
         glm::vec4 ce = mm->getCE(i) ; 

         std::cout 
             << std::setw(5)  << i     
             << " ni[nf/nv/nidx/pidx]"  << ni.description()
             << " id[nidx,midx,bidx,sidx] " << id.description() 
             << " ii[] " << ii.description() 
             << " " << gpresent("ce", ce ) 
             ;    
    }
    assert( numFacesTotal == numFaces ) ; 
}




void GMergedMesh::dumpTransforms( const char* msg) const  // migrated from OGeo
{
    const GMergedMesh* mm = this ; 

    LOG(info) << msg ; 

    NPY<float>* itransforms = mm->getITransformsBuffer();

    NSlice* islice = mm->getInstanceSlice(); 

    unsigned numTransforms = islice->count();

    if(!islice) islice = new NSlice(0, itransforms->getNumItems()) ;

    for(unsigned i=islice->low ; i<islice->high ; i+=islice->step)
    {
        glm::mat4 m4 = itransforms->getMat4(i) ; 

        //const float* tdata = glm::value_ptr(m4) ;  
        
        glm::vec4 ipos = m4[3] ; 

        if(islice->isMargin(i,5))
        std::cout
             << "[" 
             << std::setw(2) << mm->getIndex() 
             << "]" 
             << " (" 
             << std::setw(6) << i << "/" << std::setw(6) << numTransforms 
             << " ) "   
             << gpresent(" ip",ipos) 
             ;
     }
}










float* GMergedMesh::getModelToWorldPtr(unsigned int index)
{
    return index == 0 ? GMesh::getModelToWorldPtr(0) : NULL ;
}


/**
GMergedMesh::addInstancedBuffers
-----------------------------------

hmm for very large numbers of instances : probably better to defer this post-cache
and/or use some thrust trickery to do the repeating at GPU upload stage


**/


void GMergedMesh::addInstancedBuffers(const std::vector<GNode*>& placements)
{
    LOG(LEVEL) << " placements.size() " << placements.size() ; 

    NPY<float>* itransforms = GTree::makeInstanceTransformsBuffer(placements); // collect GNode placement transforms into buffer
    setITransformsBuffer(itransforms);

    NPY<unsigned int>* iidentity  = GTree::makeInstanceIdentityBuffer(placements);
    setInstancedIdentityBuffer(iidentity);

    NPY<unsigned int>* aii   = GTree::makeAnalyticInstanceIdentityBuffer(placements);
    setAnalyticInstancedIdentityBuffer(aii);
}




GMergedMesh*  GMergedMesh::MakeComposite(std::vector<GMergedMesh*> mms ) // static
{
    assert(mms.size() > 0 );

    unsigned nmm = mms.size();

    GMergedMesh* mm0 = mms[0] ; 

    GMergedMesh* com = new GMergedMesh( mm0->getIndex() ); 

    unsigned verbosity = mm0 ? mm0->getVerbosity() : 0 ;

    LOG(info)
        << " verbosity " << verbosity 
        << " nmm " << nmm 
        ;

    com->setVerbosity(verbosity );

    for(unsigned i=0 ; i < nmm ; i++)
    {
        GMergedMesh* mm = mms[i] ;
        com->countMergedMesh(mm, true);
    } 

    com->allocate(); 
 
    for(unsigned i=0 ; i < nmm ; i++)
    {
        GMergedMesh* mm = mms[i] ;
        com->mergeMergedMesh(mm, true, verbosity );
    } 

    //com->updateBounds(); ?

    com->dumpVolumes("GMergedMesh::MakeComposite");
   
    return com ; 
}


GMergedMesh*  GMergedMesh::MakeLODComposite(GMergedMesh* mm, unsigned levels ) // static
{
    assert( levels == 2 || levels == 3 );
    gbbox bb = mm->getBBox(0);


    bool cfq = CheckFacesQty(mm);

    if(!cfq)
    {
        LOG(warning) << "mm misses sensors/boundaries/nodes invoking setFacesQty " ; 
        mm->setFacesQty(NULL); 
    }


    std::vector<GMergedMesh*> comps ; 
    comps.push_back(mm);

    GMergedMesh* bbmm = CreateBBoxMesh(mm->getIndex(), bb );
    comps.push_back(bbmm);

    if(levels == 3)
    {
        GMergedMesh* qmm = CreateQuadMesh(mm->getIndex(), bb );
        comps.push_back(qmm);
    }

    GMergedMesh* lodcomp = MakeComposite(comps);

    lodcomp->stealIdentity(mm);
    
    return lodcomp ; 
}




GMergedMesh* GMergedMesh::CreateBBoxMesh(unsigned index, gbbox& bb ) // static
{
     // this was implemented as a workaround for GBBoxMesh not inheriting from GMergedMesh 

     unsigned num_vertices = GBBoxMesh::NUM_VERTICES ; // 24 = 6*4  
     gfloat3* vertices = new gfloat3[num_vertices] ; 
     gfloat3* normals  = new gfloat3[num_vertices] ; 
     gfloat2* texcoords  = NULL ; 

     unsigned num_faces = GBBoxMesh::NUM_FACES  ;    // 12 = 6*2  2-tri for each bbox facet 
     guint3* faces = new guint3[num_faces] ;

     GBBoxMesh::twentyfour(bb, vertices, faces, normals);

     GMergedMesh* bbmm = new GMergedMesh(
             index, 
             vertices, 
             num_vertices, 
             faces, 
             num_faces, 
             normals, 
             texcoords
         );

    bbmm->setFacesQty(NULL); // allocate and zeros: nodes, boundaries and sensors

    bool cfq = CheckFacesQty(bbmm); 
    assert(cfq);

    return bbmm ;   
}


bool GMergedMesh::CheckFacesQty(const GMergedMesh* mm)
{
     unsigned* node_indices = mm->getNodes();
     unsigned* boundary_indices = mm->getBoundaries();
     unsigned* sensor_indices = mm->getSensors();

     //assert( node_indices );
     //assert( boundary_indices );
     //assert( sensor_indices );

     return node_indices && boundary_indices && sensor_indices ;
}

/**
GMergedMesh::CreateQuadMesh
----------------------------

This is used to make LOD (level-of-detail) standins for 
geometry that are swapped in when the geometry is very distant.
The quad standin is constructed from the bbox cuboid by collapsing the
smallest dimension.

TODO: try moving to GMeshMaker and create GMesh, 
      as looks like is not using anything of GMergedMesh ?

**/

GMergedMesh* GMergedMesh::CreateQuadMesh(unsigned index, gbbox& bb ) // static
{
    unsigned num_vertices = 8 ; 
    gfloat3* vertices = new gfloat3[num_vertices] ; 
    gfloat3* normals  = new gfloat3[num_vertices] ; 
    gfloat2* texcoords  = NULL ; 
    unsigned num_faces = 4  ; 
    
    guint3* faces = new guint3[num_faces] ;
    

    GMergedMesh* qmm = new GMergedMesh(
             index, 
             vertices, 
             num_vertices, 
             faces, 
             num_faces, 
             normals, 
             texcoords
         );


    qmm->setFacesQty(NULL); // allocate and zeros: nodes, boundaries and sensors

    bool cfq = CheckFacesQty(qmm); 
    assert(cfq);

       
     gfloat3 dim = bb.dimensions() ;

     gfloat3 cen = bb.center(); 

     // collapse the smallest dimension

     faces[0] = guint3( 0,1,2) ;
     faces[1] = guint3( 2,1,3) ;
     faces[2] = guint3( 4,6,5) ;
     faces[3] = guint3( 6,7,5) ;

     if(dim.z <= dim.x && dim.z <= dim.y )  
     {

         /*
           Smallest in Z


                Y
                | 
                |
                |
                +----- X
               /
              /
             Z

       Need to dupe verts as need slots for opposed normals

               Y 
               |
               2----3
               | .  |
               |  . |
               0----1----> X    +Z normals 
              /
             /        (0,1,2) (2,1,3)
            Z
 
               Y 
               |
               6----7
               | .  |
               |  . |
               4----5---- X     -Z normals
              /
             /
            Z       (4,6,5) (6,7,5) 

              
         */

         vertices[0] = gfloat3( bb.min.x , bb.min.y , cen.z  ) ; 
         vertices[1] = gfloat3( bb.max.x , bb.min.y , cen.z  ) ;
         vertices[2] = gfloat3( bb.min.x , bb.max.y , cen.z  ) ; 
         vertices[3] = gfloat3( bb.max.x , bb.max.y , cen.z  ) ;

         for(unsigned i=0 ; i < 4 ; i++) vertices[4+i] = vertices[i] ; 
         for(unsigned i=0 ; i < 8 ; i++) normals[i] = gfloat3( 0.f, 0.f, i < 4 ? 1.f : -1.f )  ;
 
     }
     else if(dim.y <= dim.x && dim.y <= dim.z ) // smallest in y 
     {

         /*

                Z
                | 
                |
                |
                +----- X
               /
              /
            -Y

         */

         vertices[0] = gfloat3( bb.min.x , cen.y , bb.min.z ) ; 
         vertices[1] = gfloat3( bb.max.x , cen.y , bb.min.z ) ;
         vertices[2] = gfloat3( bb.min.x , cen.y , bb.max.z ) ; 
         vertices[3] = gfloat3( bb.max.x , cen.y , bb.max.z ) ;

         for(unsigned i=0 ; i < 4 ; i++) vertices[4+i] = vertices[i] ; 
         for(unsigned i=0 ; i < 8 ; i++) normals[i] = gfloat3( 0.f, ( i < 4 ? -1.f : 1.f ), 0.f )  ;

     }
     else if(dim.x <= dim.y && dim.x <= dim.z )  // smallest in x 
     {
         /*

                Z
                | 
                |
                |
                +----- Y
               /
              /
             X

         */

         vertices[0] = gfloat3( cen.x , bb.min.y , bb.min.z ) ; 
         vertices[1] = gfloat3( cen.x , bb.max.y , bb.min.z ) ;
         vertices[2] = gfloat3( cen.x , bb.min.y , bb.max.z ) ; 
         vertices[3] = gfloat3( cen.x , bb.max.y , bb.max.z ) ;

         for(unsigned i=0 ; i < 4 ; i++) vertices[4+i] = vertices[i] ; 
         for(unsigned i=0 ; i < 8 ; i++) normals[i] = gfloat3( ( i < 4 ? 1.f : -1.f ), 0.f , 0.f  )  ;

     }
     else
     {
         assert(0); 
     }



    return qmm ;   

}



std::string GMergedMesh::Desc(const GMergedMesh* mm)
{
    const char geocode = mm ? mm->getGeoCode() : '-' ;
    unsigned numVolumes = mm ? mm->getNumVolumes() : -1 ;
    unsigned numFaces = mm ? mm->getNumFaces() : -1 ;
    unsigned numITransforms = mm ? mm->getNumITransforms() : -1 ;
    unsigned index = mm->getIndex(); 

    std::stringstream ss ; 
    ss
        << "mm" 
        << " index " << std::setw(3) << index
        << " geocode " << std::setw(3) << geocode 
        << std::setw(5) << ( mm ? " " : "NULL" ) 
        << std::setw(5) << ( mm && mm->isSkip(  ) ? "SKIP" : " " ) 
        << std::setw(7) << ( mm && mm->isEmpty()  ? "EMPTY" : " " ) 
        << " numVolumes " << std::setw(10) << numVolumes
        << " numFaces  " << std::setw(10) << numFaces
        << " numITransforms  " << std::setw(10) << numITransforms
        << " numITransforms*numVolumes  " << std::setw(10) << numITransforms*numVolumes
        << " GParts " << ( mm->getParts() ? "Y" : "N"  )
        << " GPts " <<   ( mm->getPts() ? "Y" : "N"  )
        ;   

   return ss.str();
}



