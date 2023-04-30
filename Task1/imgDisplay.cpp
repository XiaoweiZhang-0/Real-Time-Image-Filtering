/**
 * Author: Xiaowei Zhang
 * S23
 * function: 
 * 1. read an image and display it in a window
 * 2. enter a loop checking for keypress. If the user types 'q', the program exits
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // check if the user has provided the image path
    if (argc != 2)
    {
        cout<<"Unspecified path to the image"<<endl;
        return -1;
    
    }
    // read the image
    Mat image;
    image = imread(argv[1], IMREAD_COLOR);
    // check if the image is valid
    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }
    while(true){
        // display the image
        imshow("Display Image", image);
        // wait for the user to press 'q'
        if(waitKey(1) == 'q') break;
    }
    
    return 0;
}