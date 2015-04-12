#include "Texture.hh"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assert.h"

#define LOADPPM_IMPLEMENTATION
#include "loadPPM.h"



/*
   "1" (-1,1) [0,1]          "0" (1,1) [1,1]



   "2" (-1,-1) [0,0]         "3" (1,-1) [1,0]
*/


const float Texture::pvertex[] = {  
     1.0f,  1.0f, 0.0f, 
    -1.0f,  1.0f, 0.0f, 
    -1.0f, -1.0f, 0.0f, 
     1.0f, -1.0f, 0.0f
};

const float Texture::pcolor[] = { 
      1.0f, 0.0f, 0.0f,  
      0.0f, 1.0f, 0.0f,  
      0.0f, 0.0f, 1.0f,  
      1.0f, 0.0f, 0.0f
};  
                              
const float Texture::pnormal[] = { 
      0.0f, 0.0f, 1.0f,  
      0.0f, 0.0f, 1.0f,  
      0.0f, 0.0f, 1.0f,  
      0.0f, 0.0f, 1.0f
};  
 
const float Texture::ptexcoord[] = { 
     1.0f, 1.0f, 
     0.0f, 1.0f, 
     0.0f, 0.0f, 
     1.0f, 0.0f 
};


const unsigned int Texture::pindex[] = {
      0,  1,  2 ,
      2,  3,  0 
};




Texture::Texture() : 
    GMesh(0, 
             (gfloat3*)&pvertex[0],
             4, 
             (guint3*)&pindex[0],
             2, 
             (gfloat3*)&pnormal[0],
             (gfloat2*)&ptexcoord[0]
         ),
    m_texture_id(0),
    m_sampler_id(0),
    m_tex()
{
    setColors( (gfloat3*)&pcolor[0] );
}


void Texture::loadPPM(const char* path)
{
    m_tex.rgb = ::loadPPM((char*)path, &m_tex.width, &m_tex.height );
    if(m_tex.rgb)
    {
        printf("Texture::loadPPM loaded %s into tex of width %d height %d \n", path, m_tex.width, m_tex.height);
        setSize(m_tex.width, m_tex.height);

        m_tex.rgba = (unsigned char*)malloc(m_width*m_height*4);

        for(unsigned int w=0; w<m_width;++w){
        for(unsigned int h=0; h<m_height;++h)
        {
            unsigned char* rgb = m_tex.rgb + (h*m_width+w)*3 ;
            unsigned char* rgba = m_tex.rgba + ((m_height-1-h)*m_width+w)*4 ;

            *(rgba+0) = *(rgb+0); 
            *(rgba+1) = *(rgb+1); 
            *(rgba+2) = *(rgb+2); 
            *(rgba+3) = 0x11 ; 
        }
        }
    }
    else
    {
        printf("Texture::loadPPM failed to load %s \n", path );
    }
}

void Texture::setSize(unsigned int width, unsigned int height)
{
    m_width = width ; 
    m_height = height ; 
}


void Texture::setup()
{
    glGenTextures(1, &m_texture_id);
    glBindTexture( GL_TEXTURE_2D, m_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::create()
{
    setup();
    assert(m_width > 0 && m_height > 0 && "must setSize before create");

    if( m_tex.rgba )  // ie have loaded from PPM
    {
        create_rgba(m_tex.rgba);
    }
    else
    {
        create_rgba(NULL);
    }
}

void Texture::create_rgb(unsigned char* data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
}

void Texture::create_rgba(unsigned char* data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
}

void Texture::cleanup()
{
    glDeleteTextures(1, &m_texture_id);
    m_texture_id = 0 ;
}


/*
void Texture::resize(unsigned int width, unsigned int height, unsigned char* data)
{
    if(width == m_width && height == m_height) return ;
    cleanup();
    create(data);
}
*/


unsigned int Texture::getTextureId()
{
    return m_texture_id ;
}
unsigned int Texture::getSamplerId()
{
    return m_sampler_id ;
}



unsigned int Texture::getWidth()
{
    return m_width ;
}
unsigned int Texture::getHeight()
{
    return m_height ;
}



