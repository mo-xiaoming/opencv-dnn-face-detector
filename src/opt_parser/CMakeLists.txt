add_library(opt_parser opt_parser.cpp)
target_link_libraries(opt_parser PUBLIC project_options project_warnings)

add_executable(opt_parser_test opt_parser_test.cpp)
target_link_libraries(opt_parser_test PRIVATE spdlog::spdlog Catch2::Catch2WithMain opt_parser)
target_include_directories(opt_parser_test PRIVATE ..)
catch_discover_tests(opt_parser_test)
