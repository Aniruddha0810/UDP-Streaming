#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <arpa/inet.h>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::string ipAddress = "192.168.0.1"; // Replace with the desired IP address

    std::string command = "ping -c 1 " + ipAddress; // Linux/macOS
    // std::string command = "ping -n 1 " + ipAddress; // Windows

    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "The IP address is available." << std::endl;
            if (argc != 3) {
            cerr << "Usage: stream_audio_video <ip-address> <port>" << endl;
            return -1;
        }

        

        string ipAddress = argv[1];
        int port = atoi(argv[2]);

        struct sockaddr_in sa;
        if (inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr)) == 0) {
            cerr << "Invalid IP address" << endl;
            return -1;
        }

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

        namedWindow("Camera", WINDOW_NORMAL);
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
    } else {
        std::cout << "The IP address is not available." << std::endl;
    }

    return 0;
}