#include <chrono>
#include <csignal>
#include <iomanip>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

#include <face_detector/face_detector.hpp>
#include <utils/frame_counter.hpp>

namespace {
volatile std::sig_atomic_t signal_received = 0; // NOLINT
void sig_handler(int signal) {
    if (signal == SIGINT) {
        spdlog::debug("received SIGINT");
        signal_received = signal;
    }
}
} // namespace

int main(int /*argc*/, char** /*argv*/) {
    if (std::signal(SIGINT, sig_handler) == SIG_ERR) { // NOLINT
        spdlog::error("can't catch SIGINT");
    }

    auto video_capture = cv::VideoCapture(
        "nvarguscamerasrc sensor_id=0 ! "
        "video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)NV12, framerate=(fraction)60/1 ! "
        "nvvidconv flip-method=0 ! "
        "video/x-raw, width=(int)640, height=(int)360, format=(string)BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=(string)BGR ! "
        "appsink",
        cv::CAP_GSTREAMER);
    if (!video_capture.isOpened()) {
        return 1;
    }

    constexpr const char* win_name = "Image";
    cv::namedWindow(win_name); // | cv::WINDOW_OPENGL);

    auto frame_counter = utils::Frame_counter(
        10, +[](double fps) { std::cout << std::setprecision(4) << "\rFPS: " << fps; });

    auto save_nr = 0;

    auto frame = cv::Mat();
    while (signal_received == 0) {
        if (!video_capture.read(frame)) {
            break;
        }

        auto const rectangles = detect_face_rectangles(frame);
        auto const color = cv::Scalar(0, 105, 205);
        for (auto const& r : rectangles) {
            cv::rectangle(frame, r, color, 4);
        }

        cv::imshow(win_name, frame);
        switch (static_cast<char>(cv::waitKey(1))) {
        case 'q':
        case 'Q':
        case 27:
            spdlog::info("Quiting...");
            signal_received = 1;
            break;
        case ' ':
            cv::imwrite(fmt::format("snapshot_{:.2d}.jpg", ++save_nr), frame);
            break;
        default:
            break;
        }
        frame_counter.tick();
    }

    cv::destroyAllWindows();
}
