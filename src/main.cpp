#include <chrono>
#include <iostream>

#include <opencv4/opencv2/opencv.hpp>

#include <face_detector/face_detector.hpp>

int main(int argc, char **argv) {

    cv::VideoCapture video_capture(
		    "nvarguscamerasrc sensor_id=0 ! "
		    "video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)NV12, framerate=(fraction)60/1 ! "
		    "nvvidconv flip-method=0 ! "
		    "video/x-raw, width=(int)640, height=(int)360, format=(string)BGRx ! "
		    "videoconvert ! "
		    "video/x-raw, format=(string)BGR ! "
		    "appsink", cv::CAP_GSTREAMER);
    if (!video_capture.isOpened()) {
        return 0;
    }

    FaceDetector face_detector;

    double frame_nr = 0.;
	auto tstart = std::chrono::steady_clock::now();
    cv::Mat frame;
    while (true) {
	    ++frame_nr;

        video_capture >> frame;

        auto rectangles = face_detector.detect_face_rectangles(frame);
        cv::Scalar color(0, 105, 205);
        for(const auto & r : rectangles){
            cv::rectangle(frame, r, color, 4);
        }


        imshow("Image", frame);
        int esc_key = 27;
        if (cv::waitKey(10) == esc_key) {
            break;
        }
    }
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - tstart).count();
    std::cout << frame_nr << " frames in " << milli << " milliseconds, fps " << frame_nr/milli*1000.0 << std::endl;

    video_capture.release();
    cv::destroyAllWindows();

    return 0;
}
