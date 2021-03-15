add_subdirectory(utils)
add_subdirectory(opt_parser)
add_subdirectory(face_detector)

add_executable(generic-video main.cpp)
target_link_libraries(generic-video PRIVATE spdlog::spdlog utils opt_parser face_detector opencv_core opencv_highgui)
target_include_directories(generic-video PRIVATE .)
