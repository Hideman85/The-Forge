set(THE_FORGE_LIBS)
set(THE_FORGE_SHARED_LIBS)
set(THE_FORGE_LIBS_INCLUDES)
cmake_policy(SET CMP0077 NEW) # for option overriding

##########################################
##      Assimp
##########################################
if (USE_ASSIMP)
    set(ASSIMP_BUILD_SAMPLES OFF)
    set(ASSIMP_BUILD_TESTS OFF)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
    set(INJECT_DEBUG_POSTFIX OFF)
    set(ASSIMP_INSTALL_PDB OFF)
    set(ASSIMP_ANDROID_JNIIOSYSTEM OFF) # Disable because <android/api-level.h> not found # still trying to compile it
    set(ASSIMP_BUILD_ZLIB ON)
    add_subdirectory(assimp/4.1.0)
    list(APPEND THE_FORGE_LIBS assimp)
    list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/assimp/4.1.0/include)
endif()

##########################################
##      Basis Universal
##########################################
# Inside CMakeLists produce a executable that we don't want
#set(STATIC ON)
#add_subdirectory(basis_universal)
# We really us only the transcoder code
file(GLOB BASISU_TRANSCODER_SOURCES "basis_universal/transcoder/*.cpp")
set(BASISU_TRANSCODER_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/basis_universal/transcoder)
add_library(BasisuTranscoder STATIC ${BASISU_TRANSCODER_SOURCES})

list(APPEND THE_FORGE_LIBS BasisuTranscoder)
list(APPEND THE_FORGE_LIBS_INCLUDES ${BASISU_TRANSCODER_SOURCES})

##########################################
##      DirectXShaderCompiler
##########################################
# Not use

##########################################
##      EASTL
##########################################
# Build target
#file(GLOB EASTL_SOURCES "EASTL/*.cpp")
set(EASTL_SOURCES
        EASTL/EAStdC/EAMemory.cpp
        EASTL/intrusive_list.cpp
        EASTL/red_black_tree.cpp
        EASTL/numeric_limits.cpp
        EASTL/thread_support.cpp
        EASTL/assert.cpp
        EASTL/string.cpp
        EASTL/allocator_forge.cpp
        EASTL/allocator_eastl.cpp
        EASTL/hashtable.cpp
        EASTL/fixed_pool.cpp
        EASTL/EAStdC/EASprintf.cpp
)
set(EASTL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/EASTL)
add_library(EASTL STATIC ${EASTL_SOURCES})
set_target_properties(EASTL PROPERTIES INCLUDE_DIRECTORIES "")

list(APPEND THE_FORGE_LIBS EASTL)
#list(APPEND THE_FORGE_LIBS_INCLUDES ${EASTL_INCLUDES})

##########################################
##      FluidStudios
##########################################
if (USE_LUA)
    # Build target
    file(GLOB FS_SOURCES "FluidStudios/MemoryManager/*.cpp")
    set(FS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/FluidStudios/MemoryManager)
    add_library(FS STATIC ${FS_SOURCES})

    list(APPEND THE_FORGE_LIBS FS)
    list(APPEND THE_FORGE_LIBS_INCLUDES ${FS_INCLUDES})
endif()

##########################################
##      Fontstash
##########################################
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Fontstash/src)

##########################################
##      Gainput
##########################################
set(GAINPUT_TESTS OFF)
set(GAINPUT_SAMPLES OFF)
set(GAINPUT_BUILD_SHARED OFF)
add_subdirectory(gainput)
list(APPEND THE_FORGE_LIBS gainputstatic)
list(APPEND THE_FORGE_LIBS_INCLUDES $CMAKE_CURRENT_SOURCE_DIR}/gainput/lib/include)

##########################################
##      Imgui
##########################################
set(IMGUI_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDES})
list(APPEND THE_FORGE_LIBS imgui)
list(APPEND THE_FORGE_LIBS_INCLUDES ${IMGUI_INCLUDES})

##########################################
##      LibZip
##########################################
set(LIB_ZIP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Linux/libzip.a)
add_custom_target(libzip_copy)
add_custom_command(TARGET libzip_copy PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIB_ZIP_FILE} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/libzip)

##########################################
##      Lua
##########################################
if (USE_LUA)
    # Build target
    file(GLOB LUA_SOURCES "lua-5.3.5/src/*.cpp")
    set(LUA_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/lua-5.3.5/src)
    add_library(Lua STATIC ${FS_SOURCES})

    list(APPEND THE_FORGE_LIBS Lua)
    list(APPEND THE_FORGE_LIBS_INCLUDES ${LUA_INCLUDES})
endif()

##########################################
##      MeshOptimizer
##########################################
# Build target
file(GLOB MO_SOURCES "meshoptimizer/src/*.cpp")
set(MO_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/meshoptimizer/src)
add_library(MeshOptimizer STATIC ${MO_SOURCES})

list(APPEND THE_FORGE_LIBS MeshOptimizer)
list(APPEND THE_FORGE_LIBS_INCLUDES ${MO_INCLUDES})

##########################################
##      MicroProfile
##########################################
# Build target
file(GLOB MP_SOURCES "MicroProfile/*.cpp")
set(MP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/MicroProfile)
add_library(MicroProfile STATIC ${MP_SOURCES})

list(APPEND THE_FORGE_LIBS MicroProfile)
list(APPEND THE_FORGE_LIBS_INCLUDES ${MP_INCLUDES})

##########################################
##      SonyMath
##########################################
# Header only
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/ModifiedSonyMath)

##########################################
##      STB Stuffs
##########################################
# Header only
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Nothings)

##########################################
##      Ozz-Animation
##########################################
set(ozz_build_samples OFF)
set(ozz_build_howtos OFF)
set(ozz_build_tests OFF)
set(ozz_build_tools OFF)
set(ozz_build_fbx OFF)
set(ozz_build_msvc_rt_dll OFF)
set(ozz_build_cpp14 ON)
add_subdirectory(ozz-animation)
list(APPEND THE_FORGE_LIBS ozz_base)
list(APPEND THE_FORGE_LIBS ozz_animation)
list(APPEND THE_FORGE_LIBS ozz_options)
#list(APPEND THE_FORGE_LIBS ozz_geometry) # require ozz/base/maths/simd_math.h that has been removed by TheForge team
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/ozz-animation/include)

##########################################
##      Render doc
##########################################
# Not use

##########################################
##      Soloud
##########################################
file(GLOB_RECURSE SOLOUD_SOURCES "soloud20181119/src/*.cpp")
set(SOLOUD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/soloud20181119/include")
set(SOLOUD_LIBS)

add_library(Soloud STATIC ${SOLOUD_SOURCES})
target_include_directories(Soloud PUBLIC ${SOLOUD_INCLUDES})
target_link_libraries(Soloud ${SOLOUD_LIBS})

list(APPEND THE_FORGE_LIBS Soloud)
list(APPEND THE_FORGE_LIBS_INCLUDES ${SOLOUD_INCLUDES})

##########################################
##      SPIRV_Cross
##########################################
# Build target
file(GLOB SPIRV_SOURCES "SPIRV_Cross/*.cpp")
set(SPIRV_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/SPIRV_Cross)
add_library(SPIRV STATIC ${MP_SOURCES})

list(APPEND THE_FORGE_LIBS SPIRV)
list(APPEND THE_FORGE_LIBS_INCLUDES ${SPIRV_INCLUDES})

##########################################
###      TaskScheduler
###########################################
## Not use

##########################################
##      tinydds
##########################################
# Header only
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/tinydds)

##########################################
##      tinydds
##########################################
add_library(TinyEXR STATIC TinyEXR/tinyexr.cpp)
list(APPEND THE_FORGE_LIBS TinyEXR)
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/TinyEXR)

##########################################
##      tinyimageformat
##########################################
# Header only
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/tinyimageformat)

##########################################
##      tinyktx
##########################################
# Header only
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/tinyktx)

##########################################
##      tinydds
##########################################
add_library(TinyXML2 STATIC TinyXML2/tinyxml2.cpp)
list(APPEND THE_FORGE_LIBS TinyXML2)
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/TinyXML2)

##########################################
##      tinydds
##########################################
add_library(volk STATIC volk/volk.c)
list(APPEND THE_FORGE_LIBS volk)
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/volk)

##########################################
##      VulkanMemoryAllocator
##########################################
# Header only
list(APPEND THE_FORGE_LIBS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator)

##########################################
##      winpixeventruntime
##########################################
# D3DX deps (ignore for now)

list(JOIN THE_FORGE_LIBS ";" THE_FORGE_LIBS)
