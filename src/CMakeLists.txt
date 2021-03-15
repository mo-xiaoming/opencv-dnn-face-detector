add_subdirectory(opt_parser)
add_subdirectory(face_detector)

find_package(spdlog REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core highgui)

add_executable(generic-video main.cpp)
target_link_libraries(generic-video PRIVATE spdlog::spdlog opt_parser face_detector opencv_core opencv_highgui)
target_include_directories(generic-video PRIVATE .)
