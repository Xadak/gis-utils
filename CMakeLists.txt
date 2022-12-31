cmake_minimum_required(VERSION 3.22.2)
project(gis-utils LANGUAGES CXX)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
target_compile_definitions(project_options INTERFACE $<$<CONFIG:Debug>:DEBUG_BUILD>)
if(GCC)
target_compile_options(project_options INTERFACE
  -Wall -Wextra -Wpedantic -Wconversion)
elseif(MSVC)
target_compile_options(project_options INTERFACE /W3 /WX)
endif()

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/line_segment.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/rectangle.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/gis-utils)


target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} project_options)

add_subdirectory(test)