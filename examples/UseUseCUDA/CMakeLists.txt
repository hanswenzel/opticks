#[=[
UseUseCUDA
===========

Configure the building of an executable that 
uses the UseCUDA library

#]=]


cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(name UseUseCUDA)
project(${name} VERSION 0.1.0)
include(OpticksBuildOptions)

set(OpticksCUDA_VERBOSE ON)
find_package(UseCUDA 0.1.0 REQUIRED)

echo_target_std(Opticks::UseCUDA)

add_executable( ${name} ${name}.cc )
target_link_libraries(${name} PUBLIC Opticks::UseCUDA )
install(TARGETS ${name} DESTINATION lib)

