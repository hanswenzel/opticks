#include <cstring>
#include <algorithm>
#include <sstream>

#include "SSys.hh"

#include "BStr.hh"
#include "BFile.hh"


#include "NGLMExt.hpp"
#include "GLMFormat.hpp"
#include "NParameters.hpp"
#include "NPart.h"


#include "NTrianglesNPY.hpp"
#include "NPolygonizer.hpp"

#include "NPrimitives.hpp"


#include "NSceneConfig.hpp"
#include "NScan.hpp"
#include "NNode.hpp"
#include "NNodePoints.hpp"
#include "NNodeUncoincide.hpp"
#include "NNodeNudger.hpp"
#include "NBBox.hpp"
#include "NPY.hpp"
#include "NCSG.hpp"
#include "NTxt.hpp"



#define TREE_NODES(height) ( (0x1 << (1+(height))) - 1 )

#include "PLOG.hh"

const char* NCSG::FILENAME = "csg.txt" ; 

// must match opticks/analytic/csg.py TREE_META NODE_META
const char* NCSG::TREE_META = "meta.json" ;
const char* NCSG::NODE_META = "nodemeta.json" ;

const char* NCSG::PLANES = "planes.npy" ; 
const char* NCSG::SRC_FACES = "srcfaces.npy" ; 
const char* NCSG::SRC_VERTS = "srcverts.npy" ; 

const unsigned NCSG::NTRAN = 3 ; 
const float NCSG::SURFACE_EPSILON = 1e-5f ; 


// ctor : booting via deserialization of directory 
NCSG::NCSG(const char* treedir) 
   :
   m_meta(NULL),
   m_treedir(treedir ? strdup(treedir) : NULL),
   m_index(0),
   m_surface_epsilon(SURFACE_EPSILON),
   m_verbosity(0),
   m_usedglobally(false),
   m_root(NULL),
   m_points(NULL),
   m_uncoincide(NULL),
   m_nudger(NULL),
   m_nodes(NULL),
   m_transforms(NULL),
   m_gtransforms(NULL),
   m_planes(NULL),
   m_srcverts(NULL),
   m_srcfaces(NULL),
   m_num_nodes(0),
   m_num_transforms(0),
   m_num_planes(0),
   m_num_srcverts(0),
   m_num_srcfaces(0),
   m_height(UINT_MAX),
   m_boundary(NULL),
   m_config(NULL),
   m_gpuoffset(0,0,0),
   m_container(0),
   m_containerscale(2.f),
   m_tris(NULL)
{
}

// ctor : booting from in memory node tree
NCSG::NCSG(nnode* root ) 
   :
   m_meta(NULL),
   m_treedir(NULL),
   m_index(0),
   m_surface_epsilon(SURFACE_EPSILON),
   m_verbosity(root->verbosity),
   m_usedglobally(false),
   m_root(root),
   m_points(NULL),
   m_uncoincide(make_uncoincide()),
   m_nudger(make_nudger()),
   m_nodes(NULL),
   m_transforms(NULL),
   m_gtransforms(NULL),
   m_planes(NULL),
   m_srcverts(NULL),
   m_srcfaces(NULL),
   m_num_nodes(0),
   m_num_transforms(0),
   m_num_planes(0),
   m_num_srcverts(0),
   m_num_srcfaces(0), 
   m_height(root->maxdepth()),
   m_boundary(NULL),
   m_config(NULL),
   m_gpuoffset(0,0,0),
   m_container(0),
   m_containerscale(2.f),
   m_tris(NULL)
{

   setBoundary( root->boundary );

   m_num_nodes = NumNodes(m_height);

   m_nodes = NPY<float>::make( m_num_nodes, NJ, NK);
   m_nodes->zero();

   m_transforms = NPY<float>::make(0,NTRAN,4,4) ;  
   m_transforms->zero();

   m_gtransforms = NPY<float>::make(0,NTRAN,4,4) ; 
   m_gtransforms->zero();

   m_planes = NPY<float>::make(0,4);
   m_planes->zero();

   m_meta = new NParameters ; 
}

NNodeUncoincide* NCSG::make_uncoincide() const 
{
    return NULL ;  
    //return new NNodeUncoincide(m_root, m_surface_epsilon, m_root->verbosity);
}
NNodeNudger* NCSG::make_nudger() const 
{
   // when test running from nnode there is no metadata or treedir
   // LOG(info) << soname() << " treeNameIdx " << getTreeNameIdx() ; 

    NNodeNudger* nudger = new NNodeNudger(m_root, m_surface_epsilon, m_root->verbosity);
    return nudger ; 
}



// metadata from the root nodes of the CSG trees for each solid
// pmt-cd treebase.py:Node._get_meta
//
template<typename T>
T NCSG::getMeta(const char* key, const char* fallback ) const 
{
    assert(m_meta) ;    
    return m_meta->get<T>(key, fallback) ;
}

template<typename T>
void NCSG::setMeta(const char* key, T value )
{
    return m_meta->set<T>(key, value) ;
}



template NPY_API void NCSG::setMeta<float>(const char*, float);
template NPY_API void NCSG::setMeta<int>(const char*, int);
template NPY_API void NCSG::setMeta<bool>(const char*, bool);
template NPY_API void NCSG::setMeta<std::string>(const char*, std::string);

template NPY_API std::string NCSG::getMeta<std::string>(const char*, const char*) const ;
template NPY_API int         NCSG::getMeta<int>(const char*, const char*) const ;
template NPY_API float       NCSG::getMeta<float>(const char*, const char*) const ;
template NPY_API bool        NCSG::getMeta<bool>(const char*, const char*) const ;


//std::string NCSG::pvname(){ return getMeta<std::string>("pvname","-") ; }  // <-- NOT APPROPRIATE AS NCSG IS MESH LEVEL NOT NODE LEVEL

// why no lvidx ?

std::string NCSG::lvname() const { return getMeta<std::string>("lvname","-") ; }
std::string NCSG::soname() const { return getMeta<std::string>("soname","-") ; }



std::string NCSG::TestVolumeName(const char* shapename, const char* suffix, int idx) // static
{
    std::stringstream ss ; 
    ss << shapename << "_" << suffix ; 
    if(idx > -1) ss << idx ; 
    ss << "_" ; 
    return ss.str();
}

std::string NCSG::getTestPVName() const 
{
    OpticksCSG_t type = getRootType() ;
    const char* shapename = CSGName(type); 
    unsigned idx = getIndex();
    return TestVolumeName( shapename, "pv", idx);     
}
std::string NCSG::getTestLVName() const 
{
    OpticksCSG_t type = getRootType() ;
    const char* shapename = CSGName(type); 
    unsigned idx = getIndex();
    return TestVolumeName( shapename, "lv", idx);     
}



int NCSG::treeindex() const { return getMeta<int>("treeindex","-1") ; }
int NCSG::depth() const {     return getMeta<int>("depth","-1") ; }
int NCSG::nchild() const {    return getMeta<int>("nchild","-1") ; }

bool NCSG::isSkip() const {       return getMeta<int>("skip","0") == 1 ; }
bool NCSG::is_uncoincide() const { return getMeta<int>("uncoincide","1") == 1 ; }





bool NCSG::isEmit() const 
{  
    int emit = getEmit() ;
    return emit == 1 || emit == -1 ;
}

void NCSG::setEmit(int emit) // used by --testauto
{
    setMeta<int>("emit", emit);
}
int NCSG::getEmit() const 
{   
    return getMeta<int>("emit","0") ; 
}

void NCSG::setEmitConfig(const char* emitconfig)
{
    setMeta<std::string>("emitconfig", emitconfig );
}
const char* NCSG::getEmitConfig() const 
{ 
    std::string ec = getMeta<std::string>("emitconfig","") ;
    return ec.empty() ? NULL : strdup(ec.c_str()) ; 
}




std::string NCSG::meta() const 
{
    std::stringstream ss ; 
    ss << " treeindex " << treeindex()
       << " depth " << depth()
       << " nchild " << nchild()
       << " lvname " << lvname() 
       << " soname " << soname() 
       << " isSkip " << isSkip()
       << " is_uncoincide " << is_uncoincide()
       << " emit " << getEmit()
       ;

    return ss.str();
}

std::string NCSG::smry()
{
    std::stringstream ss ; 
    ss 
       << " ht " << std::setw(2) << m_height 
       << " nn " << std::setw(4) << m_num_nodes
       << " tri " << std::setw(6) << getNumTriangles()
       << " tmsg " << ( m_tris ? m_tris->getMessage() : "NULL-tris" ) 
       << " iug " << m_usedglobally 
       << " nd " << ( m_nodes ? m_nodes->getShapeString() : "NULL" )
       << " tr " << ( m_transforms ? m_transforms->getShapeString() : "NULL" )
       << " gtr " << ( m_gtransforms ? m_gtransforms->getShapeString() : "NULL" )
       << " pln " << ( m_planes ? m_planes->getShapeString() : "NULL" )
       ;

    return ss.str();
}





std::string NCSG::MetaPath(const char* treedir, int idx)
{
    std::string metapath = idx == -1 ? BFile::FormPath(treedir, TREE_META) : BFile::FormPath(treedir, BStr::itoa(idx), NODE_META) ;
    return metapath ; 
}

bool NCSG::Exists(const char* treedir)
{
    return ExistsDir(treedir);
}

bool NCSG::ExistsDir(const char* treedir)
{
    if(!treedir) return false ; 
    if(!BFile::ExistsDir(treedir)) return false ; 
    return true ; 
}


std::string NCSG::TxtPath(const char* treedir)
{
    std::string txtpath = BFile::FormPath(treedir, FILENAME) ;
    return txtpath ; 
}

bool NCSG::ExistsTxt(const char* treedir)
{
    if(!ExistsDir(treedir)) return false ;  
    std::string txtpath = TxtPath(treedir) ; 
    bool exists = BFile::ExistsFile(txtpath.c_str() ); 
    return exists ; 
}

bool NCSG::ExistsMeta(const char* treedir, int idx)
{
    std::string metapath = MetaPath(treedir, idx) ;
    return BFile::ExistsFile(metapath.c_str()) ;
}

NParameters* NCSG::LoadMetadata(const char* treedir, int idx )
{
    // TODO:
    //    per-node metadata, using metaNN.json ?
    //    where NN is the inorder tree index 
    //    which then gets associated to the appropriate node 
    //
    //    Need for access to Trd srcmeta

    NParameters* meta = NULL  ; 
    std::string metapath = MetaPath(treedir, idx) ;

    if(BFile::ExistsFile(metapath.c_str()))
    {
         meta = new NParameters ; 
         meta->load_( metapath.c_str() );
    } 
    else
    {
        // lots missing as are looping over complete tree node count
        // see notes/issues/axel_GSceneTest_fail.rst
        LOG(trace) << "NCSG::LoadMetadata"
                     << " missing metadata "
                     << " treedir " << treedir  
                     << " idx " << idx
                     << " metapath " << metapath
                     ;
        //std::cerr << metapath << std::endl ; 
    }
    return meta ; 
}

void NCSG::loadMetadata()
{
    m_meta = LoadMetadata( m_treedir, -1 );  // -1:treemeta

    if(!m_meta)
    { 
        LOG(fatal) << "NCSG::loadMetadata"
                    << " treedir " << m_treedir 
                    ;
    } 
    assert(m_meta);
    

    m_container       = getMeta<int>("container", "-1");
    m_containerscale  = getMeta<float>("containerscale", "2.");

    std::string gpuoffset = getMeta<std::string>("gpuoffset", "0,0,0" );
    m_gpuoffset = gvec3(gpuoffset);  

    int verbosity     = getMeta<int>("verbosity", "0");
    increaseVerbosity(verbosity);
}

void NCSG::loadNodeMetadata()
{
    // FIX: this idx is not same as real complete binary tree node_idx ?


    // just looping over the number of nodes in the complete tree
    // so it is not surprising that many of them are missing metadata

    std::vector<unsigned> missmeta ;
    assert(m_num_nodes > 0);
    for(unsigned idx=0 ; idx < m_num_nodes ; idx++)
    {
        NParameters* nodemeta = LoadMetadata(m_treedir, idx);
        if(nodemeta)
        { 
            m_nodemeta[idx] = nodemeta ; 
        }
        else
        {
            missmeta.push_back(idx+1);  // make 1-based 
        }
    } 

    LOG(debug) << "NCSG::loadNodeMetadata" 
              << " m_treedir " << m_treedir
              << " m_height " << m_height
              << " m_num_nodes " << m_num_nodes
              << " missmeta " << missmeta.size()
               ; 

/*
    if(missmeta.size() > 0)
    {
        std::cerr << " missmeta 1-based (" << missmeta.size() << ") : " ; 
        for(unsigned i=0 ; i < missmeta.size() ; i++) std::cerr << " " << missmeta[i]  ; 
        std::cerr << std::endl  ; 
    }
*/

}

NParameters* NCSG::getNodeMetadata(unsigned idx) const 
{
    //typedef std::map<unsigned, NParameters*> MUP ; 
    //MUP::const_iterator it = std::find(m_nodemeta.begin(), m_nodemeta.end(), idx) ;
    //if(it == m_nodemeta.end()) return NULL ; 
    return m_nodemeta.count(idx) == 1 ? m_nodemeta.at(idx) : NULL ; 
}

void NCSG::increaseVerbosity(int verbosity)
{
    if(verbosity > -1) 
    {
        if(verbosity > m_verbosity)
        {
            LOG(debug) << "NCSG::increaseVerbosity" 
                       << " treedir " << m_treedir
                       << " old " << m_verbosity 
                       << " new " << verbosity 
                       ; 
        }
        else
        {
            LOG(debug) << "NCSG::increaseVerbosity IGNORING REQUEST TO DECREASE verbosity " 
                       << " treedir " << m_treedir
                       << " current verbosity " << m_verbosity 
                       << " requested : " << verbosity 
                       ; 
 
        }
    }
}



void NCSG::loadNodes()
{
    std::string nodepath = BFile::FormPath(m_treedir, "nodes.npy") ;

    m_nodes = NPY<float>::load(nodepath.c_str());

    if(!m_nodes)
    {
         LOG(fatal) << "NCSG::loadNodes"
                    << " failed to load " 
                    << " nodepath [" << nodepath << "]"
                    ;
    }

    assert(m_nodes);



    m_num_nodes  = m_nodes->getShape(0) ;  
    unsigned nj = m_nodes->getShape(1);
    unsigned nk = m_nodes->getShape(2);

    assert( nj == NJ );
    assert( nk == NK );

    m_height = UINT_MAX ; 
    int h = MAX_HEIGHT*2 ;   // <-- dont let exceeding MAXHEIGHT, mess up determination of height 


    while(h--)
    {
        unsigned complete_nodes = TREE_NODES(h) ;
        if(complete_nodes == m_num_nodes) m_height = h ; 
    }

    bool invalid_height = m_height == UINT_MAX ; 

    if(invalid_height)
    {
        LOG(fatal) << "NCSG::loadNodes"
                   << " INVALID_HEIGHT "
                   << " m_nodes " << m_nodes->getShapeString()
                   << " num_nodes " << m_num_nodes
                   << " MAX_HEIGHT " << MAX_HEIGHT
                   ;

    }

    assert(!invalid_height); // must be complete binary tree sized 1, 3, 7, 15, 31, ...





}


void NCSG::loadTransforms()
{
    std::string tranpath = BFile::FormPath(m_treedir, "transforms.npy") ;
    if(!BFile::ExistsFile(tranpath.c_str())) return ; 

    NPY<float>* src = NPY<float>::load(tranpath.c_str());
    assert(src); 
    bool valid_src = src->hasShape(-1,4,4) ;
    if(!valid_src) 
    {
        LOG(fatal) << "NCSG::loadTransforms"
                   << " invalid src transforms "
                   << " tranpath " << tranpath
                   << " src_sh " << src->getShapeString()
                   ;
    }

    assert(valid_src);
    unsigned ni = src->getShape(0) ;

              
    assert(NTRAN == 2 || NTRAN == 3);

    NPY<float>* transforms = NTRAN == 2 ? NPY<float>::make_paired_transforms(src) : NPY<float>::make_triple_transforms(src) ;
    assert(transforms->hasShape(ni,NTRAN,4,4));

    m_transforms = transforms ; 
    m_gtransforms = NPY<float>::make(0,NTRAN,4,4) ;  // for collecting unique gtransforms

    m_num_transforms  = ni  ;  

    LOG(trace) << "NCSG::loadTransforms"
              << " tranpath " << tranpath 
              << " num_transforms " << ni
              ;

}




void NCSG::loadSrcVerts()
{
    std::string path = BFile::FormPath(m_treedir,  SRC_VERTS ) ;
    if(!BFile::ExistsFile(path.c_str())) return ; 

    NPY<float>* a = NPY<float>::load(path.c_str());
    assert(a); 
    bool valid = a->hasShape(-1,3) ;
    if(!valid) 
    {
        LOG(fatal) << "NCSG::loadVerts"
                   << " invalid verts  "
                   << " path " << path
                   << " shape " << a->getShapeString()
                   ;
    }

    assert(valid);
    m_srcverts = a ; 
    m_num_srcverts = a->getShape(0) ; 

    LOG(info) << " loaded " << m_num_srcverts ;
}

void NCSG::loadSrcFaces()
{
    std::string path = BFile::FormPath(m_treedir,  SRC_FACES ) ;
    if(!BFile::ExistsFile(path.c_str())) return ; 

    NPY<int>* a = NPY<int>::load(path.c_str());
    assert(a); 
    bool valid = a->hasShape(-1,4) ;
    if(!valid) 
    {
        LOG(fatal) << "NCSG::loadFaces"
                   << " invalid faces  "
                   << " path " << path
                   << " shape " << a->getShapeString()
                   ;
    }
    assert(valid);
    m_srcfaces = a ; 
    m_num_srcfaces = a->getShape(0) ; 

    LOG(info) << " loaded " << m_num_srcfaces ;

}


void NCSG::loadPlanes()
{
    std::string path = BFile::FormPath(m_treedir,  PLANES ) ;
    if(!BFile::ExistsFile(path.c_str())) return ; 

    NPY<float>* a = NPY<float>::load(path.c_str());
    assert(a); 
    bool valid = a->hasShape(-1,4) ;
    if(!valid) 
    {
        LOG(fatal) << "NCSG::loadPlanes"
                   << " invalid planes  "
                   << " path " << path
                   << " shape " << a->getShapeString()
                   ;
    }
    assert(valid);

    m_planes = a ; 
    m_num_planes = a->getShape(0) ; 
}

void NCSG::load()
{
    if(m_index % 100 == 0 && m_verbosity > 0)
    {
    LOG(info) << "NCSG::load " 
              << " index " << m_index
              << " treedir " << m_treedir 
               ; 
    }

    loadMetadata();
    loadNodes();
    loadNodeMetadata();
    LOG(debug) << "NCSG::load MIDDLE " ; 
    loadTransforms();
    loadPlanes();

    loadSrcVerts();
    loadSrcFaces();


    LOG(debug) << "NCSG::load DONE " ; 
}



unsigned NCSG::NumNodes(unsigned height)
{
   return TREE_NODES(height);
}
nnode* NCSG::getRoot() const 
{
    return m_root ; 
}
OpticksCSG_t NCSG::getRootType() const 
{
    assert(m_root);
    return m_root->type ; 
}
unsigned NCSG::getHeight() const 
{
    return m_height ; 
}
unsigned NCSG::getNumNodes() const 
{
    return m_num_nodes ; 
}

NPY<float>* NCSG::getNodeBuffer()
{
    return m_nodes ; 
}
NPY<float>* NCSG::getTransformBuffer()
{
    return m_transforms ; 
}

NPY<float>* NCSG::getGTransformBuffer()
{
    return m_gtransforms ; 
}
NPY<float>* NCSG::getPlaneBuffer()
{
    return m_planes ; 
}








NParameters* NCSG::getMetaParameters() 
{
    return m_meta ; 
}





const char* NCSG::getBoundary() const 
{
    return m_boundary ; 
}
const char* NCSG::getTreeDir() const 
{
    return m_treedir ; 
}

const char* NCSG::getTreeName() const 
{
    std::string name = BFile::Name(m_treedir ? m_treedir : "-1") ; 
    return strdup(name.c_str());
}
int NCSG::getTreeNameIdx() const 
{
    const char* name = getTreeName();
    return BStr::atoi(name, -1);
}



unsigned NCSG::getIndex() const 
{
    return m_index ; 
}
int NCSG::getVerbosity() const 
{
    return m_verbosity ; 
}
bool NCSG::isContainer() const 
{
    return m_container > 0  ; 
}
float NCSG::getContainerScale() const 
{
    return m_containerscale  ; 
}
bool NCSG::isUsedGlobally() const 
{
    return m_usedglobally ; 
}







void NCSG::setIndex(unsigned index)
{
    m_index = index ; 
}
void NCSG::setVerbosity(int verbosity)
{
    m_verbosity = verbosity ; 
}
void NCSG::setIsUsedGlobally(bool usedglobally )
{
    m_usedglobally = usedglobally ; 
}


void NCSG::setBoundary(const char* boundary)
{
    m_boundary = boundary ? strdup(boundary) : NULL ; 
}

void NCSG::setConfig(const NSceneConfig* config)
{
    m_config = config ; 
}



unsigned NCSG::getTypeCode(unsigned idx)
{
    return m_nodes->getUInt(idx,TYPECODE_J,TYPECODE_K,0u);
}


unsigned NCSG::getTransformIndex(unsigned idx)
{
    unsigned raw = m_nodes->getUInt(idx,TRANSFORM_J,TRANSFORM_K,0u);
    return raw & SSys::OTHERBIT32 ;   // <-- strip the sign bit  
}

bool NCSG::isComplement(unsigned idx)
{
    unsigned raw = m_nodes->getUInt(idx,TRANSFORM_J,TRANSFORM_K,0u);
    return raw & SSys::SIGNBIT32 ; 
}



nquad NCSG::getQuad(unsigned idx, unsigned j)
{
    nquad qj ; 
    qj.f = m_nodes->getVQuad(idx, j) ;
    return qj ;
}

void NCSG::import()
{
    if(m_verbosity > 1)
    {
    LOG(info) << "NCSG::import START" 
              << " verbosity " << m_verbosity
              << " treedir " << m_treedir
              << " smry " << smry()
              ; 
    }

    assert(m_nodes);
    if(m_verbosity > 0)
    {
    LOG(info) << "NCSG::import"
              << " importing buffer into CSG node tree "
              << " num_nodes " << m_num_nodes
              << " height " << m_height 
              ;
    }

    m_root = import_r(0, NULL) ; 
    m_root->set_treedir(m_treedir) ; 
    m_root->set_treeidx(getTreeNameIdx()) ; 

    postimport();

    if(m_verbosity > 5)
    check();  // recursive transform dumping 

    if(m_verbosity > 1)
    {
    LOG(info) << "NCSG::import DONE " ; 
    } 
}


unsigned NCSG::get_num_coincidence() const 
{
   assert(m_nudger);
   return m_nudger->get_num_coincidence() ; 
}
std::string NCSG::desc_coincidence() const 
{
   assert(m_nudger);
   return m_nudger->desc_coincidence() ; 
}

std::string NCSG::get_type_mask_string() const 
{
   assert(m_root);
   return m_root->get_type_mask_string() ;
}
unsigned NCSG::get_type_mask() const 
{
   assert(m_root);
   return m_root->get_type_mask() ;
}
unsigned NCSG::get_oper_mask() const 
{
   assert(m_root);
   return m_root->get_oper_mask() ;
}
unsigned NCSG::get_prim_mask() const 
{
   assert(m_root);
   return m_root->get_prim_mask() ;
}



void NCSG::postimport()
{
    m_nudger = make_nudger() ; 

    //m_uncoincide = make_uncoincide(); 
    //m_uncoincide->uncoincide();

    //postimport_autoscan();
}


void NCSG::postimport_autoscan()
{

   float mmstep = 0.1f ; 
    NScan scan(*m_root, m_verbosity);
    unsigned nzero = scan.autoscan(mmstep);
    const std::string& msg = scan.get_message();

    bool even_crossings = nzero % 2 == 0 ; 

    if( !msg.empty() )
    {
        LOG(warning) << "NCSG::postimport_autoscan"
                     << " autoscan message " << msg 
                     ;
    }


    if( !even_crossings )
    {
        LOG(warning) << "NCSG::postimport_autoscan"
                     << " autoscan odd crossings "
                     << " nzero " << nzero 
                     ;
    }

    //LOG(info) << "NCSG::postimport" ;
    std::cout 
         << "NCSG::postimport_autoscan"
         << " nzero " << std::setw(4) << nzero 
         << " NScanTest " << std::left << std::setw(40) << getTreeDir()  << std::right
         << " soname " << std::setw(40) << soname()  
         << " tag " << std::setw(10) << m_root->tag()
         << " nprim " << std::setw(4) << m_root->get_num_prim()
         << " typ " << std::setw(20)  << m_root->get_type_mask_string()
         << " msg " << scan.get_message()
         << std::endl 
         ;

}







nmat4pair* NCSG::import_transform_pair(unsigned itra)
{
    // itra is a 1-based index, with 0 meaning None

    assert(NTRAN == 2);
    if(itra == 0 || m_transforms == NULL ) return NULL ; 

    unsigned idx = itra - 1 ; 

    assert( idx < m_num_transforms );
    assert(m_transforms->hasShape(-1,NTRAN,4,4));

    nmat4pair* pair = m_transforms->getMat4PairPtr(idx);

    return pair ; 
}


nmat4triple* NCSG::import_transform_triple(unsigned itra)
{
    // itra is a 1-based index, with 0 meaning None

    assert(NTRAN == 3);
    if(itra == 0 || m_transforms == NULL ) return NULL ; 

    unsigned idx = itra - 1 ; 

    assert( idx < m_num_transforms );
    assert(m_transforms->hasShape(-1,NTRAN,4,4));

    nmat4triple* triple = m_transforms->getMat4TriplePtr(idx);

    return triple ; 
}



nnode* NCSG::import_r(unsigned idx, nnode* parent)
{
    if(idx >= m_num_nodes) return NULL ; 
        
    OpticksCSG_t typecode = (OpticksCSG_t)getTypeCode(idx);      
    int transform_idx = getTransformIndex(idx) ; 
    bool complement = isComplement(idx) ; 

    LOG(debug) << "NCSG::import_r"
              << " idx " << idx
              << " transform_idx " << transform_idx
              << " complement " << complement 
              ;
 
    nnode* node = NULL ;   
 
    if(typecode == CSG_UNION || typecode == CSG_INTERSECTION || typecode == CSG_DIFFERENCE)
    {
        node = import_operator( idx, typecode ) ; 

        node->parent = parent ; 
        node->idx = idx ; 
        node->complement = complement ; 

        node->transform = import_transform_triple( transform_idx ) ;

        node->left = import_r(idx*2+1, node );  
        node->right = import_r(idx*2+2, node );

        node->left->other = node->right ;   // used by NOpenMesh 
        node->right->other = node->left ; 

        // recursive calls after "visit" as full ancestry needed for transform collection once reach primitives
    }
    else 
    {
        node = import_primitive( idx, typecode ); 

        node->parent = parent ;                // <-- parent hookup needed prior to gtransform collection 
        node->idx = idx ; 
        node->complement = complement ; 

        node->transform = import_transform_triple( transform_idx ) ;

        const nmat4triple* gtransform = node->global_transform();   

        // see opticks/notes/issues/subtree_instances_missing_transform.rst
        //if(gtransform == NULL && m_usedglobally)
        if(gtransform == NULL )  // move to giving all primitives a gtransform 
        {
            gtransform = nmat4triple::make_identity() ;
        }

        unsigned gtransform_idx = gtransform ? addUniqueTransform(gtransform) : 0 ; 

        node->gtransform = gtransform ; 
        node->gtransform_idx = gtransform_idx ; // 1-based, 0 for None
    }
    assert(node); 

    NParameters* nodemeta = getNodeMetadata(idx);
    if(nodemeta) node->meta = nodemeta ; 

    // Avoiding duplication between the operator and primitive branches 
    // in the above is not sufficient reason to put things here, so very late.
    return node ; 
} 


nnode* NCSG::import_operator( unsigned idx, OpticksCSG_t typecode )
{
    if(m_verbosity > 2)
    {
    LOG(info) << "NCSG::import_operator " 
              << " idx " << idx 
              << " typecode " << typecode 
              << " csgname " << CSGName(typecode) 
              ;
    }

    nnode* node = NULL ;   
    switch(typecode)
    {
       case CSG_UNION:        node = new nunion(make_union(NULL, NULL )) ; break ; 
       case CSG_INTERSECTION: node = new nintersection(make_intersection(NULL, NULL )) ; break ; 
       case CSG_DIFFERENCE:   node = new ndifference(make_difference(NULL, NULL ))   ; break ; 
       default:               node = NULL                                 ; break ; 
    }
    assert(node);
    return node ; 
}

nnode* NCSG::import_primitive( unsigned idx, OpticksCSG_t typecode )
{
    nquad p0 = getQuad(idx, 0);
    nquad p1 = getQuad(idx, 1);
    nquad p2 = getQuad(idx, 2);
    nquad p3 = getQuad(idx, 3);

    if(m_verbosity > 2)
    {
    LOG(info) << "NCSG::import_primitive  " 
              << " idx " << idx 
              << " typecode " << typecode 
              << " csgname " << CSGName(typecode) 
              ;
    }

    nnode* node = NULL ;   
    switch(typecode)
    {
       case CSG_SPHERE:         node = new nsphere(make_sphere(p0))           ; break ; 
       case CSG_ZSPHERE:        node = new nzsphere(make_zsphere(p0,p1,p2))   ; break ; 
       case CSG_BOX:            node = new nbox(make_box(p0))                 ; break ; 
       case CSG_BOX3:           node = new nbox(make_box3(p0))                ; break ; 
       case CSG_SLAB:           node = new nslab(make_slab(p0, p1))           ; break ; 
       case CSG_PLANE:          node = new nplane(make_plane(p0))             ; break ; 
       case CSG_CYLINDER:       node = new ncylinder(make_cylinder(p0, p1))   ; break ; 
       case CSG_DISC:           node = new ndisc(make_disc(p0, p1))           ; break ; 
       case CSG_CONE:           node = new ncone(make_cone(p0))               ; break ; 
       case CSG_TORUS:          node = new ntorus(make_torus(p0))             ; break ; 
       case CSG_CUBIC:          node = new ncubic(make_cubic(p0,p1))          ; break ; 
       case CSG_HYPERBOLOID:    node = new nhyperboloid(make_hyperboloid(p0)) ; break ; 
       case CSG_TRAPEZOID:  
       case CSG_SEGMENT:  
       case CSG_CONVEXPOLYHEDRON:  
                                node = new nconvexpolyhedron(make_convexpolyhedron(p0,p1,p2,p3))   ; break ; 

       case CSG_ELLIPSOID: assert(0 && "ellipsoid should be zsphere at this level" )   ; break ; 
       default:           node = NULL ; break ; 
    }       


    if(node == NULL) 
    {
            LOG(fatal) << "NCSG::import_primitive"
                       << " TYPECODE NOT IMPLEMENTED " 
                       << " idx " << idx 
                       << " typecode " << typecode
                       << " csgname " << CSGName(typecode)
                       ;
    } 

    assert(node); 

    if(CSGHasPlanes(typecode)) 
    {
        import_planes( node );
        import_srcvertsfaces( node );
    }

    if(m_verbosity > 3)
    {
    LOG(info) << "NCSG::import_primitive  " 
              << " idx " << idx 
              << " typecode " << typecode 
              << " csgname " << CSGName(typecode) 
              << " DONE " 
              ;
    } 

    

    return node ; 
}


void NCSG::import_srcvertsfaces(nnode* node)
{
    assert( node->has_planes() );

    if(!m_srcverts || !m_srcfaces) 
    {
        LOG(debug) << "NCSG::import_srcvertsfaces no m_srcverts  m_srcfaces " ; 
        return ; 
    }

    //assert(m_srcverts);
    //assert(m_srcfaces);

    nconvexpolyhedron* cpol = dynamic_cast<nconvexpolyhedron*>(node);
    assert(cpol);

    std::vector<glm::vec3> _verts ;  
    std::vector<glm::ivec4> _faces ;  

    m_srcverts->copyTo(_verts);
    m_srcfaces->copyTo(_faces);

    cpol->set_srcvertsfaces(_verts, _faces);     
}


void NCSG::import_planes(nnode* node)
{
    assert( node->has_planes() );

    nconvexpolyhedron* cpol = dynamic_cast<nconvexpolyhedron*>(node);
    assert(cpol);


    unsigned iplane = node->planeIdx() ;
    unsigned num_plane = node->planeNum() ;
    unsigned idx = iplane - 1 ; 

    if(m_verbosity > 3)
    {
    LOG(info) << "NCSG::import_planes"
              << " iplane " << iplane
              << " num_plane " << num_plane
              ;
    }

    assert( idx < m_num_planes );
    assert( idx + num_plane - 1 < m_num_planes );
    assert( m_planes->hasShape(-1,4) );
    assert( node->planes.size() == 0u );


    std::vector<glm::vec4> _planes ;  

    for(unsigned i=idx ; i < idx + num_plane ; i++)
    {
        glm::vec4 plane = m_planes->getQuad(i) ;

        _planes.push_back(plane);    

        if(m_verbosity > 3)
        std::cout << " plane " << std::setw(3) << i 
                  << gpresent(plane)
                  << std::endl ; 

    }
    cpol->set_planes(_planes);     

    assert( cpol->planes.size() == num_plane );
}


unsigned NCSG::addUniqueTransform( const nmat4triple* gtransform_ )
{
    bool no_offset = m_gpuoffset.x == 0.f && m_gpuoffset.y == 0.f && m_gpuoffset.z == 0.f ;

    bool reverse = true ; // <-- apply transfrom at root of transform hierarchy (rather than leaf)

    const nmat4triple* gtransform = no_offset ? gtransform_ : gtransform_->make_translated(m_gpuoffset, reverse, "NCSG::addUniqueTransform" ) ;


    /*
    std::cout << "NCSG::addUniqueTransform"
              << " orig " << *gtransform_
              << " tlated " << *gtransform
              << " gpuoffset " << m_gpuoffset 
              << std::endl 
              ;
    */

    NPY<float>* gtmp = NPY<float>::make(1,NTRAN,4,4);
    gtmp->zero();
    gtmp->setMat4Triple( gtransform, 0);

    unsigned gtransform_idx = 1 + m_gtransforms->addItemUnique( gtmp, 0 ) ; 
    delete gtmp ; 
    return gtransform_idx ; 
}


void NCSG::check()
{
    check_r( m_root );

    unsigned ni =  m_gtransforms ? m_gtransforms->getNumItems() : 0  ;

    if(m_verbosity > 1)
    {
    LOG(info) << "NCSG::check"
              << " unique gtransforms " << ni
              ;
    }

    for(unsigned i=0 ; i < ni ; i++)
    {
        const nmat4triple* u_gtran = m_gtransforms->getMat4TriplePtr(i);
        std::cout 
                  << "[" << std::setw(2) << i << "] " 
                  << *u_gtran 
                  << std::endl ; 
    }
}

void NCSG::check_r(nnode* node)
{

    if(m_verbosity > 2)
    {
        if(node->gtransform)
        {
            std::cout << "NCSG::check_r"
                      << " gtransform_idx " << node->gtransform_idx
                      << std::endl 
                      ;
        }
        if(node->transform)
        {
            std::cout << "NCSG::check_r"
                      << " transform " << *node->transform
                      << std::endl 
                      ;
        }
    }


    if(node->left && node->right)
    {
        check_r(node->left);
        check_r(node->right);
    }
}




void NCSG::export_()
{
    assert(m_nodes);
    LOG(debug) << "NCSG::export_ "
              << " exporting CSG node tree into buffer "
              << " num_nodes " << m_num_nodes
              << " height " << m_height 
              ;
    export_r(m_root, 0);
}

void NCSG::export_r(nnode* node, unsigned idx)
{
    assert(idx < m_num_nodes); 
    LOG(trace) << "NCSG::export_r"
              << " idx " << idx 
              << node->desc()
              ;

    // crucial 2-step here, where m_nodes gets totally rewritten
    npart pt = node->part();
    m_nodes->setPart( pt, idx);  // writes 4 quads to buffer

    if(node->left && node->right)
    {
        export_r(node->left,  2*idx + 1);
        export_r(node->right, 2*idx + 2);
    }  
}


void NCSG::dump(const char* msg)
{
    LOG(info) << msg  ; 
    std::cout << brief() << std::endl ; 

    if(!m_root) return ;

    unsigned nsp = getNumSurfacePoints();
    if( nsp > 0)
    {
        nbbox bbsp = bbox_surface_points();
        std::cout << " bbsp " << bbsp.desc() << std::endl ; 
    }

    m_root->dump("NCSG::dump");   

    if(m_meta)
    m_meta->dump(); 
}

void NCSG::dump_surface_points(const char* msg, unsigned dmax) const 
{
    if(!m_points) return ;
    m_points->dump(msg, dmax );
}





std::string NCSG::brief() const 
{
    std::stringstream ss ; 
    ss << " NCSG " 
       << " ix " << std::setw(4) << m_index
       << " surfpoints " << std::setw(4) << getNumSurfacePoints() 
       << " so " << std::setw(40) << std::left << soname()
       << " lv " << std::setw(40) << std::left << lvname()
       ;

    return ss.str();  
}

std::string NCSG::desc()
{
    std::string node_sh = m_nodes ? m_nodes->getShapeString() : "-" ;    
    std::string tran_sh = m_transforms ? m_transforms->getShapeString() : "-" ;    
    std::stringstream ss ; 
    ss << "NCSG " 
       << " index " << m_index
       << " treedir " << ( m_treedir ? m_treedir : "NULL" ) 
       << " node_sh " << node_sh  
       << " tran_sh " << tran_sh  
       << " boundary " << ( m_boundary ? m_boundary : "NULL" ) 
       << " meta " << m_meta->desc()
       ;
    return ss.str();  
}

 


NCSG* NCSG::LoadCSG(const char* treedir, const char* gltfconfig)
{
    if(!Exists(treedir))
    {
         LOG(warning) << "NCSG::LoadCSG no such dir OR does not contain tree " << treedir ;
         return NULL ; 
    }
    NSceneConfig* config = new NSceneConfig(gltfconfig) ; 

    int verbosity = SSys::getenvint("VERBOSITY", 1) ;
    if(verbosity != config->verbosity) 
    {
        LOG(info) << "NCSG::LoadCSG"
                  << " setting verbosity from envvar " 
                  << verbosity 
                  ;   
         config->verbosity = verbosity ; 
    }


    NCSG* csg = NCSG::LoadTree(treedir, config);
    assert(csg);
    return csg ; 
}


NCSG* NCSG::LoadTree(const char* treedir, const NSceneConfig* config  )
{
    if(!Exists(treedir) )
    {
         LOG(warning) << "NCSG::LoadTree no such treedir OR does not contain tree " 
                      << " treedir: " << treedir 
                      << " FILENAME: " << FILENAME 
                      ;

         return NULL ; 
    }
 

    NCSG* tree = new NCSG(treedir) ; 

    tree->setConfig(config);
    tree->setVerbosity(config->verbosity);
    tree->setIsUsedGlobally(true);

    tree->load();
    tree->import();
    tree->export_();

    if(config->verbosity > 1) tree->dump("NCSG::LoadTree");

    if(config->polygonize)
    tree->polygonize();

    tree->collect_surface_points();

    return tree ; 

}

NCSG* NCSG::FromNode(nnode* root, const NSceneConfig* config)
{
    assert( root->boundary && "must root->set_boundary(spec) first" );

    NCSG* tree = new NCSG(root);

    tree->setConfig(config);
    tree->export_();
    assert( tree->getGTransformBuffer() );

    tree->collect_surface_points();

    return tree ; 
}
 

NTrianglesNPY* NCSG::polygonize()
{
    if(m_tris == NULL)
    {
        if(m_verbosity > 1)
        {
        LOG(info) << "NCSG::polygonize START"
                  << " verbosity " << m_verbosity 
                  << " treedir " << m_treedir
                  ; 
        }

        NPolygonizer pg(this);
        m_tris = pg.polygonize();

        if(m_verbosity > 1)
        {
        LOG(info) << "NCSG::polygonize DONE" 
                  << " verbosity " << m_verbosity 
                  << " treedir " << m_treedir
                  ; 
        }

    }
    return m_tris ; 
}

NTrianglesNPY* NCSG::getTris()
{
    return m_tris ; 
}

unsigned NCSG::getNumTriangles()
{
    return m_tris ? m_tris->getNumTriangles() : 0 ; 
}



glm::uvec4 NCSG::collect_surface_points() 
{
    //LOG(info) << "NCSG::collect_surface_points START " << brief() ; 

    if(!m_points) 
    {
        //LOG(info) << "NCSG::collect_surface_points points ctor " ; 
        m_points = new NNodePoints(m_root, m_config );
    } 

    glm::uvec4 tots = m_points->collect_surface_points();


    //LOG(info) << "NCSG::collect_surface_points DONE " << m_points->desc()  ; 
    return tots ; 
}





nbbox NCSG::bbox_analytic() const 
{
    assert(m_root);
    return m_root->bbox();
}

nbbox NCSG::bbox_surface_points() const 
{
    assert(m_points); 
    return m_points->bbox_surface_points();
}


const std::vector<glm::vec3>& NCSG::getSurfacePoints() const 
{
    assert(m_points); 
    return m_points->getCompositePoints();
}
unsigned NCSG::getNumSurfacePoints() const 
{
    return m_points ? m_points->getNumCompositePoints() : 0 ;
}
float NCSG::getSurfaceEpsilon() const 
{
    return m_points ? m_points->getEpsilon() : -1.f ;
}





void NCSG::adjustToFit( const nbbox& container, float scale, float delta ) const 
{
    LOG(debug) << "NCSG::adjustToFit START " ; 

    nnode* root = getRoot();

    nbbox root_bb = root->bbox();
 
    nnode::AdjustToFit(root, container, scale, delta );         

    LOG(debug) << "NCSG::updateContainer DONE"
              << " root_bb " << root_bb.desc()
              << " container " << container.desc()
              ;
}


