set(PROJECT_NAME elemd-test)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE elemd)