cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0077 NEW) # for option overriding
project(TheForge)

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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################
##      Turn on/off verbose
##########################################
set(CMAKE_VERBOSE_MAKEFILE FALSE)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

##########################################
##      Compilation options
##########################################
option(USE_ASSIMP "Build package with assimp support." ON)
option(USE_LUA "Build package with lua support." OFF)
option(COMPILE_UNIT_TESTS "Build unit tests." ON)
option(COMPILE_SAMPLE "Build visibility buffer sample." ON)

option(VULKAN "Build with vulkan support" ON)
option(METAL "Build with metal support" ON)
option(DIRECT3D11 "Build with D3DX11 support" OFF)
option(DIRECT3D12 "Build with D3DX12 support" OFF)

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

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/include")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/include/TheForge")
    copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Common_3/OS" "${CMAKE_SOURCE_DIR}/include/TheForge/OS")
    copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Common_3/Renderer" "${CMAKE_SOURCE_DIR}/include/TheForge/Renderer")
    copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Common_3/Tools" "${CMAKE_SOURCE_DIR}/include/TheForge/Tools")
    copy_headers("${CMAKE_CURRENT_SOURCE_DIR}/Middleware_3" "${CMAKE_SOURCE_DIR}/include/TheForge")
endif()
include_directories(${CMAKE_SOURCE_DIR}/include)

##########################################
##      System deps
##########################################
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIR})
link_libraries(${Vulkan_LIBRARIES})
add_definitions(-DVULKAN=TRUE)

if (UNIX)
    find_package(X11 REQUIRED)
    find_package(ALSA REQUIRED) # sudo apt install libclalsadrv-dev
    include_directories(${X11_INCLUDE_DIR} ${ALSA_INCLUDE_DIR})
    link_libraries(${X11_LIBRARIES} ${ALSA_LIBRARIES} -pthread)
endif()

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
        Common_3/OS/FileSystem/UnixFileSystem.cpp
        Common_3/OS/FileSystem/ZipFileStream.cpp
        Common_3/OS/FileSystem/ZipFileSystem.cpp
        Common_3/OS/Image/Image.cpp
        Common_3/OS/Input/InputSystem.cpp
        Common_3/OS/Logging/Log.cpp
        Common_3/OS/MemoryTracking/MemoryTracking.cpp
        Common_3/Renderer/CommonShaderReflection.cpp
        Common_3/Renderer/GpuProfiler.cpp
        Common_3/Renderer/ResourceLoader.cpp
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
            Middleware_3/LUA/LunaV.hpp
    )
endif()
if (USE_ASSIMP)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Common_3/OS/FileSystem/AssimpIOStream.cpp
            Common_3/OS/FileSystem/AssimpIOSystem.cpp
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
            Common_3/OS/Windows/WindowsThread.cpp
            Common_3/OS/Windows/WindowsTime.cpp
            ${THE_FORGE_D3D11_SOURCES}
            ${THE_FORGE_D3D12_SOURCES}
            ${THE_FORGE_VULKAN_SOURCES}
    )
endif()

##########################################
##      Make sources linux specific
##########################################
if (UNIX)
    set(THE_FORGE_SOURCES
            ${THE_FORGE_SOURCES}
            Common_3/OS/Linux/LinuxBase.cpp
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
##      Combine all static libs
##########################################
set(THE_FORGE_LIBS "${THE_FORGE_LIBS};TheForge-static")
set(THE_FORGE_LIBS_COMBINED)
foreach(LIB ${THE_FORGE_LIBS} zip)
    set(THE_FORGE_LIBS_COMBINED "${THE_FORGE_LIBS_COMBINED};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_STATIC_LIBRARY_PREFIX}${LIB}${CMAKE_STATIC_LIBRARY_SUFFIX}")
endforeach()

set(THE_FORGE_LIB "${CMAKE_STATIC_LIBRARY_PREFIX}TheForge${CMAKE_STATIC_LIBRARY_SUFFIX}")

add_custom_target(
        ${THE_FORGE_LIB}
        ALL
        COMMAND ${CMAKE_AR} -rcT ${THE_FORGE_LIB} ${THE_FORGE_LIBS_COMBINED}
        DEPENDS ${THE_FORGE_LIBS} libzip_copy
)

##########################################
##      Finalize
##########################################
add_library(TheForge STATIC IMPORTED)
set_target_properties(TheForge PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/${THE_FORGE_LIB})
add_dependencies(TheForge ${THE_FORGE_LIB})
link_libraries(TheForge)

##########################################
##      Unit tests & Samples
##########################################
if (COMPILE_UNIT_TESTS)
    add_subdirectory(Examples_3/Unit_Tests/src)
endif()
if (COMPILE_SAMPLE)
    add_subdirectory(Examples_3/Visibility_Buffer/src)
endif()
