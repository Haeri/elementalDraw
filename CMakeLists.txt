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

# Glad
set(GLAD_DIR extern/glad)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_DIR}/include)
add_subdirectory(${GLAD_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# Glfw3
set(GLFW_DIR extern/glfw3)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)


set_target_properties(glfw glad PROPERTIES FOLDER extern)