set(PROJECT_NAME brick_breaker)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp)

#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Make res folder available inside the cmake build
set (src "${CMAKE_CURRENT_SOURCE_DIR}/res")
set (dest "${CMAKE_CURRENT_BINARY_DIR}/res")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${src} ${dest} 
    COMMENT "Copying resource files"
)

target_link_libraries(${PROJECT_NAME} PRIVATE elemd)