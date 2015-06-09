#pragma once

#include <vector>
#include <glm/glm.hpp>  

// ggeo-
#include "GVector.hh"


class Camera ;
class View ;
class Trackball ; 
class Clipper ; 
class Cfg ;
class Scene ; 

#include "Configurable.hh"

class Composition : public Configurable {
  public:
      static const char* PRINT ;  
      static const char* SELECT ;  

      friend class Interactor ;   
      friend class Bookmarks ;   
  public:
 
      Composition();
      virtual ~Composition();
  private:
      void init();

  public:
      // Configurable : for bookmarking 
      static bool accepts(const char* name);
      void configure(const char* name, const char* value);
      std::vector<std::string> getTags();
      void set(const char* name, std::string& s);
      std::string get(const char* name);

      // for cli/live updating 
      void configureI(const char* name, std::vector<int> values );
      void configureS(const char* name, std::vector<std::string> values);
      void gui();

  public: 
      void setCenterExtent(gfloat4 ce, bool autocam=false); // effectively points at what you want to look at 
      void setDomainCenterExtent(gfloat4 ce);               // typically whole geometry domain
      void setTimeDomain(gfloat4 td);


  public:
      // avaiable as uniform inside shaders allowing GPU-side selections 
      void setSelection(glm::ivec4 sel);
      void setSelection(std::string sel);
      glm::ivec4& getSelection();
  public:
      void setParam(glm::vec4 par);
      void setParam(std::string par);
      glm::vec4&  getParam();
  public:
      void setFlags(glm::ivec4 flags);
      void setFlags(std::string flags);
      glm::ivec4& getFlags();

  public:
      void setTarget(unsigned int target);
      void setScene(Scene* scene);
      void setSize(unsigned int width, unsigned int height);
      void addConfig(Cfg* cfg);

  public: 
      void home();
      void update();

  public: 
      void test_getEyeUVW();
      void getEyeUVW(glm::vec3& eye, glm::vec3& U, glm::vec3& V, glm::vec3& W);
      void getEyeUVW_no_trackball(glm::vec3& eye, glm::vec3& U, glm::vec3& V, glm::vec3& W);
      void getLookAt(glm::mat4& lookat);

  //private: 
  public: 
      // private getters of residents : usable by friend class
      Camera* getCamera(); 
      Trackball* getTrackball(); 
      View* getView(); 
      Clipper* getClipper(); 
      Scene* getScene(); 

      
      void setCamera(Camera* camera);
      void setView(View* view);

  public: 
      // getters of inputs 
      glm::mat4& getModelToWorld();
      float getExtent();
      float getNear();
      float getFar();
      unsigned int getWidth();
      unsigned int getHeight();
      unsigned int getPixelWidth(); // width*pixel_factor
      unsigned int getPixelHeight();
      unsigned int getPixelFactor();
      void setPixelFactor(unsigned int factor); // 2 for retina display

  public:
      glm::vec3 unProject(unsigned int x, unsigned int y, float z);

  public:
      // getters of the derived properties : need to call update first to make them current
      glm::vec4& getGaze();
      glm::vec4& getCenterExtent();
      glm::vec4& getDomainCenterExtent();
      glm::vec4& getTimeDomain();
      float&     getGazeLength();
      glm::mat4& getWorld2Eye();  // ModelView  including trackballing
      float*     getWorld2EyePtr();  // ModelView  including trackballing
      glm::mat4& getEye2World();
      glm::mat4& getWorld2Camera();
      glm::mat4& getCamera2World();
      glm::mat4& getEye2Look();
      glm::mat4& getLook2Eye();

   public:
      // ModelViewProjection including trackballing
      glm::mat4& getWorld2Clip();   
      float*     getWorld2ClipPtr();  
      glm::mat4& getDomainISNorm();
      glm::mat4& getWorld2ClipISNorm();      // with initial domain_isnorm
      float*     getWorld2ClipISNormPtr();   

   public:
      float*     getIdentityPtr(); 
      glm::mat4& getProjection(); 

  private:
      glm::mat4& getTrackballing(); 
      glm::mat4& getITrackballing(); 

  public:
      int        getClipMode();
      glm::vec4& getClipPlane();
      float*     getClipPlanePtr();

  private:
      // inputs 
      glm::mat4 m_model_to_world ; 
      float     m_extent ; 
      glm::vec4 m_center_extent ; 
      glm::vec4 m_domain_center_extent ; 
      glm::mat4 m_domain_isnorm ;     
      glm::vec4 m_domain_time ; 
  private:
      glm::ivec4 m_selection ;
      glm::ivec4 m_flags ;
      glm::vec4  m_param ;

  private:
      // residents
      Camera*    m_camera ;
      Trackball* m_trackball ;
      View*      m_view ;
      Clipper*   m_clipper ;

      // visitors
      Scene*     m_scene ; 

  private:
      // updated by *update* based on inputs and residents
      glm::vec4 m_viewport ; 
      glm::vec4 m_gaze ; 
      glm::vec4 m_clipplane ; 
      float     m_gazelength ;
      glm::mat4 m_world2eye ;     
      glm::mat4 m_eye2world ;     
      glm::mat4 m_world2camera ; 
      glm::mat4 m_camera2world ; 
      glm::mat4 m_eye2look ;     
      glm::mat4 m_look2eye ;     
      glm::mat4 m_world2clip ;     
      glm::mat4 m_world2clip_isnorm ;     
      glm::mat4 m_projection ;     

      glm::mat4 m_trackballing ;     
      glm::mat4 m_itrackballing ;     

      glm::mat4 m_trackballrot ;     
      glm::mat4 m_itrackballrot ;     
      glm::mat4 m_trackballtra ;     
      glm::mat4 m_itrackballtra ;     

      glm::mat4 m_identity ;     

  public: 
      void Summary(const char* msg);
      void Details(const char* msg);



};      

inline Composition::Composition()
  :
  m_model_to_world(),
  m_extent(1.0f),
  m_center_extent(),
  m_selection(-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX),  // not 0, as that is liable to being meaningful
  m_param(0.f,0.f,0.f,0.f),
  m_camera(NULL),
  m_trackball(NULL),
  m_view(NULL),
  m_clipper(NULL),
  m_scene(NULL)
{
    init();
}



inline Camera* Composition::getCamera()
{
    return m_camera ;
}
inline View* Composition::getView()
{
    return m_view ;
}
inline Trackball* Composition::getTrackball()
{
    return m_trackball ;
}
inline Clipper* Composition::getClipper()
{
    return m_clipper ;
}

inline void Composition::setCamera(Camera* camera)
{
    m_camera = camera ; 
}
inline void Composition::setView(View* view)
{
    m_view = view ; 
}
inline Scene* Composition::getScene()
{
    return m_scene ; 
}
inline void Composition::setScene(Scene* scene)
{
    m_scene = scene ; 
}
inline glm::vec4& Composition::getCenterExtent()
{
    return m_center_extent ; 
}
inline glm::vec4& Composition::getDomainCenterExtent()
{
    return m_domain_center_extent ; 
}
inline glm::vec4& Composition::getTimeDomain()
{
    return m_domain_time ; 
}


inline glm::mat4& Composition::getDomainISNorm()
{
    return m_domain_isnorm ; 
}


inline glm::ivec4& Composition::getSelection()
{
    return m_selection ; 
}
inline glm::ivec4& Composition::getFlags()
{
    return m_flags ; 
}
inline glm::vec4& Composition::getParam()
{
    return m_param ; 
}
inline glm::mat4& Composition::getModelToWorld()
{
    return m_model_to_world ; 
}
inline float Composition::getExtent()
{
    return m_extent ; 
}




