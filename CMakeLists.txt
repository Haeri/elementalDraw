cmake_minimum_required(VERSION 3.3)

set(PROJECT_NAME elemd)
project(${PROJECT_NAME} LANGUAGES CXX C)

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

# Enable folder view in solution explorer
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCE_FILES
    include/${PROJECT_NAME}/*.hpp
    src/*.hpp
    src/*.h
    src/*.cpp
    src/*.c
 )

if (BUILD_SHARED_LIBS)
    add_compile_definitions(ELEMD_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    SOVERSION 0
    VERSION 0.1.0
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

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


#find_program(GLSLANGVALIDATOR_EXECUTABLE NAMES glslangValidator PATHS ${CMAKE_SOURCE_DIR}/external/vcpkg/installed/tools/)

#add_custom_command(
#    TARGET ${PROJECT_NAME}
#    PRE_BUILD
#    COMMAND ${GLSLANGVALIDATOR_EXECUTABLE} -V ${CMAKE_SOURCE_DIR}/data/shader/rounded_rect.frag -o ${CMAKE_SOURCE_DIR}/data/shader/rounded_rect.frag.spv
#    COMMAND ${GLSLANGVALIDATOR_EXECUTABLE} -V ${CMAKE_SOURCE_DIR}/data/shader/rounded_rect.vert -o ${CMAKE_SOURCE_DIR}/data/shader/rounded_rect.vert.spv
#)





# ---------------- DEPENDENCIES ---------------------#

# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# freetype
find_package(freetype CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)




# ---------------- EXAMPLES ---------------------#

if (ELEMD_BUILD_EXAMPLES)
    add_subdirectory(examples/runtime)
    add_subdirectory(examples/window_app)
    add_subdirectory(examples/brick_breaker)
    add_subdirectory(examples/painter)

    set_target_properties(runtime PROPERTIES FOLDER examples)
    set_target_properties(window_app PROPERTIES FOLDER examples)
    set_target_properties(brick_breaker PROPERTIES FOLDER examples)
    set_target_properties(painter PROPERTIES FOLDER examples)

  if (BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS runtime painter brick_breaker
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/brick_breaker/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/runtime/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/painter/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
        COMMENT "Copy library"
    )
  endif()
endif()



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
