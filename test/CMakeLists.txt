find_package(Catch2 REQUIRED)
find_package(spdlog REQUIRED)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE spdlog::spdlog Catch2::Catch2WithMain opt_parser)
target_include_directories(tests PRIVATE ../src)

include(Catch)
catch_discover_tests(tests)
