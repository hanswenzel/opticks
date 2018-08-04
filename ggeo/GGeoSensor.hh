#pragma once

#include "GGEO_API_EXPORT.hh"

class GGeo ; 
class GOpticalSurface ; 
class GSkinSurface ; 

class GGEO_API GGeoSensor 
{
    public:
        static GOpticalSurface* MakeOpticalSurface( const char* sslv );
        static GSkinSurface*    MakeSensorSurface(const char* sslv, unsigned index ); 
        static void AddSensorSurfaces( GGeo* gg );  
};


 