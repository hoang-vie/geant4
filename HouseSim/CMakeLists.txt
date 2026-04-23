cmake_minimum_required(VERSION 3.16...3.27)
project(ShieldingSim)

find_package(Geant4 REQUIRED ui_all vis_all)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(ShieldingSim ShieldingSim.cc ${sources} ${headers})
target_include_directories(ShieldingSim PRIVATE include)
target_link_libraries(ShieldingSim PRIVATE ${Geant4_LIBRARIES})

set(MACRO_SCRIPTS
  run.mac
  vis.mac
)

foreach(_script ${MACRO_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()