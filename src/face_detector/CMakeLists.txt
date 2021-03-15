find_package(OpenCV REQUIRED COMPONENTS dnn)

add_library(face_detector face_detector.cpp)
target_link_libraries(face_detector PUBLIC project_options project_warnings opencv_dnn)

target_compile_definitions(face_detector PRIVATE FACE_DETECTION_CONFIGURATION="${PROJECT_SOURCE_DIR}/assets/deploy.prototxt")
target_compile_definitions(face_detector PRIVATE FACE_DETECTION_WEIGHTS="${PROJECT_SOURCE_DIR}/assets/res10_300x300_ssd_iter_140000_fp16.caffemodel")
