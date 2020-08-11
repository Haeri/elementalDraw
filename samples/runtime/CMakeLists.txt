set(PROJECT_NAME runtime)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
        src/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(not APPLE and not MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE elemd ${CMAKE_DL_LIBS})