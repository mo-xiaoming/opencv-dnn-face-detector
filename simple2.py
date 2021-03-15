import cv2
import time

def gstreamer_pipeline(
    capture_width=1280,
    capture_height=720,
    display_width=640,
    display_height=320,
    framerate=60,
    flip_method=0,
):
    return (
        'nvarguscamerasrc sensor_id=0 ! '
        'video/x-raw(memory:NVMM), '
        'width=(int){}, height=(int){}, '
        'format=(string)NV12, framerate=(fraction){}/1 ! '
        'nvvidconv flip-method={} ! '
        'video/x-raw, width=(int){}, height=(int){}, format=(string)BGRx ! '
        'videoconvert ! '
        'video/x-raw, format=(string)BGR ! appsink'
        .format(
            capture_width,
            capture_height,
            framerate,
            flip_method,
            display_width,
            display_height,
        )
    )


class Timer:
    def __init__(self):
        self.reset()

    def record(self):
        self.frame_nr += 1
        if self.frame_nr == 100:
            print(f'\r{self.frame_nr/(time.time()-self.start):.2f} FPS', end='')
            self.reset()

    def reset(self):
        self.start = time.time()
        self.frame_nr = 0


def show_camera():
    print(gstreamer_pipeline())
    cap = cv2.VideoCapture(gstreamer_pipeline(), cv2.CAP_GSTREAMER)
    if not cap.isOpened():
        print('cam not open')
        return

    win_name = 'CSI Camera'
    win = cv2.namedWindow(win_name, cv2.WINDOW_AUTOSIZE)

    timer = Timer()
    while True:
        ret, img = cap.read()
        if not ret:
            print('failed to cap image')
            break
        cv2.imshow(win_name, img)
        if cv2.waitKey(1) & 0xFF == 27:
            break
        timer.record()
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    show_camera()
