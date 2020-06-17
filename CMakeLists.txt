cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME Elemental_Draw)
project(${PROJECT_NAME} VERSION 0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
        include/elemental_draw/*.hpp
        src/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)


# ---------------- EXTERN ---------------------#

# Glad
set(GLAD_DIR extern/glad)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_DIR}/include)
add_subdirectory(${GLAD_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

# Glfw3
set(GLFW_DIR extern/glfw3)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR}/include)
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)


set_target_properties(glfw glad PROPERTIES FOLDER extern)