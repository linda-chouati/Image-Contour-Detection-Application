#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/opencv.hpp>

enum class FilterType {
    Sobel,
    Prewitt,
    Kirsch
};

enum class Direction {
    X,  // pour l horizontale
    Y,  // pour la vertical 
    D1, // pour la diagonale de 45°
    D2  // pour la diagonale de 135°
};

enum class EdgeDetectionStrategy {
    Bidirectional,  
    Multidirectional 
};

// on les mets en extern psk pas envie de faire de classe convolution 
extern const cv::Mat PREWITT_FILTER_X;
extern const cv::Mat PREWITT_FILTER_Y;
extern const cv::Mat PREWITT_FILTER_D1;
extern const cv::Mat PREWITT_FILTER_D2;

extern const cv::Mat SOBEL_FILTER_X;
extern const cv::Mat SOBEL_FILTER_Y;
extern const cv::Mat SOBEL_FILTER_D1;
extern const cv::Mat SOBEL_FILTER_D2;

extern const cv::Mat KIRSCH_FILTER_X;
extern const cv::Mat KIRSCH_FILTER_Y;
extern const cv::Mat KIRSCH_FILTER_D1;
extern const cv::Mat KIRSCH_FILTER_D2;

extern const int GRAY_LEVELS;


cv::Mat normalizeKernel(const cv::Mat& kernel); 


// pour obtenir un noyau de convolution selon le type du filtre
cv::Mat getKernel(FilterType filter, Direction direction);

// pour effectuer la convolution
float genericConvolutionGray(const cv::Mat &image, const cv::Mat &kernel, int i, int j); 
cv::Vec3f genericConvolutionColor(const cv::Mat& image, const cv::Mat& kernel, int i, int j); 
cv::Vec3f applyConvolution(const cv::Mat & img, const cv::Mat& kernel, int i, int j); 
float applyConvolutionGrayNew(const cv::Mat &image, const cv::Mat &kernel, int i, int j); 

// pour detecter les contours selon une stratégy de direction pour les gradient 
cv::Mat detectEdges(const cv::Mat& image, FilterType filter, EdgeDetectionStrategy strategy, bool useColor); 

#endif 




