#include "BStr.hh"
#include "Opticks.hh"
#include "OpticksFlags.hh"
#include "Index.hpp"
#include "Types.hpp"

#include "OKCORE_LOG.hh"
#include "PLOG.hh"


void test_ctor()
{
    OpticksFlags f ; 
    Index* i = f.getIndex();
    i->dump("test_ctor");
}

void test_FlagMask()
{
    unsigned msk = 0x1890 ;
    LOG(info) << std::setw(10) << std::hex << msk << std::dec 
              << " flagmask(abbrev) " << OpticksFlags::FlagMask(msk, true) 
              << " flagmask " << OpticksFlags::FlagMask(msk, false)
              ; 
}

void test_AbbrevToFlag()
{
    LOG(info) << "test_AbbrevToFlag" ; 
    for(unsigned f=0 ; f < 32 ; f++ )
    {
        unsigned flag = OpticksFlags::EnumFlag(f); 
        const char* abbrev = OpticksFlags::Abbrev(flag) ; 
        unsigned flag2 = OpticksFlags::AbbrevToFlag( abbrev );
        unsigned f2 = OpticksFlags::BitPos(flag2) ;

        std::cout
              << " f " << std::setw(4) << f
              << " f2 " << std::setw(4) << f2
              << " flag " << std::setw(10) << flag 
              << " flag2 " << std::setw(10) << flag2 
              << " abbrev " << std::setw(3) << abbrev
              << std::endl 
              ; 

        if(strcmp(abbrev, OpticksFlags::_BAD_FLAG) == 0) break ; 
         assert( flag2 == flag ); 
        // assert( f2 == f ); 

    }
        

    unsigned flag_non_existing = OpticksFlags::AbbrevToFlag("ZZ") ; 
    assert( flag_non_existing == 0 );
    unsigned flag_NULL = OpticksFlags::AbbrevToFlag(NULL) ; 
    assert( flag_NULL == 0 );


}

void test_AbbrevToFlagSequence(const char* abbseq)
{
    unsigned long long seqhis = OpticksFlags::AbbrevToFlagSequence(abbseq);
    std::string abbseq2 = OpticksFlags::FlagSequence( seqhis ); 
    BStr::rtrim(abbseq2);

    bool match = abbseq2.compare(abbseq) == 0 ;

    LOG(match ? info : fatal) 
           << " abbseq [" << abbseq << "]"
           << " seqhis " << std::setw(16) << std::hex << seqhis << std::dec 
           << " abbseq2 [" << abbseq2 << "]"
           ;
            
    assert(match);
}

void test_AbbrevToFlagSequence()
{
    test_AbbrevToFlagSequence("TO SR SA");
    test_AbbrevToFlagSequence("TO SC SR SA");
    //test_AbbrevToFlagSequence("TO ZZ SC SR SA");
}



void test_AbbrevToFlagValSequence(const char* seqmap, const char* x_abbseq, unsigned long long x_seqval)
{
    unsigned long long seqhis(0ull) ; 
    unsigned long long seqval(0ull) ; 

    OpticksFlags::AbbrevToFlagValSequence(seqhis, seqval, seqmap );

    unsigned long long x_seqhis = OpticksFlags::AbbrevToFlagSequence(x_abbseq) ; 

    bool seqhis_match = seqhis == x_seqhis  ;
    bool seqval_match = seqval == x_seqval  ;

    LOG( seqhis_match ? info : fatal )
            << " seqmap " << std::setw(20) << seqmap 
            << " seqhis " << std::setw(16) << std::hex << seqhis << std::dec 
            << " x_seqhis " << std::setw(16) << std::hex << x_seqhis << std::dec 
            << " x_abbseq " << x_abbseq 
            ;

    LOG( seqval_match ? info : fatal )
            << " seqmap " << std::setw(20) << seqmap 
            << " seqval " << std::setw(16) << std::hex << seqval << std::dec 
            << " x_seqval " << std::setw(16) << std::hex << x_seqval << std::dec 
            ;
 
    assert( seqhis_match ) ;
    assert( seqval_match ) ;
}


void test_AbbrevToFlagValSequence()
{
    test_AbbrevToFlagValSequence("TO:0 SR:1 SA:0", "TO SR SA", 0x121ull );
    test_AbbrevToFlagValSequence("TO:0 SC: SR:1 SA:0", "TO SC SR SA", 0x1201ull );
}

void test_PointAbbrev()
{
    unsigned long long seqhis = 0x4ad ;  
    for(unsigned p=0 ; p < 5 ; p++) LOG(info) << OpticksFlags::PointAbbrev(seqhis, p ) ; 
}
void test_PointVal1()
{
    unsigned long long seqval = 0x121 ;  
    for(unsigned p=0 ; p < 5 ; p++) LOG(info) << OpticksFlags::PointVal1(seqval, p ) ; 
}


void test_cfTypes(Opticks* ok)
{
    Types* types = ok->getTypes();

    for(unsigned i=0 ; i < 32 ; i++)
    {
        unsigned bitpos = i ;
        unsigned flag = 0x1 << bitpos ; 
        std::string hs = types ? types->getHistoryString( flag ) : "notyps" ;

        const char* hs2 = OpticksFlags::Flag(flag) ; 

        std::cout 
            << " i " << std::setw(3) << i 
            << " flag " << std::setw(10) << flag 
            << " hs " << std::setw(20) << hs
            << " hs2 " << std::setw(20) << hs2
            << std::endl 
            ;

    }
}




int main(int argc, char** argv)
{
    PLOG_(argc, argv);
    OKCORE_LOG__ ;   

    Opticks ok(argc, argv);

    /*
    test_ctor();
    test_FlagMask();
    test_AbbrevToFlag();
    test_AbbrevToFlagSequence();
    test_AbbrevToFlagValSequence();
    test_PointAbbrev();
    test_PointVal1();
    */

    test_cfTypes(&ok);

    return 0 ; 
}
