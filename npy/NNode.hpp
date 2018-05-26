#pragma once

#include <string>
#include <vector>
#include <functional>

#include "OpticksCSG.h"
#include "NQuad.hpp"
#include "Nuv.hpp"
#include "NPY_API_EXPORT.hh"

#include <glm/fwd.hpp>

class BRng ; 

struct nbbox ; 
struct npart ; 
struct NSceneConfig ; 

class NNodeDump ; 
class NNodePoints ; 
class NParameters ;
//struct nuv ; 

// NGLMExt
struct nmat4pair ; 
struct nmat4triple ; 

#include "NNodeEnum.hpp"

struct NPY_API nnode 
{
    //virtual float operator()(const glm::vec3& p) const ;
    virtual float operator()(float px, float py, float pz) const  ;
    virtual float sdf_(const glm::vec3& pos, NNodeFrameType fr) const ;

    std::function<float(float,float,float)> sdf() const ;

    static nnode* load(const char* treedir, const NSceneConfig* config=NULL);
    static void AdjustToFit(nnode* node, const nbbox& bb, float scale, float delta ) ;

    virtual const char* csgname() const ;  
    virtual nbbox bbox() const ;

    void check_primitive_bb( const nbbox& bb) const  ;
    void get_composite_bbox( nbbox& bb ) const ;
    void get_primitive_bbox( nbbox& bb ) const ;

    virtual npart part() const ;
    virtual unsigned maxdepth();
    virtual unsigned _maxdepth(unsigned depth);

    static const unsigned desc_indent ; 
    virtual std::string desc() const ;
    std::string tag() const ;

    static void Tests(std::vector<nnode*>& nodes );
    static void Init(nnode& n, OpticksCSG_t type, nnode* left=NULL, nnode* right=NULL);

    //unsigned uncoincide(unsigned verbosity);
    //bool can_uncoincide(const nnode* a, const nnode* b) const ;


    //glm::uvec4 getCompositePoints( std::vector<glm::vec3>& surf, unsigned level, int margin , unsigned pointmask, float epsilon, const glm::mat4* tr ) const ;
    //glm::uvec4 selectBySDF(std::vector<glm::vec3>& dest, const std::vector<glm::vec3>& source, unsigned pointmask, float epsilon, const glm::mat4* tr) const ;
    //void dumpPointsSDF(const std::vector<glm::vec3>& points, float epsilon ) const ;

    //void getParPoints( std::vector<glm::vec3>& parpoi, unsigned prim_idx, unsigned level, unsigned margin, NNodeFrameType frty, unsigned verbosity  ) const;
    //void getSurfacePointsAll(       std::vector<glm::vec3>& surf,        unsigned level, int margin, NNodeFrameType fr, unsigned verbosity) const ;
    //void getSurfacePoints(          std::vector<glm::vec3>& surf, int s, unsigned level, int margin, NNodeFrameType fr, unsigned verbosity) const ;


    // back-compat : to be reworked
    void dumpSurfacePointsAll(const char* msg, NNodeFrameType fr) const ;
    


    void collectParPointsSheet(unsigned prim_idx, int sheet, unsigned level, int margin, NNodeFrameType fr, unsigned verbosity) ;
    void collectParPoints(     unsigned prim_idx,            unsigned level, int margin, NNodeFrameType fr, unsigned verbosity) ;

    const std::vector<glm::vec3>& get_par_points() const ;
    const std::vector<nuv>&       get_par_coords() const ;
 

    void getCoincidentSurfacePoints(std::vector<nuv>& coincident, int s, unsigned level, int margin, const nnode* other, float epsilon, NNodeFrameType fr) const ;
    void getCoincident(             std::vector<nuv>& coincident, const nnode* other, float epsilon=1e-5f, unsigned level=1, int margin=1, NNodeFrameType fr=FRAME_LOCAL) const ;


    glm::vec3 center() const  ;      // override if needed
    glm::vec3 direction() const  ;   // override if needed

    glm::vec3 gseeddir() const ;     
    glm::vec3 gseedcenter() const ;  

    glm::vec3 par_pos_(const nuv& uv, NNodeFrameType fr) const ;
    glm::vec3 par_pos_(const nuv& uv, const nmat4triple* triple) const ;
    glm::vec3 par_pos_local(const nuv& uv) const ;  // "transform"  local node frame
    glm::vec3 par_pos_global(const nuv& uv) const ; // "gtransform" CSG tree root node frame 


    virtual glm::vec3 par_pos_model(const nuv& uv) const ;
    virtual void      par_posnrm_model(glm::vec3& pos, glm::vec3& nrm, unsigned sheet, float fu, float fv ) const ;        


    virtual unsigned  par_nsurf() const ;
    virtual int       par_euler() const ; 
    virtual unsigned  par_nvertices(unsigned nu, unsigned nv) const ;
    virtual void      nudge(unsigned s, float delta);

    static void _par_pos_endcap(glm::vec3& pos,  const nuv& uv, const float r_, const float z_ ) ; 


    void selectSheets( std::vector<unsigned>& sheets, unsigned sheetmask ) const ;
    void generateParPoints(unsigned seed, const glm::vec4& uvdom, std::vector<glm::vec3>& points, std::vector<glm::vec3>& normals, unsigned num_total, unsigned sheetmask ) const   ;
    void generateParPointsSheet(std::vector<glm::vec3>& points, std::vector<glm::vec3>& normals, BRng& ugen, BRng& vgen, unsigned sheet, unsigned num ) const ;



    // see NNodeUncoincide
    virtual void increase_z2(float dz);
    virtual void decrease_z1(float dz);
    virtual float z1() const ; 
    virtual float z2() const ; 
    virtual float r1() const ; 
    virtual float r2() const ; 


    void update_gtransforms();
    static void update_gtransforms_r(nnode* node);

    const nmat4triple* global_transform(); 
    static const nmat4triple* global_transform(nnode* n); 

    glm::vec3 apply_gtransform(const glm::vec4& v_) const ;

    void collect_prim_centers(std::vector<glm::vec3>& centers, std::vector<glm::vec3>& dirs, int verbosity=0);
    void collect_prim_centers(std::vector<glm::vec3>& centers, std::vector<glm::vec3>& dirs,  const nnode* p  );



    virtual void pdump(const char* msg="nnode::pdump") const ; 

    virtual void dump(const char* msg=NULL) const ;
    void dump_prim() const ;
    void dump_transform() const ;
    void dump_gtransform() const ;
    void dump_planes() const ;
  


    unsigned get_num_prim() const ;
    void collect_prim(std::vector<const nnode*>& prim) const ;
    static void collect_prim_r(std::vector<const nnode*>& prim, const nnode* node) ;

    void collect_prim_for_edit(std::vector<nnode*>& prim) ;
    static void collect_prim_for_edit_r(std::vector<nnode*>& prim, nnode* node) ;


    void collect_monogroup( std::vector<const nnode*>& monogroup ) const ;
    void collect_progeny( std::vector<const nnode*>& progeny, OpticksCSG_t qtyp ) const ;
    void collect_ancestors( std::vector<const nnode*>& ancestors, OpticksCSG_t qtyp) const ;
    void collect_connectedtype_ancestors( std::vector<const nnode*>& ancestors) const ;

    static void collect_progeny_r( const nnode* n, std::vector<const nnode*>& progeny, OpticksCSG_t qtyp );
    static void collect_ancestors_( const nnode* n, std::vector<const nnode*>& ancestors, OpticksCSG_t qtyp);
    static void collect_connectedtype_ancestors_( const nnode* n, std::vector<const nnode*>& ancestors, OpticksCSG_t qtyp);

    static bool is_same_union(const nnode* a, const nnode* b) ; // static
    static bool is_same_monogroup(const nnode* a, const nnode* b, OpticksCSG_t op) ; // static


    std::string get_type_mask_string() const ;
    std::string get_prim_mask_string() const ;
    std::string get_oper_mask_string() const ;

    unsigned    get_type_mask() const ;
    unsigned    get_prim_mask() const ;
    unsigned    get_oper_mask() const ;

    unsigned get_mask(NNodeType ntyp) const ;
    static void get_mask_r(const nnode* node, NNodeType ntyp, unsigned& msk);
    std::string get_mask_string(NNodeType ntyp) const ;


    void set_treeidx(int idx) ; 
    void set_treedir(const char* treedir) ; 
    void set_boundary(const char* boundary) ; 

    bool is_znudge_capable() const ;
    bool is_operator() const ;
    bool is_primitive() const ;
    bool is_unbounded() const ;
    bool is_root() const ;
    bool is_bileaf() const ;
    bool has_planes();
    unsigned planeIdx();
    unsigned planeNum();

    unsigned idx ; 
    OpticksCSG_t type ;  
    nnode* left ; 
    nnode* right ; 
    nnode* parent ; 
    nnode* other ; 
    const char* label ; 
    const char* treedir ; 
    int         treeidx ;   
    const char* boundary ; 

    const nmat4triple* transform ; 
    const nmat4triple* gtransform ; 
    unsigned   gtransform_idx ; 
    bool  complement ; 
    int verbosity ; 

    nquad param ; 
    nquad param1 ; 
    nquad param2 ; 
    nquad param3 ; 

    std::vector<glm::vec4> planes ; 
    std::vector<glm::vec3> par_points ; 
    std::vector<nuv>       par_coords ; 

    NParameters*  meta ;
    NNodeDump* _dump ;
    nbbox*   _bbox_model ; 


};

// hmm perhaps easier to switch on these ?? instead
// of having separate types ? 

struct NPY_API nunion : nnode {
    float operator()(float x, float y, float z) const ;
};
struct NPY_API nintersection : nnode {
    float operator()(float x, float y, float z) const ;
};
struct NPY_API ndifference : nnode {
    float operator()(float x, float y, float z) const ;
};


inline NPY_API nunion make_union(nnode* left, nnode* right)
{
    nunion n ;         nnode::Init(n, CSG_UNION , left, right ); return n ; 
}
inline NPY_API nintersection make_intersection(nnode* left, nnode* right)
{
    nintersection n ;  nnode::Init(n, CSG_INTERSECTION , left, right ); return n ;
}
inline NPY_API ndifference make_difference(nnode* left, nnode* right)
{
    ndifference n ;    nnode::Init(n, CSG_DIFFERENCE , left, right ); return n ;
}


