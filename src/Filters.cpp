#include "Filters.h"

cv::Mat normalizeKernel(const cv::Mat& kernel) {
    double sum = cv::sum(cv::abs(kernel))[0]; 
    return (sum != 0) ? kernel / sum : kernel; 
}

const cv::Mat PREWITT_FILTER_X = (cv::Mat_<float>(3,3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
const cv::Mat PREWITT_FILTER_Y = (cv::Mat_<float>(3,3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
const cv::Mat PREWITT_FILTER_D1 = (cv::Mat_<float>(3,3) << 0, -1, -2, 1,  0, -1, 2,  1,  0);
const cv::Mat PREWITT_FILTER_D2 = (cv::Mat_<float>(3,3) << -2, -1,  0, -1,  0,  1, 0,  1,  2);

const cv::Mat SOBEL_FILTER_X = (cv::Mat_<float>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
const cv::Mat SOBEL_FILTER_Y = (cv::Mat_<float>(3,3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
const cv::Mat SOBEL_FILTER_D1 = (cv::Mat_<float>(3,3) << 0, -1, -2, 1,  0, -1, 2,  1,  0);
const cv::Mat SOBEL_FILTER_D2 = (cv::Mat_<float>(3,3) << -2, -1,  0, -1,  0,  1, 0,  1,  2);

const cv::Mat KIRSCH_FILTER_X = (cv::Mat_<float>(3,3) << -3, -3, 5, -3, 0, 5, -3, -3, 5);
const cv::Mat KIRSCH_FILTER_Y = (cv::Mat_<float>(3,3) << -3, 5, 5, -3, 0, 5, -3, -3, -3);
const cv::Mat KIRSCH_FILTER_D1 = (cv::Mat_<float>(3,3) << -3, -3, 5, -3, 0, 5, -3, -3, 5);
const cv::Mat KIRSCH_FILTER_D2 = (cv::Mat_<float>(3,3) << 5, 5, 5, -3, 0, -3, -3, -3, -3);

const int GRAY_LEVELS = 256;

cv::Mat getKernel(FilterType filter, Direction direction) {
    switch (filter) {
        case FilterType::Prewitt:
            switch (direction) {
                case Direction::X: return PREWITT_FILTER_X;
                case Direction::Y: return PREWITT_FILTER_Y;
                case Direction::D1: return PREWITT_FILTER_D1;
                case Direction::D2: return PREWITT_FILTER_D2;
            }
            break;

        case FilterType::Sobel:
            switch (direction) {
                case Direction::X: return SOBEL_FILTER_X;
                case Direction::Y: return SOBEL_FILTER_Y;
                case Direction::D1: return SOBEL_FILTER_D1;
                case Direction::D2: return SOBEL_FILTER_D2;
            }
            break;

        case FilterType::Kirsch:
            switch (direction) {
                case Direction::X: return KIRSCH_FILTER_X;
                case Direction::Y: return KIRSCH_FILTER_Y;
                case Direction::D1: return KIRSCH_FILTER_D1;
                case Direction::D2: return KIRSCH_FILTER_D2;
            }
            break;
    }

    std::cerr << "Filtre inconnu, Sobel X utilisé par défaut." << std::endl;
    return SOBEL_FILTER_X;
}



float genericConvolutionGray(const cv::Mat &image, const cv::Mat &kernel, int i, int j) {
    if (image.channels() != 1) {
        std::cerr << "Only grayscale images are supported in genericConvolutionGray." << std::endl;
        return 0;
    }

    if (kernel.rows % 2 == 0 || kernel.cols % 2 == 0) {
        std::cerr << "The size of the kernel must be odd." << std::endl;
        return 0;
    }

    const int dx = kernel.cols / 2;
    const int dy = kernel.rows / 2;

    float sum = 0.0f;
    bool ignorePixel = false;
    int borderHandling = 0;

    for (int k = 0; k < kernel.rows; k++) {
        for (int l = 0; l < kernel.cols; l++) {
            int x = j - dx + l;
            int y = i - dy + k;

            switch (borderHandling) {
                case 0: // Dupliquer le pixel voisin le plus proche
                    x = std::max(0, std::min(x, image.cols - 1));
                    y = std::max(0, std::min(y, image.rows - 1));
                    break;
                case 1: // Noir (padding à 0)
                    if (x < 0 || x >= image.cols || y < 0 || y >= image.rows)
                        continue;
                    break;
                case 2: // Blanc (padding à 255)
                    if (x < 0 || x >= image.cols || y < 0 || y >= image.rows) {
                        sum += 255 * kernel.at<float>(k, l);
                        continue;
                    }
                    break;
                case 3: // Ignore les bords
                    if (x < 0 || x >= image.cols || y < 0 || y >= image.rows) {
                        ignorePixel = true;
                    }
                    break;
            }

            if (!ignorePixel) {
                sum += static_cast<float>(image.at<uchar>(y, x)) * kernel.at<float>(k, l); // on fait le calcul de la convolution
            }
        }
    }

    return std::max(0.0f, std::min(sum, 255.0f)); // pour debug => etre sur que le pixel pixel est entre 0 et 255
}

cv::Vec3f genericConvolutionColor(const cv::Mat& image, const cv::Mat& kernel, int i, int j) {
    if (image.channels() != 3) {
        std::cerr << "Only color images are supported in genericConvolutionColor" << std::endl;
        return cv::Vec3f(0, 0, 0);
    }

    std::vector<cv::Mat> channels;
    cv::split(image, channels); 

    cv::Vec3f result;
    for (int c = 0; c < 3; ++c) {
        result[c] = genericConvolutionGray(channels[c], kernel, i, j);
    }

    return result;
}

cv::Vec3f applyConvolution(const cv::Mat & img, const cv::Mat& kernel, int i, int j) {
    return img.channels() == 1 ? 
        cv::Vec3f(genericConvolutionGray(img, kernel, i, j), 0, 0) :
        genericConvolutionColor(img, kernel, i, j);
}

float applyConvolutionGrayNew(const cv::Mat &image, const cv::Mat &kernel, int i, int j) {

    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = image;
    }
    
    if (kernel.rows % 2 == 0 || kernel.cols % 2 == 0) {
        std::cerr << "The size of the kernel must be odd." << std::endl;
        return 0;
    }
    
    const int dx = kernel.cols / 2;
    const int dy = kernel.rows / 2;
    
    float sum = 0.0f;
    bool ignorePixel = false;
    int borderHandling = 0;
    
    for (int k = 0; k < kernel.rows; k++) {
        for (int l = 0; l < kernel.cols; l++) {
            int x = j - dx + l;
            int y = i - dy + k;
            
            switch (borderHandling) {
                case 0: 
                    x = std::max(0, std::min(x, grayImage.cols - 1));
                    y = std::max(0, std::min(y, grayImage.rows - 1));
                    break;
                case 1: 
                    if (x < 0 || x >= grayImage.cols || y < 0 || y >= grayImage.rows)
                        continue;
                    break;
                case 2: 
                    if (x < 0 || x >= grayImage.cols || y < 0 || y >= grayImage.rows) {
                        sum += 255 * kernel.at<float>(k, l);
                        continue;
                    }
                    break;
                case 3: 
                    if (x < 0 || x >= grayImage.cols || y < 0 || y >= grayImage.rows) {
                        ignorePixel = true;
                    }
                    break;
            }
            
            if (!ignorePixel) {
                sum += static_cast<float>(grayImage.at<uchar>(y, x)) * kernel.at<float>(k, l);
            }
        }
    }
    
    return std::max(0.0f, std::min(sum, 255.0f)); 
}


// edges détection 

cv::Mat detectEdges(const cv::Mat& image, FilterType filter, EdgeDetectionStrategy strategy, bool useColor) {
    cv::Mat kernel_x = getKernel(filter, Direction::X);
    cv::Mat kernel_y = getKernel(filter, Direction::Y);

    cv::Mat gradient = cv::Mat::zeros(image.size(), CV_32F);
    cv::Mat angle = cv::Mat::zeros(image.size(), CV_32F);
    cv::Mat gradientHSV;

    if (useColor) {
        gradientHSV = cv::Mat(image.size(), CV_32FC3);  // image en HSV
    }

    if (strategy == EdgeDetectionStrategy::Multidirectional) {
        cv::Mat kernel_d1 = getKernel(filter, Direction::D1);
        cv::Mat kernel_d2 = getKernel(filter, Direction::D2);

        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                float gx = applyConvolutionGrayNew(image, kernel_x, i, j) + applyConvolutionGrayNew(image, kernel_d1, i, j);
                float gy = applyConvolutionGrayNew(image, kernel_y, i, j) + applyConvolutionGrayNew(image, kernel_d2, i, j);

                float magnitude = sqrt(gx * gx + gy * gy);
                magnitude = std::max(0.0f, std::min(magnitude, 255.0f));

                float theta = atan2(gy, gx);  
                float hue = (theta + CV_PI) * 180.0f / CV_PI;  

                gradient.at<float>(i, j) = magnitude;
                angle.at<float>(i, j) = theta;

                if (useColor) {
                    gradientHSV.at<cv::Vec3f>(i, j) = cv::Vec3f(hue, 255.0f, magnitude); 
                }
            }
        }
    } else { 
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                float gx = applyConvolutionGrayNew(image, kernel_x, i, j);  
                float gy = applyConvolutionGrayNew(image, kernel_y, i, j);

                float magnitude = sqrt(gx * gx + gy * gy);
                magnitude = std::max(0.0f, std::min(magnitude, 255.0f));

                float theta = atan2(gy, gx);
                float hue = (theta + CV_PI) * 180.0f / CV_PI;

                gradient.at<float>(i, j) = magnitude;
                angle.at<float>(i, j) = theta;

                if (useColor) {
                    gradientHSV.at<cv::Vec3f>(i, j) = cv::Vec3f(hue, 255.0f, magnitude);
                }
            }
        }
    }

    if (useColor) {
        cv::Mat gradientBGR;
        gradientHSV.convertTo(gradientHSV, CV_8UC3);
        cv::cvtColor(gradientHSV, gradientBGR, cv::COLOR_HSV2BGR);
        return gradientBGR;
    }

    gradient.convertTo(gradient, CV_8U, 255.0 / cv::norm(gradient, cv::NORM_INF));
    return gradient;
}

