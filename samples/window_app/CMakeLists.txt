set(PROJECT_NAME window_app)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Make res folder available inside the cmake build
set (lib_src "${CMAKE_SOURCE_DIR}/res")
set (lib_dest "${CMAKE_CURRENT_BINARY_DIR}/elemd_res")
set (src "${CMAKE_CURRENT_SOURCE_DIR}/res")
set (dest "${CMAKE_CURRENT_BINARY_DIR}/res")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${lib_src} ${lib_dest} 
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest} 
    COMMENT "Creating sylinks to resources folders"
)

target_link_libraries(${PROJECT_NAME} PRIVATE elemd)