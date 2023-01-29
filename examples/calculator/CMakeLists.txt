set(PROJECT_NAME calculator)
project(${PROJECT_NAME})

if(WIN32) 
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/res/icon.rc")
else()
    set(APP_ICON_RESOURCE_WINDOWS "")
endif()
if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp
        ${APP_ICON_RESOURCE_WINDOWS}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Make res folder available inside the cmake build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/examples/calculator/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copying resource files"
)

target_link_libraries(${PROJECT_NAME} PRIVATE elemd)