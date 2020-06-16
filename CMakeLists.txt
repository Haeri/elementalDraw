cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME Elemental_Draw)
project(${PROJECT_NAME} VERSION 0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
#find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCE_FILES
        include/elemental_draw/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
#target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)


## Extern

set(GLAD_DIR extern/glad/include)
# Include the glm headers:
target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_DIR})