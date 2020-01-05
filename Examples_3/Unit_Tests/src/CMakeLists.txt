file(GLOB UNIT_TESTS LIST_DIRECTORIES TRUE */)

foreach(UNIT_TEST ${UNIT_TESTS})
    if (IS_DIRECTORY ${UNIT_TEST})
        get_filename_component(UNIT_TEST_NAME ${UNIT_TEST} NAME)
        file(GLOB_RECURSE UNIT_TEST_SOURCES ${UNIT_TEST}/*.cpp)
        add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
    endif()
endforeach()
