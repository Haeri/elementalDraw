set(PROJECT_NAME video)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Make res folder available inside the cmake build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copying resource files"
)

target_link_libraries(${PROJECT_NAME} PRIVATE elemd)