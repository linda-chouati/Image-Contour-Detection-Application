#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <opencv2/opencv.hpp>


cv::Mat applyThreshold(const cv::Mat &image, int threshold); 
float computeThreshold(const cv::Mat &image, bool useMedian); 

cv::Mat applyHysteresisThresholding(const cv::Mat &image, int lowThreshold, int highThreshold); 
std::pair<int, int> estimateHysteresisThresholds(const cv::Mat &image); 
cv::Mat zhangSuenPrep(const cv::Mat &image);
void thinning(cv::Mat &img);
void thinningIteration(cv::Mat &img, int iter);

#endif 

