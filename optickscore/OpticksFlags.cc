
#include <map>
#include <vector>
#include <string>

#include "BBit.hh"
#include "BRegex.hh"
#include "PLOG.hh"

#include "Index.hpp"

#include "OpticksFlags.hh"


//const char* OpticksFlags::ENUM_HEADER_PATH = "$ENV_HOME/graphics/optixrap/cu/photon.h" ;
//const char* OpticksFlags::ENUM_HEADER_PATH = "$ENV_HOME/opticks/OpticksPhoton.h" ;
//const char* OpticksFlags::ENUM_HEADER_PATH = "$ENV_HOME/optickscore/OpticksPhoton.h" ;

const char* OpticksFlags::ENUM_HEADER_PATH = "$OPTICKS_INSTALL_PREFIX/include/OpticksCore/OpticksPhoton.h" ;
//  envvar OPTICKS_INSTALL_PREFIX is set internally by OpticksResource based on cmake config 


const char* OpticksFlags::ZERO_              = "." ;
const char* OpticksFlags::NATURAL_           = "NATURAL" ;
const char* OpticksFlags::FABRICATED_        = "FABRICATED" ;
const char* OpticksFlags::MACHINERY_         = "MACHINERY" ;

const char* OpticksFlags::CERENKOV_          = "CERENKOV" ;
const char* OpticksFlags::SCINTILLATION_     = "SCINTILLATION" ;
const char* OpticksFlags::MISS_              = "MISS" ;
const char* OpticksFlags::OTHER_             = "OTHER" ;
const char* OpticksFlags::BULK_ABSORB_       = "BULK_ABSORB" ;
const char* OpticksFlags::BULK_REEMIT_       = "BULK_REEMIT" ;
const char* OpticksFlags::BULK_SCATTER_      = "BULK_SCATTER" ; 
const char* OpticksFlags::SURFACE_DETECT_    = "SURFACE_DETECT" ;
const char* OpticksFlags::SURFACE_ABSORB_    = "SURFACE_ABSORB" ; 
const char* OpticksFlags::SURFACE_DREFLECT_  = "SURFACE_DREFLECT" ; 
const char* OpticksFlags::SURFACE_SREFLECT_  = "SURFACE_SREFLECT" ; 
const char* OpticksFlags::BOUNDARY_REFLECT_  = "BOUNDARY_REFLECT" ; 
const char* OpticksFlags::BOUNDARY_TRANSMIT_ = "BOUNDARY_TRANSMIT" ; 
const char* OpticksFlags::TORCH_             = "TORCH" ; 
const char* OpticksFlags::G4GUN_             = "G4GUN" ; 
const char* OpticksFlags::NAN_ABORT_         = "NAN_ABORT" ; 
const char* OpticksFlags::BAD_FLAG_          = "BAD_FLAG" ; 

// NB this is duplicating abbrev from /usr/local/opticks/opticksdata/resource/GFlags/abbrev.json
//    TODO: get rid of that 
//
//     as these are so fixed they deserve static enshrinement for easy access from everywhere
//
const char* OpticksFlags::_ZERO              = "  " ;
const char* OpticksFlags::_NATURAL           = "NL" ;
const char* OpticksFlags::_FABRICATED        = "FD" ;
const char* OpticksFlags::_MACHINERY         = "MY" ;

const char* OpticksFlags::_CERENKOV          = "CK" ;
const char* OpticksFlags::_SCINTILLATION     = "SI" ;
const char* OpticksFlags::_TORCH             = "TO" ; 
const char* OpticksFlags::_MISS              = "MI" ;
const char* OpticksFlags::_BULK_ABSORB       = "AB" ;
const char* OpticksFlags::_BULK_REEMIT       = "RE" ;
const char* OpticksFlags::_BULK_SCATTER      = "SC" ; 
const char* OpticksFlags::_SURFACE_DETECT    = "SD" ;
const char* OpticksFlags::_SURFACE_ABSORB    = "SA" ; 
const char* OpticksFlags::_SURFACE_DREFLECT  = "DR" ; 
const char* OpticksFlags::_SURFACE_SREFLECT  = "SR" ; 
const char* OpticksFlags::_BOUNDARY_REFLECT  = "BR" ; 
const char* OpticksFlags::_BOUNDARY_TRANSMIT = "BT" ; 
const char* OpticksFlags::_NAN_ABORT         = "NA" ; 
const char* OpticksFlags::_G4GUN             = "GN" ; 
const char* OpticksFlags::_BAD_FLAG          = "XX" ; 


const char* OpticksFlags::natural_           = "natural" ;
const char* OpticksFlags::fabricated_        = "fabricated" ;
const char* OpticksFlags::machinery_         = "machinery" ;
const char* OpticksFlags::cerenkov_          = "cerenkov" ;
const char* OpticksFlags::scintillation_     = "scintillation" ;
const char* OpticksFlags::torch_             = "torch" ; 
const char* OpticksFlags::g4gun_             = "g4gun" ; 
const char* OpticksFlags::other_             = "other" ;


const char* OpticksFlags::Flag(const unsigned int flag)
{
    const char* s = 0 ; 
    switch(flag)
    {
        case 0:                s=ZERO_;break;
        case CERENKOV:         s=CERENKOV_;break;
        case SCINTILLATION:    s=SCINTILLATION_ ;break; 
        case MISS:             s=MISS_ ;break; 
        case BULK_ABSORB:      s=BULK_ABSORB_ ;break; 
        case BULK_REEMIT:      s=BULK_REEMIT_ ;break; 
        case BULK_SCATTER:     s=BULK_SCATTER_ ;break; 
        case SURFACE_DETECT:   s=SURFACE_DETECT_ ;break; 
        case SURFACE_ABSORB:   s=SURFACE_ABSORB_ ;break; 
        case SURFACE_DREFLECT: s=SURFACE_DREFLECT_ ;break; 
        case SURFACE_SREFLECT: s=SURFACE_SREFLECT_ ;break; 
        case BOUNDARY_REFLECT: s=BOUNDARY_REFLECT_ ;break; 
        case BOUNDARY_TRANSMIT:s=BOUNDARY_TRANSMIT_ ;break; 
        case TORCH:            s=TORCH_ ;break; 
        case NAN_ABORT:        s=NAN_ABORT_ ;break; 
        case G4GUN:            s=G4GUN_ ;break; 
        case NATURAL:          s=NATURAL_ ;break; 
        case FABRICATED:       s=FABRICATED_ ;break; 
        case MACHINERY:        s=MACHINERY_;break; 
        default:               s=BAD_FLAG_  ;
                               LOG(warning) << "OpticksFlags::Flag BAD_FLAG [" << flag << "]" << std::hex << flag << std::dec ;             
    }
    return s;
}



const char* OpticksFlags::Abbrev(const unsigned int flag)
{
    const char* s = 0 ; 
    switch(flag)
    {
        case 0:                s=_ZERO;break;
        case CERENKOV:         s=_CERENKOV;break;
        case SCINTILLATION:    s=_SCINTILLATION ;break; 
        case MISS:             s=_MISS ;break; 
        case BULK_ABSORB:      s=_BULK_ABSORB ;break; 
        case BULK_REEMIT:      s=_BULK_REEMIT ;break; 
        case BULK_SCATTER:     s=_BULK_SCATTER ;break; 
        case SURFACE_DETECT:   s=_SURFACE_DETECT ;break; 
        case SURFACE_ABSORB:   s=_SURFACE_ABSORB ;break; 
        case SURFACE_DREFLECT: s=_SURFACE_DREFLECT ;break; 
        case SURFACE_SREFLECT: s=_SURFACE_SREFLECT ;break; 
        case BOUNDARY_REFLECT: s=_BOUNDARY_REFLECT ;break; 
        case BOUNDARY_TRANSMIT:s=_BOUNDARY_TRANSMIT ;break; 
        case TORCH:            s=_TORCH ;break; 
        case NAN_ABORT:        s=_NAN_ABORT ;break; 
        case G4GUN:            s=_G4GUN ;break; 
        case NATURAL:          s=_NATURAL ;break; 
        case FABRICATED:       s=_FABRICATED ;break; 
        case MACHINERY:        s=_MACHINERY;break; 
        default:               s=_BAD_FLAG  ;
                               LOG(warning) << "OpticksFlags::Abbrev BAD_FLAG [" << flag << "]" << std::hex << flag << std::dec ;             
    }
    return s;
}













std::string OpticksFlags::FlagSequence(const unsigned long long seqhis, bool abbrev)
{
    std::stringstream ss ;
    assert(sizeof(unsigned long long)*8 == 16*4);
    for(unsigned int i=0 ; i < 16 ; i++)
    {
        unsigned long long f = (seqhis >> i*4) & 0xF ; 
        unsigned int flg = f == 0 ? 0 : 0x1 << (f - 1) ; 
        ss << ( abbrev ? Abbrev(flg) : Flag(flg) ) << " " ;
    }
    return ss.str();
}


std::string OpticksFlags::FlagMask(const unsigned mskhis, bool abbrev)
{
    std::vector<const char*> labels ; 

    assert( MACHINERY == 0x1 << 17 );
    unsigned lastBit = BBit::ffs(MACHINERY) - 1 ;  
    assert(lastBit == 17 ); 
 
    for(unsigned n=0 ; n <= lastBit ; n++ )
    {
        unsigned flag = 0x1 << n ; 
        if(mskhis & flag) labels.push_back( abbrev ? Abbrev(flag) : Flag(flag) );
    }
    unsigned nlab = labels.size() ; 

    std::stringstream ss ;
    for(unsigned i=0 ; i < nlab ; i++ ) ss << labels[i] << ( i < nlab - 1 ? "|" : ""  ) ; 
    return ss.str();
}


const char* OpticksFlags::SourceType( int code )
{
    const char* name = 0 ; 
    switch(code)
    {
       case NATURAL      :name = NATURAL_       ;break;
       case FABRICATED   :name = FABRICATED_    ;break;
       case MACHINERY    :name = MACHINERY_     ;break;
       case CERENKOV     :name = CERENKOV_      ;break;
       case SCINTILLATION:name = SCINTILLATION_ ;break;
       case TORCH        :name = TORCH_         ;break;
       case G4GUN        :name = G4GUN_         ;break;
       default           :name = OTHER_         ;break; 
    }
    return name ; 
}

const char* OpticksFlags::SourceTypeLowercase( int code )
{
    const char* name = 0 ; 
    switch(code)
    {
       case NATURAL      :name = natural_       ;break;
       case FABRICATED   :name = fabricated_    ;break;
       case MACHINERY    :name = machinery_     ;break;
       case CERENKOV     :name = cerenkov_      ;break;
       case SCINTILLATION:name = scintillation_ ;break;
       case TORCH        :name = torch_         ;break;
       case G4GUN        :name = g4gun_         ;break;
       default           :name = other_         ;break; 
    }
    return name ; 
}

unsigned int OpticksFlags::SourceCode(const char* type)
{
    unsigned int code = 0 ; 
    if(     strcmp(type,natural_)==0)       code = NATURAL ;
    else if(strcmp(type,fabricated_)==0)    code = FABRICATED ;
    else if(strcmp(type,machinery_)==0)     code = MACHINERY ;
    else if(strcmp(type,torch_)==0)         code = TORCH ;
    else if(strcmp(type,cerenkov_)==0)      code = CERENKOV ;
    else if(strcmp(type,scintillation_)==0) code = SCINTILLATION ;
    else if(strcmp(type,g4gun_)==0)         code = G4GUN ;
    return code ; 
}



OpticksFlags::OpticksFlags(const char* path) 
    :
    m_index(NULL)
{
    init(path);
}



Index* OpticksFlags::getIndex()
{
    return m_index ; 
} 

void OpticksFlags::init(const char* path)
{
    m_index = parseFlags(path);
    unsigned int num_flags = m_index ? m_index->getNumItems() : 0 ;

    LOG(trace) << "OpticksFlags::init"
              << " path " << path 
              << " num_flags " << num_flags 
              << " " << ( m_index ? m_index->description() : "NULL index" )
              ;
    
    assert(num_flags > 0 && "missing flags header : you need to update OpticksFlags::ENUM_HEADER_PATH ");
}


void OpticksFlags::save(const char* idpath)
{
    m_index->setExt(".ini"); 
    m_index->save(idpath);    
}

Index* OpticksFlags::parseFlags(const char* path)
{
    typedef std::pair<unsigned int, std::string>  upair_t ;
    typedef std::vector<upair_t>                  upairs_t ;
    upairs_t ups ;
    BRegex::enum_regexsearch( ups, path ); 

    const char* reldir = NULL ; 
    Index* index = new Index("GFlags", reldir);
    for(unsigned int i=0 ; i < ups.size() ; i++)
    {
        upair_t p = ups[i];
        unsigned int mask = p.first ;
        unsigned int bitpos = BBit::ffs(mask);  // first set bit, 1-based bit position
        unsigned int xmask = 1 << (bitpos-1) ; 
        assert( mask == xmask);
        index->add( p.second.c_str(), bitpos );
    }
    return index ; 
}


