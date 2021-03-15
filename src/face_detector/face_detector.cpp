#include <sstream>
#include <string>
#include <vector>

#include <opencv4/opencv2/opencv.hpp>
#include <spdlog/fmt/fmt.h>

#include "face_detector.hpp"

std::vector<cv::Rect> detect_face_rectangles(const cv::Mat& frame) {
    auto network = cv::dnn::readNetFromCaffe(FACE_DETECTION_CONFIGURATION, FACE_DETECTION_WEIGHTS);
    network.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    network.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    if (network.empty()) {
        throw std::invalid_argument(fmt::format(FMT_STRING("failed to load network with weight ({}) and model ({})"),
                                                FACE_DETECTION_WEIGHTS, FACE_DETECTION_CONFIGURATION));
    }

    auto const input_size = cv::Size(300, 300);
    constexpr auto scale_factor = 1.0;
    auto const mean_values = cv::Scalar(104.0, 177.0, 123.0);
    constexpr auto confidence_threshold = 0.5F;

    auto input_blob = cv::dnn::blobFromImage(frame, scale_factor, input_size, mean_values, false, false);
    network.setInput(input_blob, "data");
    cv::Mat detection = network.forward("detection_out");
    cv::Mat detection_matrix(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    auto faces = std::vector<cv::Rect>();

    for (auto i = 0; i < detection_matrix.rows; i++) {
        auto confidence = detection_matrix.at<float>(i, 2);

        if (confidence < confidence_threshold) {
            continue;
        }
        auto const x_left_bottom = static_cast<int>(detection_matrix.at<float>(i, 3) * static_cast<float>(frame.cols));
        auto const y_left_bottom = static_cast<int>(detection_matrix.at<float>(i, 4) * static_cast<float>(frame.rows));
        auto const x_right_top = static_cast<int>(detection_matrix.at<float>(i, 5) * static_cast<float>(frame.cols));
        auto const y_right_top = static_cast<int>(detection_matrix.at<float>(i, 6) * static_cast<float>(frame.rows));

        faces.emplace_back(x_left_bottom, y_left_bottom, (x_right_top - x_left_bottom), (y_right_top - y_left_bottom));
    }

    return faces;
}
