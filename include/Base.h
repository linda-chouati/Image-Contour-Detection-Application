#ifndef BASE_H
#define BASE_H

#include<iostream>
#include <opencv2/opencv.hpp> 

// to load an image 
cv::Mat loadImage(const std::string& filePath); 

// to display an image 
void showImage(const cv::Mat& img, const std::string& window); 

// to display multipes image in a window 
cv::Mat displayMultipleImages(const std::vector<cv::Mat>& images, int rows, int cols);

// to add text on a image
cv::Mat addTextToImage(const cv::Mat& image, const std::string& text); 

#endif