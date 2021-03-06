cmake_minimum_required(VERSION 3.13)

project(TheForge)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0077 NEW) # for option overriding
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


##########################################
##      Configure our build system
##########################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

string(TOUPPER ${CMAKE_BUILD_TYPE} CONFIG)
set(LIB_POSTFIX ${CMAKE_${CONFIG}_POSTFIX})

##########################################
##      Verbose mode
##########################################
if (VERBOSE)
    set(CMAKE_VERBOSE_MAKEFILE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -Wl,--verbose)
endif()


# Generic optimizations
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -flto -fwhole-program")
# Experimental options, should be used with caution
# Good topic https://stackoverflow.com/questions/14492436/g-optimization-beyond-o3-ofast
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funroll-loops -funroll-all-loops")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native")

##########################################
##      Compilation options
##########################################
option(USE_ASSIMP "Build package with assimp support." ON)
option(USE_LUA "Build package with lua support." ON)
option(COMPILE_UNIT_TESTS "Build unit tests." ON)
option(COMPILE_SAMPLE "Build visibility buffer sample." ON)

option(VULKAN "Build with vulkan support" ON)
option(METAL "Build with metal support" OFF)
option(DIRECT3D11 "Build with D3DX11 support" OFF)
option(DIRECT3D12 "Build with D3DX12 support" OFF)

##########################################
##      Add definitions
##########################################
set(COMPILE_DEFINITIONS)
if (${VULKAN})
    list(APPEND COMPILE_DEFINITIONS VULKAN=TRUE)
endif()
if (${METAL})
    list(APPEND COMPILE_DEFINITIONS METAL=TRUE)
endif()
if (${DIRECT3D11})
    list(APPEND COMPILE_DEFINITIONS DIRECT3D11=TRUE)
endif()
if (${DIRECT3D12})
    list(APPEND COMPILE_DEFINITIONS DIRECT3D12=TRUE)
endif()

# Definition for fixing cross compiling with GCC or CLANG for ModifiedSonyMath lib
list(APPEND COMPILE_DEFINITIONS CPP14=TRUE)
list(APPEND COMPILE_DEFINITIONS CPP11=TRUE)

list(APPEND COMPILE_DEFINITIONS USE_MEMORY_TRACKING=TRUE)
add_compile_definitions(${COMPILE_DEFINITIONS})

##########################################
##      System deps
##########################################
set(SYSTEM_DEPS)
if (VULKAN)
    if (WIN32)
        include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/VulkanSDK/WinVulkan/Include")
        list(APPEND SYSTEM_DEPS "${CMAKE_SOURCE_DIR}/VulkanSDK/WinVulkan/Lib/vulkan-1.lib")
    elseif(ANDROID)
        find_package(Vulkan CONFIG)
        include_directories(SYSTEM ${Vulkan_INCLUDE_DIR})
        list(APPEND SYSTEM_DEPS -lvulkan)
    else()
        find_package(Vulkan CONFIG)
        include_directories(SYSTEM ${Vulkan_INCLUDE_DIR})
        list(APPEND SYSTEM_DEPS -lvulkan)
    endif()
endif()
if (WIN32)
    list(APPEND SYSTEM_DEPS "/usr/x86_64-w64-mingw32/lib/libz.a")
else()
    list(APPEND SYSTEM_DEPS -lz)
endif ()
list(APPEND SYSTEM_DEPS ${CMAKE_DL_LIBS})

if (UNIX AND NOT ANDROID AND NOT MINGW)
    find_package(X11 REQUIRED)
    find_package(ALSA REQUIRED) # sudo apt install libclalsadrv-dev
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    include_directories(SYSTEM ${X11_INCLUDE_DIR} ${ALSA_INCLUDE_DIR})
    list(APPEND SYSTEM_DEPS ${X11_LIBRARIES} ${ALSA_LIBRARIES} -pthread)
endif()

##########################################
##      Include TheForge dirs
##########################################
macro(copy_headers SRC_DIR DST_DIR)
    file(GLOB_RECURSE HEADERS "${SRC_DIR}/*.h" "${SRC_DIR}/*.hpp" "${SRC_DIR}/*.inl")
    foreach(SRC_FILE ${HEADERS})
        string(REPLACE ${SRC_DIR} ${DST_DIR} DST_FILE ${SRC_FILE})
        get_filename_component(DST_DIR2 ${DST_FILE} DIRECTORY)
        file(COPY ${SRC_FILE} DESTINATION ${DST_DIR2})
    endforeach()
endmacro()

if (EXISTS "${CMAKE_SOURCE_DIR}/include")
    file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/include")
endif()
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/include/TheForge")
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Common_3/OS"          "${CMAKE_SOURCE_DIR}/include/TheForge/OS")
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Common_3/Renderer"    "${CMAKE_SOURCE_DIR}/include/TheForge/Renderer")
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Common_3/Tools"       "${CMAKE_SOURCE_DIR}/include/TheForge/Tools")
copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Middleware_3"         "${CMAKE_SOURCE_DIR}/include/TheForge")

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/include)

##########################################
##      Resources
##########################################
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Resources")
file(COPY   Middleware_3/PaniniProjection/Shaders     DESTINATION     ${CMAKE_BINARY_DIR}/Resources/PaniniProjection)
file(COPY   Middleware_3/ParallelPrimitives/Shaders   DESTINATION     ${CMAKE_BINARY_DIR}/Resources/ParallelPrimitives)
file(COPY   Middleware_3/Text/Shaders                 DESTINATION     ${CMAKE_BINARY_DIR}/Resources/Text)
file(COPY   Middleware_3/UI/Shaders                   DESTINATION     ${CMAKE_BINARY_DIR}/Resources/UI)

##########################################
##      Compile nested deps
##########################################
set(BUILD_SHARED_LIBS CACHE BOOL OFF)
# Turn off warnings
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -w ")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W0 ")
endif()
# Include sub dir
add_subdirectory(Common_3/ThirdParty/OpenSource)
# Turn warning back
#if ( CMAKE_COMPILER_IS_GNUCC )
#    string(REPLACE " -w " " -Wall -Wextra " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#endif()
#if ( MSVC )
#    string(REPLACE " /W0 " " /W4 " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#endif()

get_directory_property(THE_FORGE_LIBS DIRECTORY Common_3/ThirdParty/OpenSource DEFINITION THE_FORGE_LIBS)
get_directory_property(THE_FORGE_SHARED_LIBS DIRECTORY Common_3/ThirdParty/OpenSource DEFINITION THE_FORGE_SHARED_LIBS)

##########################################
##      Make sources
##########################################
set(THE_FORGE_SOURCES
        Common_3/OS/Camera/CameraController.cpp
        Common_3/OS/Core/ThreadSystem.cpp
        Common_3/OS/Core/Timer.cpp
        Common_3/OS/FileSystem/FileSystemInternal.cpp
        Common_3/OS/FileSystem/MemoryStream.cpp
        Common_3/OS/FileSystem/SystemFileStream.cpp
        Common_3/OS/FileSystem/SystemRun.cpp
        Common_3/OS/FileSystem/ZipFileStream.cpp
        Common_3/OS/FileSystem/ZipFileSystem.cpp
        Common_3/OS/Image/Image.cpp
        Common_3/OS/Input/InputSystem.cpp
        Common_3/OS/Logging/Log.cpp
        Common_3/OS/MemoryTracking/MemoryTracking.cpp
        Common_3/Renderer/CommonShaderReflection.cpp
        Common_3/Renderer/GpuProfiler.cpp
        Common_3/Renderer/ResourceLoader.cpp
        Common_3/Tools/AssetPipeline/src/AssetLoader.cpp
        Common_3/Tools/AssetPipeline/src/AssetPipeline.cpp
        Common_3/Tools/AssetPipeline/src/gltfpack.cpp
        Common_3/Tools/AssetPipeline/src/TFXImporter.cpp
        Common_3/Tools/AssetPipeline/src/TressFXAsset.cpp
        Common_3/Tools/SpirvTools/SpirvTools.cpp
        Middleware_3/Animation/AnimatedObject.cpp
        Middleware_3/Animation/Animation.cpp
        Middleware_3/Animation/Clip.cpp
        Middleware_3/Animation/ClipController.cpp
        Middleware_3/Animation/ClipMask.cpp
        Middleware_3/Animation/Rig.cpp
        Middleware_3/Animation/SkeletonBatcher.cpp
        Middleware_3/ECS/BaseComponent.cpp
        Middleware_3/ECS/ComponentRepresentation.cpp
        Middleware_3/ECS/EntityManager.cpp
        Middleware_3/PaniniProjection/Panini.cpp
        Middleware_3/ParallelPrimitives/Shaders/Metal/ParallelPrimitives.comp.metal
        Middleware_3/ParallelPrimitives/ParallelPrimitives.cpp
        Middleware_3/Text/Fontstash.cpp
        Middleware_3/UI/AppUI.cpp
        Middleware_3/UI/imgui_user.cpp
        Middleware_3/UI/ImguiGUIDriver.cpp
)

if (USE_LUA)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Middleware_3/LUA/LuaManager.cpp
            Middleware_3/LUA/LuaManagerImpl.cpp
    )
endif()
if (USE_ASSIMP)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Common_3/OS/FileSystem/AssimpIOStream.cpp
            Common_3/OS/FileSystem/AssimpIOSystem.cpp
            Common_3/Tools/AssimpImporter/AssimpImporter.cpp
    )
endif()

##########################################
##      Vulkan renderer
##########################################
set(THE_FORGE_VULKAN_SOURCES
        Common_3/Renderer/Vulkan/Vulkan.cpp
        Common_3/Renderer/Vulkan/VulkanRaytracing.cpp
        Common_3/Renderer/Vulkan/VulkanShaderReflection.cpp
)

##########################################
##      Metal renderer
##########################################
set(THE_FORGE_METAL_SOURCES
        Common_3/Renderer/Metal/MetalRaytracing.mm
        Common_3/Renderer/Metal/MetalRenderer.mm
        Common_3/Renderer/Metal/MetalShaderReflection.mm
)

##########################################
##      D3D11 renderer
##########################################
set(THE_FORGE_D3D11_SOURCES
        Common_3/Renderer/Direct3D11/Direct3D11.cpp
        Common_3/Renderer/Direct3D11/Direct3D11Raytracing.cpp
        Common_3/Renderer/Direct3D11/Direct3D11ShaderReflection.cpp
)

##########################################
##      D3D12 renderer
##########################################
set(THE_FORGE_D3D12_SOURCES
        Common_3/Renderer/Direct3D12/Direct3D12.cpp
        Common_3/Renderer/Direct3D12/Direct3D12Hooks.cpp
        Common_3/Renderer/Direct3D12/Direct3D12MemoryAllocator.cpp
        Common_3/Renderer/Direct3D12/Direct3D12Raytracing.cpp
        Common_3/Renderer/Direct3D12/Direct3D12ShaderReflection.cpp
)

##########################################
##      Make sources mac specific
##########################################
if (APPLE OR IOS)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Common_3/OS/Darwin/CocoaFileSystem.mm
            Common_3/OS/Darwin/DarwinLog.cpp
            Common_3/OS/Darwin/DarwinThread.cpp
            Common_3/OS/Darwin/iOSAppDelegate.m
            Common_3/OS/Darwin/iOSBase.mm
            Common_3/OS/Darwin/iOSFileSystem.mm
            Common_3/OS/Darwin/macOSAppDelegate.m
            Common_3/OS/Darwin/macOSBase.mm
            Common_3/OS/Darwin/macOSFileSystem.mm
            Common_3/OS/macOS/macOSLogManager.cpp
            ${THE_FORGE_METAL_SOURCES}
    )
endif()

##########################################
##      Make sources android specific
##########################################
if (ANDROID)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Common_3/OS/Android/AndroidBase.cpp
            Common_3/OS/Android/AndroidFileSystem.cpp
            Common_3/OS/Android/AndroidLog.cpp
            Common_3/OS/Android/AndroidThread.cpp
            Common_3/OS/Android/AndroidTime.cpp
            ${THE_FORGE_VULKAN_SOURCES}
    )
endif()

##########################################
##      Make sources win specific
##########################################
if (WIN32 OR MSVC)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Common_3/OS/Windows/WindowsBase.cpp
            Common_3/OS/Windows/WindowsFileSystem.cpp
            Common_3/OS/Windows/WindowsLog.cpp
            Common_3/OS/Windows/WindowsTime.cpp
            Common_3/OS/Windows/WindowsThread.cpp
            ${THE_FORGE_D3D11_SOURCES}
            ${THE_FORGE_D3D12_SOURCES}
            ${THE_FORGE_VULKAN_SOURCES}
    )
endif()

##########################################
##      Make sources linux specific
##########################################
if (UNIX AND NOT WIN32)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Common_3/OS/Linux/LinuxBase.cpp
            Common_3/OS/FileSystem/UnixFileSystem.cpp
            Common_3/OS/Linux/LinuxFileSystem.cpp
            Common_3/OS/Linux/LinuxLog.cpp
            Common_3/OS/Linux/LinuxThread.cpp
            Common_3/OS/Linux/LinuxTime.cpp
            ${THE_FORGE_VULKAN_SOURCES}
    )
endif()

##########################################
##      Static lib target
##########################################
add_library(TheForge-static STATIC ${THE_FORGE_SOURCES})
target_link_libraries(TheForge-static PUBLIC ${THE_FORGE_LIBS} zip)

##########################################
##   Link all libs in the correct order
##########################################
list(APPEND THE_FORGE_LIBS zip TheForge-static ${SYSTEM_DEPS})
link_libraries(${THE_FORGE_LIBS})

##########################################
##      Unit tests & Samples
##########################################
if (COMPILE_UNIT_TESTS)
    add_subdirectory(Examples_3/Unit_Tests/src)
endif()
if (COMPILE_SAMPLE)
    add_subdirectory(Examples_3/Visibility_Buffer/src)
endif()
