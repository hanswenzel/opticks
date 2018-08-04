// TEST=NPYSpecListTest om-t 

#include "OPTICKS_LOG.hh"
#include "NPYBase.hpp"
#include "NPYSpec.hpp"
#include "NPYSpecList.hpp"

typedef enum 
{ 
   SRC_NODES,
   SRC_IDX, 
   SRC_TRANSFORMS, 
   SRC_PLANES, 
   SRC_FACES, 
   SRC_VERTS,

   NODES,
   TRANSFORMS, 
   GTRANSFORMS, 
   IDX

}  NCSGData_t ; 

NPYSpecList* make_speclist()
{
    NPYSpecList* sl = new NPYSpecList ; 

    sl->add( (unsigned)SRC_NODES       , new NPYSpec("srcnodes.npy"       , 0, 4, 4, 0, 0, NPYBase::FLOAT , "" ));
    sl->add( (unsigned)SRC_IDX         , new NPYSpec("srcidx.npy"         , 0, 4, 0, 0, 0, NPYBase::UINT  , "" ));
    sl->add( (unsigned)SRC_TRANSFORMS  , new NPYSpec("srctransforms.npy"  , 0, 4, 4, 0, 0, NPYBase::FLOAT , "" ));
    sl->add( (unsigned)SRC_PLANES      , new NPYSpec("srcplanes.npy"      , 0, 4, 0, 0, 0, NPYBase::FLOAT , "" ));
    sl->add( (unsigned)SRC_FACES       , new NPYSpec("srcfaces.npy"       , 0, 3, 0, 0, 0, NPYBase::INT   , "" ));
    sl->add( (unsigned)SRC_VERTS       , new NPYSpec("srcverts.npy"       , 0, 3, 0, 0, 0, NPYBase::FLOAT , "" ));
    sl->add( (unsigned)NODES           , new NPYSpec("nodes.npy"          , 0, 4, 4, 0, 0, NPYBase::FLOAT , "" ));
    sl->add( (unsigned)TRANSFORMS      , new NPYSpec("transforms.npy"     , 0, 4, 4, 0, 0, NPYBase::FLOAT , "" ));
    sl->add( (unsigned)GTRANSFORMS     , new NPYSpec("gtransforms.npy"    , 0, 4, 4, 0, 0, NPYBase::FLOAT , "" ));
    sl->add( (unsigned)IDX             , new NPYSpec("idx.npy"            , 0, 4, 0, 0, 0, NPYBase::UINT  , "" ));

    LOG(info) << std::endl << sl->description() ; 
    return sl ; 
} 

int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv); 

    NPYSpecList* sl = make_speclist(); 

    NCSGData_t bid = GTRANSFORMS ; 
    const NPYSpec* spec = sl->getByIdx( (unsigned)bid ); 
    LOG(info) << " GTRANSFORMS : " << spec->description() ; 

    return 0 ; 
}

