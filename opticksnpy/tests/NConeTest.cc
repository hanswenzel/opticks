#include <iostream>
#include <iomanip>

#include "NCone.hpp"

int main()
{
    float radius = 4.f ; 
    float height = 3.f ;
    unsigned flags = 0u ; 

    ncone cone = make_cone(0,0,0, radius, height, flags ) ; 
    nnode* node = (nnode*)&cone ;

    for(float v=10. ; v >= -10. ; v-=1.f )
        std::cout 
               << " v       " << std::setw(10) << v 
               << " x       " << std::setw(10) << (*node)(v, 0, 0) 
               << " y       " << std::setw(10) << (*node)(0, v, 0) 
               << " z       " << std::setw(10) << (*node)(0, 0, v) 
               << " x(z=1)  " << std::setw(10) << (*node)(v, 0, 1.f) 
               << " y(z=1)  " << std::setw(10) << (*node)(0, v, 1.f) 
               << " x(z=-1) " << std::setw(10) << (*node)(v, 0, -1.f) 
               << " y(z=-1) " << std::setw(10) << (*node)(0, v, -1.f) 
               << std::endl ;  


    return 0 ; 
} 
