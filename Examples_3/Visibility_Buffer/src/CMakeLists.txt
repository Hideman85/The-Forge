file(GLOB_RECURSE SOURCES *.cpp)
add_executable(Visibility_Buffer ${SOURCES})
set_target_properties(Visibility_Buffer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/build") # just for fitting to relative path in the code
