set(PROJECT_NAME presenter)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

# Make res folder available inside the cmake build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copying resource files"
)

target_link_libraries(${PROJECT_NAME} PRIVATE elemd)