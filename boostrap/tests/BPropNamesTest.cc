#include "BPropNames.hh"
#include "BTxt.hh"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <climits>


int main(int, char**)
{

    //const char* lib = "GMaterialLib.txt" ;   // requires geocache 
    const char* lib = "$TMP/GMaterialLib.txt" ;  // absolute path mode for testing

    BTxt txt(lib);
    txt.addLine("red");
    txt.addLine("green");
    txt.addLine("blue");
    txt.write();


    BPropNames pn(lib);

    for(unsigned int i=0 ; i < pn.getNumLines() ; i++)
    {
        std::string line = pn.getLine(i) ;
        unsigned int index = pn.getIndex(line.c_str());

        std::cout << " i  " << std::setw(3) << i 
                  << " ix " << std::setw(3) << index
                  << " line " << line
                  << std::endl 
                  ; 

         assert(i == index);
    }

    assert( pn.getIndex("THIS_LINE_IS_NOT_PRESENT") == UINT_MAX );

    return 0 ; 
}