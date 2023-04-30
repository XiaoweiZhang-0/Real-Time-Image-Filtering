/**
 * Author: Xiaowei Zhang
 * S23
 * function: 
 * 1. open a video channel, create an window, and then loop, capturing a new frame and displaying it each time through the loop
 * 2. enter a loop checking for keypress. If the user types 'q', the program exits
 * 3. save an image of the current frame to a file if the user types 's'
 */
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "filter.h"
using namespace std;
int main(int argc, char *argv[]) {
        cv::VideoCapture *capdev;

        // open the video device
        capdev = new cv::VideoCapture(0);
        if( !capdev->isOpened() ) {
                printf("Unable to open video device\n");
                return(-1);
        }

        // get some properties of the image
        cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                       (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("Expected size: %d %d\n", refS.width, refS.height);

        cv::namedWindow("Video", 1); // identifies a window
        cv::Mat frame;
        int imgCount = 0;
        // create an folder to save the images
        std::string folderName = "savedImages";
        std::string command = "mkdir " + folderName;
        system(command.c_str());
        int k = 0;
        for(;;) {
                *capdev >> frame; // get a new frame from the camera, treat as a stream
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }                

                // see if there is a waiting keystroke
                char key = cv::waitKey(10);
                if(k == 1){
                    cv::Mat gray;
                    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
                    cv::imshow("Video", gray);
                    frame = gray;
                }
                else if(k == 2){
                    cv::Mat altGray;
                    grayscale(frame, altGray);
                    cv::imshow("Video", altGray);
                    frame = altGray;
                }
                else if(k == 3){
                    cv::Mat blur;
                    blur5x5(frame, blur);
                    cv::imshow("Video", blur);
                    frame = blur;
                }
                else if (k == 4) {
                    cv::Mat dst;
                    sobelX3x3(frame, dst);
                    cv::convertScaleAbs( dst, frame );
                    cv::imshow("Video", frame);
                }
                //sets the current frame to a 3x3 sobel y filter as defined in filter.c
                else if (k == 5) {
                    cv::Mat dst;
                    sobelY3x3(frame, dst);
                    cv::convertScaleAbs( dst, frame );
                    cv::imshow("Video", frame);
                }
                else if (k == 6){
                    cv::Mat dst;
                    cv::Mat dst2;
                    cv::Mat magFrame;
                    sobelX3x3(frame, dst);
                    sobelY3x3(frame, dst2);
                    magnitude(dst, dst2, magFrame);
                    cv::convertScaleAbs(magFrame, frame);
                    cv::imshow("Video", frame);
                }
                else if(k == 7){
                    cv::Mat dst;
                    blurQuantize(frame, dst, 15);
                    cv::imshow("Video", dst);
                    frame = dst;
                }
                else if(k==8){
                    cv::Mat dst;
                    cartoon(frame, dst, 15, 15);
                    cv::imshow("Video", dst);
                    frame = dst;
                }
                else if(k==9){
                    cv::Mat dst;
                    pencilSketch(frame, dst);
                    cv::imshow("Video", dst);
                    frame = dst;
                }
                else{
                    cv::imshow("Video", frame);
                }
                //if the user types 'g', the program will display the image in color(if it is gray) or gray(if it is color
                if( key == 'g') {
                    k = 1;
                }
                //if the user types 'h', the program will display an alternative grey scale image
                else if( key == 'h') {
                    k = 2;
                }
                else if( key == 'q') {
                    break;
                }
                else if( key == 's') {
                    std::string count = std::to_string(imgCount);
                    cv::imwrite("savedImages/IMG"+count+".jpg", frame);
                    imgCount++;
                }
                else if(key == 'b'){
                   k = 3;
                }
                else if(key == 'r'){
                    k = 0;
                }
                else if(key == 'x'){
                    k = 4;
                }
                else if(key == 'y'){
                    k = 5;
                }
                else if(key == 'm'){
                    k = 6;
                }
                else if(key == 'l'){
                    k = 7;
                }
                else if(key == 'c'){
                    k = 8;
                }
                else if(key == 'p'){
                    k = 9;
                }

        }

        delete capdev;
        return(0);
}