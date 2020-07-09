cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME elemental_draw)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ELEMD_STANDALONE TRUE)
endif()

option(BUILD_SHARED_LIBS "Build as shared/dynamic library" OFF)
option(ELEMD_BUILD_EXAMPLES "Build the example programs" ${ELEMD_STANDALONE})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCE_FILES
        include/elemental_draw/*.hpp
        src/*.hpp
        src/*.cpp)

if (BUILD_SHARED_LIBS)
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

# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

set_target_properties(glad PROPERTIES FOLDER ${EXTERN_DIR})