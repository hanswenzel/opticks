#include "NPY.hpp"
#include "PhotonsNPY.hpp"
#include "GLMPrint.hpp"
#include "stdlib.h"

int main(int argc, char** argv)
{
    const char* idpath = getenv("IDPATH");

    NPY<float>* photons = NPY<float>::load("oxcerenkov", "1");
    NPY<short>* records = NPY<short>::load("rxcerenkov", "1");
    NPY<float>* domain = NPY<float>::load("domain","1");
    domain->dump();

    glm::vec4 ce = domain->getQuad(0,0);
    glm::vec4 td = domain->getQuad(1,0);
    glm::vec4 wd = domain->getQuad(2,0);
    print(ce, "ce");
    print(td, "td");
    print(wd, "wd");


    PhotonsNPY pn(photons, records);

    pn.readFlags("$ENV_HOME/graphics/ggeoview/cu/photon.h");
    pn.dumpFlags();

    pn.readMaterials(idpath, "GMaterialIndexLocal.json");
    pn.dumpMaterials();
    pn.setCenterExtent(ce);    
    pn.setTimeDomain(td);    
    pn.setWavelengthDomain(wd);    


    pn.dump("oxc.dump");

    pn.classify();
    pn.classify(true);

    pn.examinePhotonHistories();
    pn.examineRecordHistories(); 
    pn.dumpRecords("records", 30);


    return 0 ;
}
