/**
 * Author: Xiaowei Zhang
 * S23
 * filter functions
*/
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// Method: Greyscale using value (V) channel in HSV color space
int grayscale_vchannel(cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) {
        return -1; // Error: Input image is empty
    }
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    cv::cvtColor(channels[2], dst, cv::COLOR_GRAY2BGR);
    return 0; // Success
}
// Wrapper function to choose a greyscale method
int grayscale(cv::Mat& src, cv::Mat& dst) {
    return grayscale_vchannel(src, dst); // Use average method by default

}

//blurs an image by using a 5x5 gaussian filter as separable 1x5 filters
int blur5x5( cv::Mat &src, cv::Mat &dst) {
    
    //create the intermediate and destination cv::Mat objects
    cv::Mat intermed;
    intermed.create(src.size(), src.type());
    dst.create(src.size(), src.type());
    
    //make our own initialized color channel
    cv::Vec3i result = {0, 0, 0};
    cv::Vec3i resultCol = {0, 0, 0};
    
    //applying 5x1 filter --> [1 2 4 2 1] horizontal
    for (int i = 2; i < src.rows-2; i++) {
        for (int j = 2; j < src.cols-2; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = src.at<cv::Vec3b>(i-2, j-2)[c] + src.at<cv::Vec3b>(i-2, j-1)[c]*2
                + src.at<cv::Vec3b>(i-2, j)[c]*4 + src.at<cv::Vec3b>(i-2, j+1)[c]*2
                + src.at<cv::Vec3b>(i-2, j+2)[c];
                
                result[c] /= 10;
            }
            intermed.at<cv::Vec3b>(i,j) = result;
        }
    }

    //applying 1x5 filter --> [1 2 4 2 1] vertical
    for (int i = 2; i < intermed.rows-2; i++) {
        for (int j = 2; j < intermed.cols-2; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3b>(i-2, j-2)[c] + intermed.at<cv::Vec3b>(i-1, j-2)[c]*2
                + intermed.at<cv::Vec3b>(i, j-2)[c]*4 + intermed.at<cv::Vec3b>(i+1, j-2)[c]*2
                + intermed.at<cv::Vec3b>(i+2, j-2)[c];
                
                result[c] /= 10;
            }
            dst.at<cv::Vec3b>(i,j) = result;
        }
    }
    return 0;
}

int sobelX3x3( cv::Mat &src, cv::Mat &dst ){

    //create intermediate and dst objects with type CV_16SC3
    cv::Mat intermed;
    intermed.create(src.size(), CV_16SC3);
    dst.create(src.size(), CV_16SC3);
    
    //make our own initialized color channel
    //vec3s works for intermed and dst
    cv::Vec3s result= {0, 0, 0};

    //applying 3x1 filter
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = src.at<cv::Vec3b>(i-1, j-1)[c]*-1 + src.at<cv::Vec3b>(i-1, j)[c]*0 + src.at<cv::Vec3b>(i-1, j+1)[c];
            }
            intermed.at<cv::Vec3s>(i,j) = result;
        }
    }

    //applying 1x3 filter
    for (int i = 1; i < intermed.rows-1; i++) {
        for (int j = 1; j < intermed.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3s>(i-1, j-1)[c] + intermed.at<cv::Vec3s>(i, j-1)[c]*2 + intermed.at<cv::Vec3s>(i+1, j-1)[c] ;
                result[c] /= 4;
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    return 0;
}

int sobelY3x3( cv::Mat &src, cv::Mat &dst ){

    cv::Mat intermed;
    intermed.create(src.size(), CV_16SC3);
    dst.create(src.size(), CV_16SC3);
    
    //make our own initialized color channel
    cv::Vec3s result = {0, 0, 0};
 
    //applying 3x1 filter
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = src.at<cv::Vec3b>(i-1, j-1)[c] + src.at<cv::Vec3b>(i-1, j)[c]*2
                + src.at<cv::Vec3b>(i-1, j+1)[c];
            
                result[c] /= 4;
            }
            intermed.at<cv::Vec3s>(i,j) = result;
        }
    }

    //applying 1x3 filter
    for (int i = 1; i < intermed.rows-1; i++) {
        for (int j = 1; j < intermed.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3s>(i-1, j-1)[c] + intermed.at<cv::Vec3s>(i, j-1)[c]*0
                + intermed.at<cv::Vec3s>(i+1, j-1)[c]*-1 ;
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    return 0;
}
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){

    dst.create(sx.size(), sx.type());
    
    //sx and sy come in as vec3s
    cv::Vec3s result = {0, 0, 0};
    
    //only need to go through one image since sy should be the same
    for (int i = 0; i < sx.rows; i++) {
        for (int j = 0; j < sx.cols; j++){
            //go pixel by pixel for x and y
            for (int c = 0; c < 3; c++) {
                //find the euclidian distance with formula to find the new pixel values
                result[c] = sqrt(pow(sx.at<cv::Vec3s>(i, j)[c], 2) + pow(sy.at<cv::Vec3s>(i, j)[c], 2));
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    
    return 0;
}

int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ){
    
    cv::Mat intermed;
    intermed.create(src.size(), src.type());
    dst.create(src.size(), src.type());
    cv::Vec3i result = {0, 0, 0};
    
    //first blur the image
    blur5x5(src, intermed);
    
    //then quantize the image
    int buckets = 255 / levels;
    for (int i = 0; i < intermed.rows; i++) {
        for (int j = 0; j < intermed.cols; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3b>(i, j)[c] / buckets;
                result[c] = result[c] * buckets;
            }
            dst.at<cv::Vec3b>(i,j) = result;
        }
    }
    return 0;
}

int cartoon( cv::Mat &src, cv::Mat&dst, int levels, int magThreshold ) {

    cv::Mat sobelx;
    cv::Mat sobely;
    cv::Mat mag;
    cv::Mat quantize;

    sobelX3x3(src, sobelx);
    sobelY3x3(src, sobely);
    magnitude(sobelx, sobely, mag);

    blurQuantize(src, quantize, levels);

    dst = quantize.clone();

    for (int i = 0; i < dst.rows; i++) {

        cv::Vec3s *rowptr = mag.ptr<cv::Vec3s>(i);

        cv::Vec3b *dstptr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < dst.cols; j++) {
            if (rowptr[j][0] > magThreshold || rowptr[j][1] > magThreshold || rowptr[j][2] > magThreshold) {
                dstptr[j][0] = 0;
                dstptr[j][1] = 0;
                dstptr[j][2] = 0;
            }
        }
    }
    return 0;
}
int pencilSketch(cv::Mat &src, cv::Mat &dst)
{
    cv::cvtColor(src, dst, cv::COLOR_RGBA2GRAY, 0);
    blur5x5(dst,dst);
    cv::Laplacian(dst, dst, -1, 5);
    dst = 255 - dst;
    cv::threshold(dst, dst, 150, 255, cv::THRESH_BINARY);
    return 0;
}