cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME elemental_draw)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ELEMD_STANDALONE TRUE)
endif()

option(ELEMD_BUILD_SHARED "Build as shared/dynamic library" OFF)
option(ELEMD_BUILD_EXAMPLES "Build the example programs" ${ELEMD_STANDALONE})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCE_FILES
        include/elemental_draw/*.hpp
        src/*.hpp
        src/*.cpp)

if (ELEMD_BUILD_SHARED)
    add_compile_definitions(ELEMD_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)


if (ELEMD_BUILD_EXAMPLES)
    add_subdirectory(samples)
endif()


# ---------------- EXTERN ---------------------#

set(EXTERN_DIR extern/)

# glad
add_subdirectory(${EXTERN_DIR}glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# glslang
#set(SKIP_GLSLANG_INSTALL OFF CACHE BOOL "" FORCE)
#set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
#set(ENABLE_CTEST OFF CACHE BOOL "" FORCE)
#set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)
#add_subdirectory(${EXTERN_DIR}glslang)
#target_link_libraries(${PROJECT_NAME} PRIVATE glslang SPIRV)


# glfw3
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
#add_subdirectory(${EXTERN_DIR}glfw3)
#target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)


#set_target_properties(glfw glslang glslangValidator SPIRV glad PROPERTIES FOLDER ${EXTERN_DIR})