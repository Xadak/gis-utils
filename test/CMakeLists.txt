add_executable(test mbr_test.cpp line_intersect_test.cpp contains_test.cpp)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(test PRIVATE ${PROJECT_NAME} project_options Catch2::Catch2WithMain)