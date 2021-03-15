#pragma once

#include <opencv4/opencv2/core.hpp>
std::vector<cv::Rect> detect_face_rectangles(const cv::Mat &frame);
