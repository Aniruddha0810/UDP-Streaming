#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <arpa/inet.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cerr << "Usage: stream_audio_video <ip-address> <port>" << endl;
        return -1;
    }

    string ipAddress = argv[1];
    int port = atoi(argv[2]);

    VideoCapture cap(0); // Open the default camera
    if(!cap.isOpened()) { // Check if we succeeded
        cerr << "Could not open camera" << endl;
        return -1;
    }

    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(CAP_PROP_FPS);

    cout << "Camera resolution: " << frame_width << "x" << frame_height << endl;
    cout << "Camera FPS: " << fps << endl;

    string pipeline = "appsrc ! videoconvert ! video/x-raw,format=YUY2,width=" + to_string(frame_width) + ",height=" + to_string(frame_height) + ",framerate=" + to_string(fps) + "/1 ! jpegenc ! rtpjpegpay ! udpsink host=" + ipAddress + " port=" + to_string(port)
                + " pulsesrc ! audioconvert ! audioresample ! opusenc ! rtpopuspay ! udpsink host=" + ipAddress + " port=" + to_string(port + 1);

    VideoWriter writer(pipeline, 0, fps, Size(frame_width, frame_height), true);

    if (!writer.isOpened()) {
        
        cerr << "Could not open video writer pipeline" << endl;
        return -1;
    }

    namedWindow("Camera", WINDOW_NORMAL); //  if (inet_addr(argv[1]) == INADDR_NONE) {
    //     cerr << "Invalid IP address" << endl;
    //     return -1;
    // }
    resizeWindow("Camera", frame_width, frame_height);

    while (true) {
        Mat frame;
        cap >> frame; // Get a new frame from camera

        // Check if the frame was successfully retrieved
        if (frame.empty()) {
            cerr << "Failed to get frame from camera" << endl;
            break;
        }

        imshow("Camera", frame); // Show the frame in a window

        writer.write(frame); // Write the frame to the video writer pipeline

        // Check for ESC key press
        if (waitKey(1) == 27) {
            break;
        }
    }

    // Release resources
    destroyAllWindows();
    writer.release();

    return 0;
}

