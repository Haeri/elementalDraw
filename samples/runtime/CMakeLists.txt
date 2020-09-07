set(PROJECT_NAME runtime)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(NOT APPLE AND NOT MSVC)
  target_link_libraries(${PROJECT_NAME} PRIVATE elemd pthread ${CMAKE_DL_LIBS})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE elemd)
endif()

# Make res folder available inside the cmake build
set (lib_src "${CMAKE_SOURCE_DIR}/res")
set (lib_dest "${CMAKE_CURRENT_BINARY_DIR}/elemd_res")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${lib_src} ${lib_dest} 
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/samples/window_app/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Creating sylinks to resources folders"
)