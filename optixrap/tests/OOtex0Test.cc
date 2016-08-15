#include "NPY.hpp"
#include "OXPPNS.hh"
#include "OConfig.hh"
#include "PLOG.hh"

int main(int argc, char** argv)
{
    PLOG_(argc, argv);    
    LOG(info) << " ok " ; 

    optix::Context context = optix::Context::create();

    int nx = 16 ; 
    int ny = 16 ; 

    NPY<float>* inp = NPY<float>::make(nx, ny);
    inp->zero();

    float* vals = inp->getValues();
    for(int i=0 ; i < nx ; i++){
    for(int j=0 ; j < ny ; j++)
    {
       int index = i*ny + j ;
       *(vals + index) = float(index) ;
    }
    }

    inp->dump();
    inp->save("$TMP/tex0Test_inp.npy");

    optix::Buffer outBuffer = context->createBuffer(RT_BUFFER_OUTPUT, RT_FORMAT_FLOAT, nx, ny);
    optix::Buffer texBuffer = context->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT, nx, ny);
    memcpy( texBuffer->map(), inp->getBytes(), inp->getNumBytes(0) );
    texBuffer->unmap(); 

    optix::TextureSampler tex = context->createTextureSampler();

    // RT_WRAP_CLAMP_TO_BORDER: giving zero when out of range makes 
    // most sense, but seems not supported... but actually not 
    // a big issue as tex coords are highly controlled anyhow
    // except perhaps the wavelength
    RTwrapmode wrapmode = RT_WRAP_REPEAT ;
    //RTwrapmode wrapmode = RT_WRAP_CLAMP_TO_EDGE ;
    //RTwrapmode wrapmode = RT_WRAP_MIRROR ;
    //RTwrapmode wrapmode = RT_WRAP_CLAMP_TO_BORDER ;  
    tex->setWrapMode(0, wrapmode); 
    tex->setWrapMode(1, wrapmode);

    RTfiltermode filtermode = RT_FILTER_LINEAR ;
    RTfiltermode minification = filtermode ;
    RTfiltermode magnification = filtermode ;
    RTfiltermode mipmapping = RT_FILTER_NONE ;
    tex->setFilteringModes(minification, magnification, mipmapping);

    //RTtextureindexmode indexingmode = RT_TEXTURE_INDEX_ARRAY_INDEX ; 
    RTtextureindexmode indexingmode = RT_TEXTURE_INDEX_NORMALIZED_COORDINATES ; 
    tex->setIndexingMode(indexingmode);  

    //RTtexturereadmode readmode = RT_TEXTURE_READ_NORMALIZED_FLOAT ;
    RTtexturereadmode readmode = RT_TEXTURE_READ_ELEMENT_TYPE ;    // No conversion
    tex->setReadMode(readmode);

    tex->setMaxAnisotropy(1.0f);
    tex->setMipLevelCount(1);
    tex->setArraySize(1);
    tex->setBuffer(0, 0, texBuffer);


    context["some_texture"]->setTextureSampler(tex);
    context["out_buffer"]->setBuffer(outBuffer);   

    int tex_id = tex->getId();
    context["tex_param"]->setInt(optix::make_int4(tex_id, 0, 0, 0 ));


    OConfig* cfg = new OConfig(context); 

    bool defer = true ;
    unsigned int entry = cfg->addEntry("tex0Test.cu.ptx", "tex0Test", "exception", defer);

    context->setEntryPointCount(1) ;

    cfg->dump();
    cfg->apply();     


    context->setPrintEnabled(true);
    context->setPrintBufferSize(8192);

    context->validate();
    context->compile();
    context->launch(entry,  0, 0);
    context->launch(entry, nx, ny);


    NPY<float>* out = NPY<float>::make(nx, ny);
    out->read( outBuffer->map() );
    outBuffer->unmap(); 

    out->dump();
    out->save("$TMP/tex0Test_out.npy");

    float maxdiff = inp->maxdiff(out) ;

    LOG(info) << "maxdiff " << maxdiff  ; 
    assert(maxdiff < 1e-6 );

    LOG(info) << "DONE" ; 

    return 0 ;     
}

