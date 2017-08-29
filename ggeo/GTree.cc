#include "NSensor.hpp"
#include "NPY.hpp"

#include "GNode.hh"
#include "GSolid.hh"
#include "GTree.hh"

#include "PLOG.hh"





NPY<float>* GTree::makeInstanceTransformsBuffer(const std::vector<GNode*>& placements)
{
    // collecting transforms from GNode instances into a buffer
    // getPlacement for ridx=0 just returns m_root (which always has identity transform)
    // for ridx > 0 returns all GNode instances 

    unsigned int ni = placements.size(); 
    NPY<float>* buf = NPY<float>::make(0, 4, 4);
    for(unsigned int i=0 ; i < ni ; i++)
    {
        GNode* place = placements[i] ;
        GMatrix<float>* t = place->getTransform();
        buf->add(t->getPointer(), 4*4*sizeof(float) );
    } 
    assert(buf->getNumItems() == ni);
    return buf ; 
}


NPY<unsigned int>* GTree::makeAnalyticInstanceIdentityBuffer(const std::vector<GNode*>& placements) 
{
    // collect identity information for each of the repeated nodes (or subtrees)
    // eg PMT sensor index

    unsigned int numInstances = placements.size() ;

    NPY<unsigned int>* buf = NPY<unsigned int>::make(numInstances, 1, 4); // huh non-analytic uses (-1,4)
    buf->zero(); 

    // NB the differences:
    //
    //    analytic
    //         identity buffer has numInstances items (ie one entry for each repeated instance)
    //
    //    triangulated:  
    //         identity buffer has numInstances*numSolids items (ie one entry for every solid of every instance)
    //         ... downstream this gets repeated further to every triangle
    //

    unsigned int numProgeny = placements[0]->getProgenyCount();
    unsigned int numSolids  = numProgeny + 1 ; 

    // observe that each instance has only one sensor, so not need 
    // to repeat over the number of solids just one entry per instance

    LOG(info) << "GTree::makeAnalyticInstanceIdentityBuffer " 
              << " numPlacements " << numInstances
              << " numSolids " << numSolids      
              ;

    for(unsigned int i=0 ; i < numInstances ; i++) // over instances of the same geometry
    {
        GNode* base = placements[i] ;
        unsigned ridx = base->getRepeatIndex();

        assert( numProgeny == base->getProgenyCount() );  // repeated geometry for the instances, so the progeny counts must match 
        std::vector<GNode*>& progeny = base->getProgeny();

        bool progeny_match = progeny.size() == numProgeny ;

        if(ridx > 0)
        {
            if(!progeny_match)
               LOG(fatal) << "GTree::makeAnalyticInstanceIdentityBuffer"
                          << " progeny MISMATCH "
                          << " progeny.size() " << progeny.size() 
                          << " numProgeny " << numProgeny
                          << " numInstances " << numInstances
                          << " numSolids " << numSolids
                          << " i " << i 
                          << " ridx " << ridx
                          ;

            assert( progeny_match );
        }

      
        NSensor* sensor = NULL ;  
        for(unsigned int s=0 ; s < numSolids ; s++ )  // loop over solids looking for a sensor
        {
            GNode* node = s == 0 ? base : progeny[s-1] ; 
            GSolid* solid = dynamic_cast<GSolid*>(node) ;
            NSensor* ss = solid->getSensor();
            //assert(ss); dont have JUNO sensor info

            unsigned int sid = ss && ss->isCathode() ? ss->getId() : 0 ;

            if(sid > 0)
            LOG(debug) << "GTree::makeAnalyticInstanceIdentityBuffer " 
                      << " s " << std::setw(3) << s 
                      << " sid " << std::setw(10) << std::hex << sid << std::dec 
                      << " ss " << (ss ? ss->description() : "NULL" )
                      ;

            if(sid > 0 && numInstances > 1)  // formerly && ridx > 0 
            {
                assert(sensor == NULL && "not expecting more than one sensor solid with non-zero id within an instance of repeated geometry");
                sensor = ss ; 
            }
        }

        glm::uvec4 aii ; 

        aii.x = base->getIndex();        
        aii.y = i ;  // instance index (for triangulated this contains the mesh index)
        aii.z = 0 ;  // formerly boundary, but with analytic have broken 1-1 solid/boundary relationship so boundary must live in partBuffer
        aii.w = NSensor::RefIndex(sensor) ;  // the only critical one 

        buf->setQuadU(aii, i, 0); 
        
    }
    return buf ; 
}

NPY<unsigned int>* GTree::makeInstanceIdentityBuffer(const std::vector<GNode*>& placements) 
{
    /*
     Repeating identity guint4 for all solids of an instance (typically ~5 solids for 1 instance)
     into all the instances (typically large 500-36k).


     Instances need to know the sensor they correspond 
     even though their geometry is duplicated. 

     For analytic geometry this is needed at the solid level 
     ie need buffer of size:
             #transforms * #solids-per-instance

     For triangulated geometry this is needed at the triangle level
     ie need buffer of size 
             #transforms * #triangles-per-instance

     The triangulated version can be created from the analytic one
     by duplication according to the number of triangles.

    */

    unsigned int numInstances = placements.size() ;
    unsigned int numProgeny = placements[0]->getProgenyCount();
    unsigned int numSolids  = numProgeny + 1 ; 
    unsigned int num = numSolids*numInstances ; 

    NPY<unsigned int>* buf = NPY<unsigned int>::make(0, 4);

    for(unsigned int i=0 ; i < numInstances ; i++)
    {
        GNode* base = placements[i] ;

        unsigned ridx = base->getRepeatIndex();

        assert( numProgeny == base->getProgenyCount() && "repeated geometry for the instances, so the progeny counts must match");

        std::vector<GNode*>& progeny = base->getProgeny();

        bool progeny_match = progeny.size() == numProgeny ;

        if(ridx > 0)
        {
           if(!progeny_match)
           LOG(fatal) << "GTree::makeInstanceIdentityBuffer"
                      << " progeny_match " << ( progeny_match ? " OK " : " MISMATCH " )
                      << " progeny.size() " << progeny.size() 
                      << " numProgeny " << numProgeny
                      << " numInstances " << numInstances
                      << " numSolids " << numSolids
                      << " i " << i 
                      << " ridx " << ridx
                      ;


            assert( progeny_match );
        }

        for(unsigned int s=0 ; s < numSolids ; s++ )
        {
            GNode* node = s == 0 ? base : progeny[s-1] ; 
            GSolid* solid = dynamic_cast<GSolid*>(node) ;

            guint4 id = solid->getIdentity();
            buf->add(id.x, id.y, id.z, id.w ); 

#ifdef DEBUG
            std::cout  
                  << " i " << i
                  << " s " << s
                  << " node/mesh/boundary/sensor " << id.x << "/" << id.y << "/" << id.z << "/" << id.w 
                  << " nodeName " << node->getName()
                  << std::endl 
                  ;
#endif
        }
    }
    assert(buf->getNumItems() == num);
    return buf ;  
}

/*

::

    In [1]: ii = np.load("iidentity.npy")

    In [3]: ii.shape
    Out[3]: (3360, 4)

    In [4]: ii.reshape(-1,5,4)
    Out[4]: 
    array([[[ 3199,    47,    19,     1],
            [ 3200,    46,    20,     2],
            [ 3201,    43,    21,     3],
            [ 3202,    44,     1,     4],
            [ 3203,    45,     1,     5]],

           [[ 3205,    47,    19,     6],
            [ 3206,    46,    20,     7],
            [ 3207,    43,    21,     8],
            [ 3208,    44,     1,     9],
            [ 3209,    45,     1,    10]],

After requiring an associated sensor surface to provide the sensor index, only cathodes 
have non-zero index::

    In [1]: ii = np.load("iidentity.npy")

    In [2]: ii.reshape(-1,5,4)
    Out[2]: 
    array([[[ 3199,    47,    19,     0],
            [ 3200,    46,    20,     0],
            [ 3201,    43,    21,     3],
            [ 3202,    44,     1,     0],
            [ 3203,    45,     1,     0]],

           [[ 3205,    47,    19,     0],
            [ 3206,    46,    20,     0],
            [ 3207,    43,    21,     8],
            [ 3208,    44,     1,     0],
            [ 3209,    45,     1,     0]],
*/


