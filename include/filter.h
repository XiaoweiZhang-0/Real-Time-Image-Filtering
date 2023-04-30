#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
// Method: Grayscale using average of color channels
int grayscale_vchannel(cv::Mat& src, cv::Mat& dst);

// Wrapper function to choose a greyscale method
int grayscale(cv::Mat& src, cv::Mat& dst);

int blur5x5(cv::Mat& src, cv::Mat& dst);

int sobelX3x3( cv::Mat &src, cv::Mat &dst );

int sobelY3x3( cv::Mat &src, cv::Mat &dst );

int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst );

int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels );

int cartoon( cv::Mat &src, cv::Mat&dst, int levels, int magThreshold );

int pencilSketch(cv::Mat &src, cv::Mat &dst);
#endif // FILTER_H