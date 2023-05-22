#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: receive_video <port>" << endl;
        return -1;
    }

    int port = atoi(argv[1]);

    string pipeline = "udpsrc port=" + to_string(port) + " ! application/x-rtp,encoding-name=JPEG,payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink"
                      + " udpsrc port=" + to_string(port + 1) + " ! application/x-rtp, payload=96, media=audio, clock-rate=48000, encoding-name=OPUS ! rtpopusdepay ! opusdec ! audioconvert ! audioresample ! autoaudiosink";

    VideoCapture video_stream(pipeline, cv::CAP_GSTREAMER);
    if (!video_stream.isOpened()) {
        cerr << "Failed to open video stream" << endl;
        return -1;
    }

    Mat frame;
    namedWindow("Received Video", WINDOW_NORMAL);

    while (true) {
        video_stream >> frame;
        
        // Check if the frame was successfully retrieved
        if (frame.empty()) {
            cerr << "Failed to get frame from video stream" << endl;
            break;
        }

        imshow("Received Video", frame); // Show the received frame in a window

        // Check for ESC key press
        if (waitKey(1) == 27) {
            break;
        }
    }

    // Release resources
    destroyAllWindows();
    video_stream.release();

    return 0;
}
