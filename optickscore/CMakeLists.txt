cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(name OpticksCore)
project(${name})

include(EnvBuildOptions)
include(EnvCompilationFlags)

find_package(OpticksBoost REQUIRED)
find_package(GLM REQUIRED)

find_package(BRegex REQUIRED)
find_package(BCfg REQUIRED)
find_package(NPY REQUIRED)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${OpticksBoost_INCLUDE_DIRS} 
   ${GLM_INCLUDE_DIRS}  

   ${BRegex_INCLUDE_DIRS}  
   ${BCfg_INCLUDE_DIRS}  
   ${NPY_INCLUDE_DIRS}  
)

add_definitions( 
   ${OpticksBoost_DEFINITIONS} 
   ${GLM_DEFINITIONS} 
   ${NPY_DEFINITIONS} 
   ${BCfg_DEFINITIONS} 
   ${BRegex_DEFINITIONS} 
    -DVERBOSE 
)

set(LIBRARIES
   ${OpticksBoost_LIBRARIES}
   ${GLM_LIBRARIES}

   ${NPY_LIBRARIES}
   ${BCfg_LIBRARIES}
   ${BRegex_LIBRARIES}
)

add_library( ${name}  SHARED
    OpticksResource.cc
    Opticks.cc
    OpticksCfg.cc

    Composition.cc
    CompositionCfg.cc
    View.cc
    ViewCfg.cc
    Camera.cc
    CameraCfg.cc
    Trackball.cc
    TrackballCfg.cc
    Clipper.cc
    ClipperCfg.cc
    InterpolatedView.cc
    OrbitalView.cc
    TrackView.cc

    Animator.cc
    Bookmarks.cc
    Light.cc


    Demo.cc
    DemoCfg.cc
)

SET(hdrs 
    OpticksResource.hh
    Opticks.hh
    OpticksCfg.hh

    Composition.hh
    CompositionCfg.hh
    View.hh
    ViewCfg.hh
    Camera.hh
    CameraCfg.hh
    Trackball.hh
    TrackballCfg.hh
    Clipper.hh
    ClipperCfg.hh
    InterpolatedView.hh
    OrbitalView.hh
    TrackView.hh

    Animator.hh
    Bookmarks.hh
    Light.hh

    OpticksPhoton.h
    OpticksFlags.h
)

target_link_libraries( ${name} ${LIBRARIES} )
install(TARGETS ${name}  DESTINATION lib)
install(FILES ${hdrs} DESTINATION include/${name})

add_subdirectory(tests EXCLUDE_FROM_ALL)
