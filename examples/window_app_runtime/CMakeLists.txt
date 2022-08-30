set(PROJECT_NAME window-app-runtime)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

if(NOT APPLE AND NOT MSVC)
  target_link_libraries(${PROJECT_NAME} PRIVATE elemd window-app pthread ${CMAKE_DL_LIBS})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE elemd window-app)
endif()

# Make res folder available inside the cmake build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/examples/window_app/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copying resource files"
)