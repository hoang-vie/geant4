cmake_minimum_required(VERSION 3.16...3.27)
project(DecaySim)

find_package(Geant4 REQUIRED ui_all vis_all)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(DecaySim DecaySim.cc ${sources} ${headers})
target_include_directories(DecaySim PRIVATE include)
target_link_libraries(DecaySim PRIVATE ${Geant4_LIBRARIES})

# Copy các file kịch bản (kể cả file loop)
set(MACRO_SCRIPTS run.mac run_single.mac vis.mac)
foreach(_script ${MACRO_SCRIPTS})
  configure_file(${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_BINARY_DIR}/${_script} COPYONLY)
endforeach()