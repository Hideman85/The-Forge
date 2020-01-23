file(GLOB UNIT_TESTS LIST_DIRECTORIES TRUE */)

foreach(UNIT_TEST ${UNIT_TESTS})
    if (IS_DIRECTORY ${UNIT_TEST})
        get_filename_component(UNIT_TEST_NAME ${UNIT_TEST} NAME)
        if (NOT ${UNIT_TEST_NAME} MATCHES "31_Audio") # Disable this case for now socloud need more times to allocate for
            file(GLOB_RECURSE UNIT_TEST_SOURCES ${UNIT_TEST}/*.cpp)
            add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
            set_target_properties(${UNIT_TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${UNIT_TEST}/build")
        endif()
    endif()
endforeach()
