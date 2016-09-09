#include <algorithm>
#include <boost/algorithm/string/replace.hpp>

#include "NGLM.hpp"
#include "NPYBase.hpp"

#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>

// sysrap-
#include "SDigest.hh"

//brap- 
#include "BStr.hh"


#include "Parameters.hpp"
#include "NPYSpec.hpp"
#include "PLOG.hh"


bool NPYBase::GLOBAL_VERBOSE = false ; 


const char* NPYBase::FLOAT_ = "FLOAT" ; 
const char* NPYBase::SHORT_ = "SHORT" ; 
const char* NPYBase::DOUBLE_ = "DOUBLE" ; 
const char* NPYBase::INT_ = "INT" ; 
const char* NPYBase::UINT_ = "UINT" ; 
const char* NPYBase::CHAR_ = "CHAR" ; 
const char* NPYBase::UCHAR_ = "UCHAR" ; 
const char* NPYBase::ULONGLONG_ = "ULONGLONG" ; 

const char* NPYBase::TypeName(Type_t type)
{
    const char* name = NULL ; 
    switch(type)
    { 
        case FLOAT:name=FLOAT_;break;
        case SHORT:name=SHORT_;break;
        case DOUBLE:name=DOUBLE_;break;
        case INT:name=INT_;break;
        case UINT:name=UINT_;break;
        case CHAR:name=CHAR_;break;
        case UCHAR:name=UCHAR_;break;
        case ULONGLONG:name=ULONGLONG_;break;
    } 
    return name ; 
}




void NPYBase::setGlobalVerbose(bool verbose)
{
    GLOBAL_VERBOSE = verbose ;
}

void NPYBase::setLookup(NLookup* lookup)
{
    m_lookup = lookup ; 
}
NLookup* NPYBase::getLookup()
{
    return m_lookup ; 
}
Parameters* NPYBase::getParameters()
{
    return m_parameters ;
}


bool NPYBase::isGenstepTranslated()
{
    return m_parameters->get<bool>("GenstepTranslated","0");  // fallback to false if not set 
}
void NPYBase::setGenstepTranslated(bool flag)
{
    m_parameters->add<bool>("GenstepTranslated", flag); 
}




NPYBase::NPYBase(std::vector<int>& shape, unsigned char sizeoftype, Type_t type, std::string& metadata, bool has_data) 
         :
         m_shape_spec(NULL),
         m_item_spec(NULL),
         m_buffer_spec(NULL),
         m_sizeoftype(sizeoftype),
         m_type(type),
         m_buffer_id(-1),
         m_buffer_target(-1),
         m_buffer_control(0),
         m_buffer_name(NULL),
         m_action_control(0),
         m_aux(NULL),
         m_verbose(false),
         m_allow_prealloc(false),
         m_shape(shape),
         m_metadata(metadata),
         m_has_data(has_data),
         m_dynamic(false),
         m_lookup(NULL),
         m_parameters(new Parameters)
{
   init();
} 

 NPYBase::~NPYBase()
{
}


 void NPYBase::setHasData(bool has_data)
{
    m_has_data = has_data ; 
}

 bool NPYBase::hasData()
{
    return m_has_data ; 
}

 NPYSpec* NPYBase::getShapeSpec()
{
    return m_shape_spec ; 
}
NPYSpec* NPYBase::getItemSpec()
{
    return m_item_spec ; 
}



// shape related

 std::vector<int>& NPYBase::getShapeVector()
{
    return m_shape ; 
}




 unsigned int NPYBase::getNumItems(int ifr, int ito)
{
    //  A) default ifr/ito  0/1 correponds to shape of 1st dimension
    //
    //  B) example ifr/ito  0/-1  gives number of items excluding last dimension 
    //               -->    0/2   --> shape(0)*shape(1)    for ndim 3 
    //
    //  C)       ifr/ito  0/3 for ndim 3   shape(0)*shape(1)*shape(2)
    //
    //  D)  ifr/ito 0/0     for any dimension
    //           -> 0/ndim     -> shape of all dimensions  
    //
    //
    int ndim = m_shape.size();
    if(ifr <  0) ifr += ndim ; 
    if(ito <= 0) ito += ndim ; 

    assert(ifr >= 0 && ifr < ndim);
    assert(ito >= 0 && ito <= ndim);

    unsigned int nit(1) ; 
    for(int i=ifr ; i < ito ; i++) nit *= getShape(i);
    return nit ;
}
 unsigned int NPYBase::getNumElements()
{
    return getShape(m_shape.size()-1);
}


 unsigned int NPYBase::getDimensions()
{
    return m_shape.size();
}
 unsigned int NPYBase::getShape(unsigned int n)
{
    return n < m_shape.size() ? m_shape[n] : 0 ;
}



// OpenGL related

 void NPYBase::setBufferId(int buffer_id)
{
    m_buffer_id = buffer_id  ;
}
 int NPYBase::getBufferId()
{
    return m_buffer_id ;
}

 void NPYBase::setBufferTarget(int buffer_target)
{
    m_buffer_target = buffer_target  ;
}
 int NPYBase::getBufferTarget()
{
    return m_buffer_target ;
}


void NPYBase::setBufferControl(unsigned long long control)
{
    m_buffer_control = control ;  
}
unsigned long long NPYBase::getBufferControl()
{
    return m_buffer_control ;  
}
unsigned long long* NPYBase::getBufferControlPtr()
{
    return &m_buffer_control ;  
}



void NPYBase::addActionControl(unsigned long long control)
{
    m_action_control |= control ;  
}
void NPYBase::setActionControl(unsigned long long control)
{
    m_action_control = control ;  
}

unsigned long long NPYBase::getActionControl()
{
    return m_action_control ;  
}
unsigned long long* NPYBase::getActionControlPtr()
{
    return &m_action_control ;  
}


void NPYBase::transfer(NPYBase* dst, NPYBase* src)
{
    NPYSpec* spec = src->getBufferSpec();

    dst->setBufferSpec(spec ? spec->clone() : NULL);
    dst->setBufferControl(src->getBufferControl());
    dst->setActionControl(src->getActionControl());
    dst->setLookup(src->getLookup());   // NB not cloning, as lookup is kinda global
}


void NPYBase::setBufferName(const char* name )
{
    m_buffer_name = name ? strdup(name) : NULL  ;  
}
void NPYBase::setBufferSpec(NPYSpec* spec)
{
    // set when OpticksEvent uses the ctor  NPY<T>::make(NPYSpec* )
    m_buffer_spec = spec ; 
    setBufferName(spec->getName());
}

const char* NPYBase::getBufferName()
{
    return m_buffer_name ;  
}
NPYSpec* NPYBase::getBufferSpec()
{
    return m_buffer_spec ; 
}









// used for CUDA OpenGL interop
 void NPYBase::setAux(void* aux)
{
    m_aux = aux ; 
}
 void* NPYBase::getAux()
{
    return m_aux ; 
}







 void NPYBase::setVerbose(bool verbose)
{
    m_verbose = verbose ; 
}
 void NPYBase::setAllowPrealloc(bool allow)
{
    m_allow_prealloc = allow ; 
}


 unsigned int NPYBase::getValueIndex(unsigned int i, unsigned int j, unsigned int k, unsigned int l)
{
    //assert(m_dim == 3 ); 
    unsigned int nj = m_nj ;
    unsigned int nk = m_nk ;
    unsigned int nl = m_nl == 0 ? 1 : m_nl ;

    return  i*nj*nk*nl + j*nk*nl + k*nl + l ;
}

 unsigned int NPYBase::getNumValues(unsigned int from_dim)
{
    unsigned int nvals = 1 ; 
    for(unsigned int i=from_dim ; i < m_shape.size() ; i++) nvals *= m_shape[i] ;
    return nvals ;  
}


// depending on sizeoftype

 unsigned char NPYBase::getSizeOfType()
{
    return m_sizeoftype;
}
 NPYBase::Type_t NPYBase::getType()
{
    return m_type;
}



 unsigned int NPYBase::getNumBytes(unsigned int from_dim)
{
    return m_sizeoftype*getNumValues(from_dim);
}
 unsigned int NPYBase::getByteIndex(unsigned int i, unsigned int j, unsigned int k, unsigned int l)
{
    return m_sizeoftype*getValueIndex(i,j,k,l);
}

 void NPYBase::setDynamic(bool dynamic)
{
    m_dynamic = dynamic ; 
}
 bool NPYBase::isDynamic()
{
    return m_dynamic ; 
}


void NPYBase::init()
{
   updateDimensions(); 
   m_shape_spec = new NPYSpec(NULL, m_ni, m_nj, m_nk, m_nl, m_type, "" ); 
   m_item_spec  = new NPYSpec(NULL,    0, m_nj, m_nk, m_nl, m_type, "" ); 
}


void NPYBase::updateDimensions()
{
    m_ni = getShape(0); 
    m_nj = getShape(1);
    m_nk = getShape(2);
    m_nl = getShape(3);  // gives 0 when beyond dimensions
    m_dim = m_shape.size();
}


unsigned int NPYBase::getNumQuads()
{
   unsigned int num_quad ;  
   unsigned int ndim = m_shape.size() ;
   unsigned int last_dimension = ndim > 1 ? m_shape[ndim-1] : 0  ;

   if(last_dimension != 4 )
   {
       LOG(fatal) << "NPYBase::getNumQuads last dim expected to be 4  " << getShapeString()  ;
       num_quad = 0 ; 
   } 
   else
   {
       num_quad = 1 ; 
       for(unsigned int i=0 ; i < ndim - 1 ; i++ ) num_quad *= m_shape[i] ; 
   } 
   return num_quad ;
}


bool NPYBase::hasSameShape(NPYBase* other)
{
    std::vector<int>& a = getShapeVector();
    std::vector<int>& b = other->getShapeVector();
    if(a.size() != b.size()) return false ; 
    unsigned int n = a.size();
    for(unsigned int i=0 ; i < n ; i++) if(a[i] != b[i]) return false ;
    return true ; 
}

bool NPYBase::hasShape(unsigned int ni, unsigned int nj, unsigned int nk, unsigned int nl)
{
    return m_ni == ni && m_nj == nj && m_nk == nk && m_nl == nl ;
}

bool NPYBase::hasItemShape(unsigned int nj, unsigned int nk, unsigned int nl)
{
    return m_nj == nj && m_nk == nk && m_nl == nl ;
}

bool NPYBase::hasItemSpec(NPYSpec* item_spec)
{
    return m_item_spec->isEqualTo(item_spec); 
}

bool NPYBase::hasShapeSpec(NPYSpec* shape_spec)
{
    return m_shape_spec->isEqualTo(shape_spec); 
}





void NPYBase::setNumItems(unsigned int ni)
{
    unsigned int orig = m_shape[0] ;
    //assert(ni >= orig);

    if(ni >= orig)
    {
       LOG(debug) << "NPYBase::setNumItems"
                  << " increase from " << orig << " to " << ni 
                  ; 
    }
    else
    {
       LOG(warning) << "NPYBase::setNumItems"
                  << " decrease from " << orig << " to " << ni 
                  ; 
    }
  
    m_shape[0] = ni ; 
    m_ni = ni ; 
}





void NPYBase::reshape(int ni_, unsigned int nj, unsigned int nk, unsigned int nl)
{
    unsigned int nvals = std::max(1u,m_ni)*std::max(1u,m_nj)*std::max(1u,m_nk)*std::max(1u,m_nl) ; 
    unsigned int njkl  = std::max(1u,nj)*std::max(1u,nk)*std::max(1u,nl) ;
    unsigned int ni    = ni_ < 0 ? nvals/njkl : ni_ ;    // auto resizing of 1st dimension, when -ve

    unsigned int nvals2 = std::max(1u,ni)*std::max(1u,nj)*std::max(1u,nk)*std::max(1u,nl) ; 

    if(nvals != nvals2) LOG(fatal) << "NPYBase::reshape INVALID AS CHANGES COUNTS " 
                              << " nvals " << nvals
                              << " nvals2 " << nvals2
                              ;

    assert(nvals == nvals2 && "NPYBase::reshape cannot change number of values, just their addressing");

    LOG(debug) << "NPYBase::reshape (0 means no-dimension) "
              << "(" << m_ni << "," << m_nj << "," << m_nk << "," << m_nl << ")"
              << " --> "
              << "(" <<   ni << "," <<   nj << "," <<   nk << "," <<   nl << ")"
              ;

    m_shape.clear();
    if(ni > 0) m_shape.push_back(ni);
    if(nj > 0) m_shape.push_back(nj);
    if(nk > 0) m_shape.push_back(nk);
    if(nl > 0) m_shape.push_back(nl);

    updateDimensions();
}



std::string NPYBase::getDigestString()
{
    return getDigestString(getBytes(), getNumBytes(0));
}

std::string NPYBase::getDigestString(void* bytes, unsigned int nbytes)
{
    return SDigest::digest(bytes, nbytes);
}

bool NPYBase::isEqualTo(NPYBase* other)
{
    return isEqualTo(other->getBytes(), other->getNumBytes(0));
}

bool NPYBase::isEqualTo(void* bytes, unsigned int nbytes)
{
    std::string self = getDigestString();
    std::string other = getDigestString(bytes, nbytes);

    bool same = self.compare(other) == 0 ; 

    if(!same)
         LOG(warning) << "NPYBase::isEqualTo NO "
                      << " self " << self 
                      << " other " << other
                      ;
 

    return same ; 
}




std::string NPYBase::getShapeString(unsigned int ifr)
{
    return getItemShape(ifr);
}

std::string NPYBase::getItemShape(unsigned int ifr)
{
    std::stringstream ss ; 
    for(size_t i=ifr ; i < m_shape.size() ; i++)
    {
        ss << m_shape[i]  ;
        if( i < m_shape.size() - 1) ss << "," ;
    }
    return ss.str(); 
}


void NPYBase::Summary(const char* msg)
{
    std::string desc = description(msg);
    LOG(info) << desc ; 
}   

std::string NPYBase::description(const char* msg)
{
    std::stringstream ss ; 

    ss << msg << " (" ;

    for(size_t i=0 ; i < m_shape.size() ; i++)
    {
        ss << m_shape[i]  ;
        if( i < m_shape.size() - 1) ss << "," ;
    }
    ss << ") " ;

    //ss << " ni " << m_ni ;
    //ss << " nj " << m_nj ;
    //ss << " nk " << m_nk ;
    //ss << " nl " << m_nl ;

    ss << " NumBytes(0) " << getNumBytes(0) ;
    ss << " NumBytes(1) " << getNumBytes(1) ;
    ss << " NumValues(0) " << getNumValues(0) ;
    ss << " NumValues(1) " << getNumValues(1) ;

    ss << m_metadata  ;

    return ss.str();
}




