cmake_minimum_required(VERSION 3.3)

set(PROJECT_NAME elemd)
project(${PROJECT_NAME} LANGUAGES CXX)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ELEMD_STANDALONE TRUE)
endif()

# Options
option(BUILD_SHARED_LIBS "Build as shared/dynamic library" ON)
option(ELEMD_BUILD_EXAMPLES "Build the example programs" ${ELEMD_STANDALONE})

# Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINE_HIDDEN YES)

# Collect all binaries in one folder so no copying is necessary
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Enable folder view in solution explorer
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCE_FILES
        include/${PROJECT_NAME}/*.hpp
        src/*.hpp
        src/*.cpp)

if (BUILD_SHARED_LIBS)
    add_compile_definitions(ELEMD_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    SOVERSION 0
    VERSION 0.0.1
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

if(APPLE)
    set(CONVERTER_POSTFIX "mac.sh")
elseif(WIN32) 
    set(CONVERTER_POSTFIX "win.bat")
else()
    set(CONVERTER_POSTFIX "linux.sh")
endif()

add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/convert_assets_${CONVERTER_POSTFIX} -s
)


# ---------------- EXTERN ---------------------#

set(EXTERN_DIR extern/)

# glad
add_subdirectory(${EXTERN_DIR}glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

set_target_properties(glad PROPERTIES FOLDER ${EXTERN_DIR})

# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)



# ---------------- EXAMPLES ---------------------#

if (ELEMD_BUILD_EXAMPLES)
    add_subdirectory(samples/runtime)
    add_subdirectory(samples/window_app)
    add_subdirectory(samples/brick_breaker)
endif()


install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        RUNTIME DESTINATION "bin"
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib/static")