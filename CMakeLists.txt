cmake_minimum_required(VERSION 3.16)

# Features
option(ELEMD_UI "Enable DOM based ui creation and management" OFF)
if(ELEMD_UI)
  list(APPEND VCPKG_MANIFEST_FEATURES "ui")
endif()

option(ELEMD_AUDIO "Enable audio loading and playback with miniaudio" OFF)
if(ELEMD_AUDIO)
  list(APPEND VCPKG_MANIFEST_FEATURES "audio")
endif()

option(ELEMD_VIDEO "Enable video loading and playback with ffmpeg" OFF)
if(ELEMD_VIDEO)
  list(APPEND VCPKG_MANIFEST_FEATURES "video")
endif()

set(PROJECT_NAME elemd)
project(${PROJECT_NAME} LANGUAGES CXX C)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ELEMD_STANDALONE TRUE)
endif()

if(APPLE)
    set(ELEMD_DEFAULT_RENDERING_BACKEND "opengl")
    #set(ELEMD_DEFAULT_RENDERING_BACKEND "metal")
else()
    set(ELEMD_DEFAULT_RENDERING_BACKEND "vulkan")
    #set(ELEMD_DEFAULT_RENDERING_BACKEND "opengl")
endif()

# Options
option(BUILD_SHARED_LIBS "Build as shared/dynamic library" ON)
option(DYNAMIC_RUNTIME_LIBRARY "Set runtime library to dynamic" ${BUILD_SHARED_LIBS})
option(ELEMD_BUILD_EXAMPLES "Build the example programs" ${ELEMD_STANDALONE})

# Settings
set(ELEMD_RENDERING_BACKEND ${ELEMD_DEFAULT_RENDERING_BACKEND} CACHE STRING "Choose Backend (vulkan, metal, opengl)")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINE_HIDDEN YES)

# Enable folder view in solution explorer
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

string(TOUPPER ${ELEMD_RENDERING_BACKEND} ELEMD_RENDERING_BACKEND_UPPER)
add_compile_definitions("ELEMD_RENDERING_BACKEND_${ELEMD_RENDERING_BACKEND_UPPER}")

file(GLOB SOURCE_FILES
    include/${PROJECT_NAME}/*.hpp
    src/*    
)
file(GLOB_RECURSE SOURCE_FILES
    ${SOURCE_FILES}
    src/backends/${ELEMD_RENDERING_BACKEND}/*
)

if(ELEMD_UI)
    file(GLOB SOURCE_FILES
        ${SOURCE_FILES}
        include/${PROJECT_NAME}/ui/*.hpp
        src/ui/*    
    )
endif()
if(ELEMD_VIDEO)
    file(GLOB SOURCE_FILES
        ${SOURCE_FILES}
        include/${PROJECT_NAME}/video/*.hpp
        src/video/*    
    )
endif()
if(ELEMD_AUDIO)
    file(GLOB SOURCE_FILES
        ${SOURCE_FILES}
        include/${PROJECT_NAME}/audio/*.hpp
        src/audio/*    
    )
endif()

# Microsoft Runtime Library shenanigens
if(NOT DYNAMIC_RUNTIME_LIBRARY) 
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


# ffmpeg fails on linux so we use this for shared version as well
# relocation R_X86_64_PC32 against symbol `ff_pw_9' can not be used when making a shared object; recompile with -fPIC
if (ELEMD_VIDEO AND UNIX AND NOT APPLE AND BUILD_SHARED_LIBS)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
endif()


if (BUILD_SHARED_LIBS)
    add_compile_definitions(ELEMD_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    SOVERSION 0
    VERSION 0.2.1
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

message("VCPKG_MANIFEST_FEATURES: ${VCPKG_MANIFEST_FEATURES}")
message("ELEMD_RENDERING_BACKEND: ${ELEMD_RENDERING_BACKEND_UPPER}")
message("BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message("DYNAMIC_RUNTIME_LIBRARY: ${DYNAMIC_RUNTIME_LIBRARY}")
message("CMAKE_MSVC_RUNTIME_LIBRARY: ${CMAKE_MSVC_RUNTIME_LIBRARY}")

# ---------------- DEPENDENCIES ---------------------#

if(NOT ANDROID)
# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()

# freetype
find_package(freetype CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

# stb
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIRS})

# metal-cpp
if (APPLE)
    find_library(METAL_LIBRARY Metal REQUIRED)
    find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)
    find_library(NS_FOUNDATION Foundation REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY} ${NS_FOUNDATION})

    find_path(METAL_CPP_INCLUDE_DIRS "Foundation/Foundation.hpp")
    target_include_directories(${PROJECT_NAME} PRIVATE ${METAL_CPP_INCLUDE_DIRS})
endif()

if (ELEMD_VIDEO)
    # ffmpeg
    find_package(FFMPEG REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
    target_link_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})
endif()

if (ELEMD_AUDIO)
    # miniaudio
    find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")
    target_include_directories(${PROJECT_NAME} PRIVATE ${MINIAUDIO_INCLUDE_DIRS})
    
    # miniaudio needs to link stuff on linux
    if(UNIX AND NOT APPLE)
        target_link_libraries(${PROJECT_NAME} PRIVATE pthread m dl)    
    endif()
endif()

# ---------------- EXAMPLES ---------------------#

if (ELEMD_BUILD_EXAMPLES)
    add_subdirectory(examples/benchmark)
    add_subdirectory(examples/brick_breaker)
    add_subdirectory(examples/game_of_life)
    add_subdirectory(examples/hello_world)
    add_subdirectory(examples/overview)
    add_subdirectory(examples/painter)    
    add_subdirectory(examples/presenter)
    add_subdirectory(examples/window_app)
    add_subdirectory(examples/window_app_runtime)

    set_target_properties(benchmark PROPERTIES FOLDER examples)
    set_target_properties(brick-breaker PROPERTIES FOLDER examples)
    set_target_properties(game-of-life PROPERTIES FOLDER examples)
    set_target_properties(hello-world PROPERTIES FOLDER examples)
    set_target_properties(overview PROPERTIES FOLDER examples)
    set_target_properties(painter PROPERTIES FOLDER examples)
    set_target_properties(presenter PROPERTIES FOLDER examples)
    
    set_target_properties(window-app PROPERTIES FOLDER examples)
    set_target_properties(window-app-runtime PROPERTIES FOLDER examples)

    if (ELEMD_UI)
        add_subdirectory(examples/calculator)
        add_subdirectory(examples/elemx)
        add_subdirectory(examples/slack)
        add_subdirectory(examples/tasker)
        set_target_properties(calculator PROPERTIES FOLDER examples)
        set_target_properties(elemx PROPERTIES FOLDER examples)
        set_target_properties(slack PROPERTIES FOLDER examples)
        set_target_properties(tasker PROPERTIES FOLDER examples)
    endif()
    if (ELEMD_AUDIO)
        add_subdirectory(examples/roots_of_darknes)
        add_subdirectory(examples/platformer)

        set_target_properties(roots-of-darknes PROPERTIES FOLDER examples)   
        set_target_properties(platformer PROPERTIES FOLDER examples)        
    endif()
    if (ELEMD_VIDEO)
        add_subdirectory(examples/video)
        set_target_properties(video PROPERTIES FOLDER examples)
    endif()


  if (BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS benchmark brick-breaker game-of-life hello-world overview painter presenter window-app-runtime  
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/benchmark/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/brick_breaker/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/game_of_life/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/hello_world/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/overview/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}> 
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/painter/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>        
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/presenter/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}> 
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/window_app_runtime/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        COMMENT "Copy library"
    )

    if (ELEMD_UI)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            DEPENDS calculator elemx slack tasker  
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/calculator/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>             
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/elemx/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>             
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/slack/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>             
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/tasker/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>             
            COMMENT "Copy library"
        )
    endif()
    if (ELEMD_AUDIO)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            DEPENDS platformer roots-of-darknes                         
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/roots_of_darknes/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>             
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/platformer/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>             
            COMMENT "Copy library"
        )
    endif()
    if (ELEMD_VIDEO)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            DEPENDS video  
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/video/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
            COMMENT "Copy library"
        )
    endif()

  endif()
endif()



# ---------------- EXPORT ---------------------#

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
  cmake/config.cmake.in
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Target
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


install(
  EXPORT ${PROJECT_NAME}Target
  FILE  ${PROJECT_NAME}Target.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  FILES
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)