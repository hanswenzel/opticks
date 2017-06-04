#pragma once

#include <vector>
#include <map>

struct nnode ; 

#include "NOpenMeshType.hpp"

template <typename T> struct NOpenMesh ;
template <typename T> struct NOpenMeshProp ;
template <typename T> struct NOpenMeshDesc ;
template <typename T> struct NOpenMeshFind ;

template <typename T>
struct NPY_API  NOpenMeshBuild
{
    typedef typename T::VertexHandle VH ; 
    typedef typename T::FaceHandle   FH ; 

    NOpenMeshBuild( T& mesh, 
                    const NOpenMeshProp<T>& prop, 
                    const NOpenMeshDesc<T>& desc, 
                    const NOpenMeshFind<T>& find );

    VH add_vertex_unique(typename T::Point pt, bool& added, const float epsilon) ;  

    void add_face_(VH v0, VH v1, VH v2, VH v3, int verbosity=0 );
    FH   add_face_(VH v0, VH v1, VH v2, int verbosity=0 );
    bool is_consistent_face_winding(VH v0, VH v1, VH v2);

    void add_parametric_primitive(const nnode* node, int level, int verbosity, int ctrl, float epsilon )  ;
    void euler_check(const nnode* node, int level, int verbosity );

    void copy_faces(const NOpenMesh<T>* other, int facemask, int verbosity, float epsilon );

    void mark_faces(const nnode* other);
    void mark_face(FH fh, const nnode* other);
    std::string desc_inside_other();


    void add_hexpatch(int verbosity);
    void add_tetrahedron(int verbosity);
    void add_cube(int verbosity);


    T& mesh  ;
    const NOpenMeshProp<T>& prop ;
    const NOpenMeshDesc<T>& desc ;
    const NOpenMeshFind<T>& find ;

    std::map<int,int> f_inside_other_count ; 


};
 


