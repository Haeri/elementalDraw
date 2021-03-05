cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME elemd)
project(${PROJECT_NAME} LANGUAGES CXX C)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ELEMD_STANDALONE TRUE)
endif()

if(APPLE)
    set(ELEMD_DEFAULT_RENDERING_BACKEND "opengl")
else()
    set(ELEMD_DEFAULT_RENDERING_BACKEND "vulkan")
endif()

# Options
option(BUILD_SHARED_LIBS "Build as shared/dynamic library" ON)
option(ELEMD_BUILD_EXAMPLES "Build the example programs" ${ELEMD_STANDALONE})
option(DYNAMIC_RUNTIME_LIBRARY "Set runtime library to dynamic" ${BUILD_SHARED_LIBS})
set(ELEMD_RENDERING_BACKEND ${ELEMD_DEFAULT_RENDERING_BACKEND} CACHE STRING "Choose Backend (vulkan, opengl)")
string(TOUPPER ${ELEMD_RENDERING_BACKEND} ELEMD_RENDERING_BACKEND_UPPER)

# Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINE_HIDDEN YES)

# Enable folder view in solution explorer
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_compile_definitions("ELEMD_RENDERING_BACKEND_${ELEMD_RENDERING_BACKEND_UPPER}")

file(GLOB SOURCE_FILES
    include/${PROJECT_NAME}/*.hpp
    src/*    
 )
 file(GLOB_RECURSE SOURCE_FILES
    ${SOURCE_FILES}
    src/${ELEMD_RENDERING_BACKEND}/*
 )

# Microsoft Runtime Library shenanigens
if(NOT DYNAMIC_RUNTIME_LIBRARY) 
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
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
    VERSION 0.1.1
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

message("ELEMD_RENDERING_BACKEND: " ${ELEMD_RENDERING_BACKEND_UPPER})
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




# ---------------- EXAMPLES ---------------------#

if(APPLE)
    set(CONVERTER_POSTFIX "mac.sh")
    set(SHARED_LIB_NAME "lib${PROJECT_NAME}.dylib")
elseif(WIN32) 
    set(CONVERTER_POSTFIX "win.bat")
    set(SHARED_LIB_NAME "${PROJECT_NAME}.dll")
else()
    set(CONVERTER_POSTFIX "linux.sh")
    set(SHARED_LIB_NAME "lib${PROJECT_NAME}.so")
endif()

if (ELEMD_BUILD_EXAMPLES)
    add_subdirectory(examples/benchmark)
    add_subdirectory(examples/brick_breaker)
    add_subdirectory(examples/game_of_life)
    add_subdirectory(examples/hello_world)
    add_subdirectory(examples/overview)
    add_subdirectory(examples/painter)
    add_subdirectory(examples/window_app)
    add_subdirectory(examples/window_app_runtime)

    set_target_properties(benchmark PROPERTIES FOLDER examples)
    set_target_properties(brick-breaker PROPERTIES FOLDER examples)
    set_target_properties(game-of-life PROPERTIES FOLDER examples)
    set_target_properties(hello-world PROPERTIES FOLDER examples)
    set_target_properties(overview PROPERTIES FOLDER examples)
    set_target_properties(painter PROPERTIES FOLDER examples)
    set_target_properties(window-app PROPERTIES FOLDER examples)
    set_target_properties(window-app-runtime PROPERTIES FOLDER examples)

  if (BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS window-app-runtime painter brick-breaker hello-world overview game-of-life
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/benchmark/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/brick_breaker/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/game_of_life/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/hello_world/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/overview/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/painter/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/window_app_runtime/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMENT "Copy library"
    )
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
