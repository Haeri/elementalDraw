set(PROJECT_NAME overview)
project(${PROJECT_NAME})

if(WIN32) 
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/res/app.rc")
else()
    set(APP_ICON_RESOURCE_WINDOWS "")
endif()

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp
        ${APP_ICON_RESOURCE_WINDOWS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copying resource files"
)

target_link_libraries(${PROJECT_NAME} PRIVATE elemd)