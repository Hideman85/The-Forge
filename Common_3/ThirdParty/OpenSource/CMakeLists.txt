set(THE_FORGE_LIBS)
set(THE_FORGE_SHARED_LIBS)

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
    copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/assimp/4.1.0/include" "${CMAKE_SOURCE_DIR}/include")
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
copy_headers("${BASISU_TRANSCODER_INCLUDES}" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      DirectXShaderCompiler
##########################################
# Not use

##########################################
##      EASTL
##########################################
# Build target
file(GLOB_RECURSE EASTL_SOURCES "EASTL/*.cpp")
add_library(EASTL STATIC ${EASTL_SOURCES})

list(APPEND THE_FORGE_LIBS EASTL)
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/EASTL" "${CMAKE_SOURCE_DIR}/include/EASTL")

##########################################
##      FluidStudios
##########################################
# Build target
file(GLOB FS_SOURCES "FluidStudios/MemoryManager/*.cpp")
set(FS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/FluidStudios/MemoryManager)
add_library(FluidStudio STATIC ${FS_SOURCES})

list(APPEND THE_FORGE_LIBS FluidStudio)
copy_headers("${FS_INCLUDES}" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      Fontstash
##########################################
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Fontstash/src" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      Gainput
##########################################
set(GAINPUT_TESTS OFF)
set(GAINPUT_SAMPLES OFF)
set(GAINPUT_BUILD_SHARED OFF)
add_subdirectory(gainput)
list(APPEND THE_FORGE_LIBS gainputstatic)
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/gainput/lib/include" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      Imgui
##########################################
set(IMGUI_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDES})
list(APPEND THE_FORGE_LIBS imgui)
copy_headers("${IMGUI_INCLUDES}" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      LibZip
##########################################
set(LIB_ZIP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Linux/libzip.a)
add_custom_target(libzip_copy)
add_custom_command(TARGET libzip_copy PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIB_ZIP_FILE} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/libzip" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      Lua
##########################################
if (USE_LUA)
    # Build target
    file(GLOB LUA_SOURCES "lua-5.3.5/src/*.c")
    set(LUA_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/lua-5.3.5/src)
    add_library(Lua STATIC ${LUA_SOURCES})

    list(APPEND THE_FORGE_LIBS Lua)
    copy_headers("${LUA_INCLUDES}" "${CMAKE_SOURCE_DIR}/include")
endif()

##########################################
##      MeshOptimizer
##########################################
# Build target
file(GLOB MO_SOURCES "meshoptimizer/src/*.cpp")
add_library(MeshOptimizer STATIC ${MO_SOURCES})

list(APPEND THE_FORGE_LIBS MeshOptimizer)
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/meshoptimizer/src" "${CMAKE_SOURCE_DIR}/include")
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/meshoptimizer/tools" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      MicroProfile
##########################################
# Build target
file(GLOB MP_SOURCES "MicroProfile/*.cpp")
set(MP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/MicroProfile)
add_library(MicroProfile STATIC ${MP_SOURCES})

list(APPEND THE_FORGE_LIBS MicroProfile)
copy_headers("${MP_INCLUDES}" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      SonyMath
##########################################
# Header only
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/ModifiedSonyMath" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      STB Stuffs
##########################################
# Header only
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Nothings" "${CMAKE_SOURCE_DIR}/include")

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
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/ozz-animation/include" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      Render doc
##########################################
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/renderdoc" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      Soloud
##########################################
#file(GLOB_RECURSE SOLOUD_SOURCES "soloud20181119/src/*.cpp")
#set(SOLOUD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/soloud20181119/include")
#
#add_library(Soloud STATIC ${SOLOUD_SOURCES})
#target_include_directories(Soloud PUBLIC ${SOLOUD_INCLUDES})
#
#list(APPEND THE_FORGE_LIBS Soloud)
#copy_headers("${SOLOUD_INCLUDES}" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      SPIRV_Cross
##########################################
# Build target
file(GLOB SPIRV_SOURCES "SPIRV_Cross/*.cpp")
set(SPIRV_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/SPIRV_Cross)
add_library(SPIRV STATIC ${SPIRV_SOURCES})

list(APPEND THE_FORGE_LIBS SPIRV)
copy_headers("${SPIRV_INCLUDES}" "${CMAKE_SOURCE_DIR}/include")

##########################################
###      TaskScheduler
###########################################
## Not use

##########################################
##      tinydds
##########################################
# Header only
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/tinydds" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      tinydds
##########################################
add_library(TinyEXR STATIC TinyEXR/tinyexr.cpp)
list(APPEND THE_FORGE_LIBS TinyEXR)
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/TinyEXR" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      tinyimageformat
##########################################
# Header only
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/tinyimageformat" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      tinyktx
##########################################
# Header only
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/tinyktx" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      TinyXML2
##########################################
add_library(TinyXML2 STATIC TinyXML2/tinyxml2.cpp)
list(APPEND THE_FORGE_LIBS TinyXML2)
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/TinyXML2" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      tinydds
##########################################
add_library(volk STATIC volk/volk.c)
list(APPEND THE_FORGE_LIBS volk)
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/volk" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      VulkanMemoryAllocator
##########################################
# Header only
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator" "${CMAKE_SOURCE_DIR}/include")

##########################################
##      winpixeventruntime
##########################################
# D3DX deps (ignore for now)

list(JOIN THE_FORGE_LIBS ";" THE_FORGE_LIBS)
