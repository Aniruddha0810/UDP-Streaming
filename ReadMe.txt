To install OpenCV, run the following commands
1.sudo apt update
2.apt install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
3.git clone https://github.com/opencv/opencv.git
4.git clone https://github.com/opencv/opencv-contrib
5.cd opencv
6.mkdir build
7.cd build
8.cmake ..
9.make -j4
10.make install

Compile the sender code in terminal with the folowing command
"g++ Sender.cpp -o sender `pkg-config --cflags --libs opencv4 gstreamer-1.0 gstreamer-app-1.0`
Run the Sender application
./sender "ip-address" "port-number"

Compile the reciever code in terminal with the folowing command
"g++ Reciever.cpp -o reciever `pkg-config --cflags --libs opencv4 gstreamer-1.0 gstreamer-app-1.0`
Run the reciever application
./reciever "port-number"
