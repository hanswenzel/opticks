#include "NPropNames.hpp"
#include "NTxt.hpp"
#include "NLog.hpp"

#include <cstdlib>

std::string NPropNames::libpath(const char* libname)
{
    char* idp = getenv("IDPATH") ;
    char path[256];
    snprintf(path, 256, "%s/GItemList/%s.txt", idp, libname );
    return path ; 
}


void NPropNames::read()
{
    if(!m_txt)
    {
        std::string path = libpath(m_libname) ; 
        m_txt = new NTxt(path.c_str());
    }
    m_txt->read();
}


const char* NPropNames::getLine(unsigned int num)
{
   return m_txt ? m_txt->getLine(num) : NULL ; 
}
unsigned int  NPropNames::getNumLines()
{
   return m_txt ? m_txt->getNumLines() : 0 ;
}



