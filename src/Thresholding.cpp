#include "Thresholding.h"


cv::Mat applyThreshold(const cv::Mat &image, int threshold) {
    cv::Mat result = image.clone();

    if (result.channels() == 1) {
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                result.at<uchar>(i, j) = (result.at<uchar>(i, j) >= threshold) ? 255 : 0;
            }
        }
    } else if (result.channels() == 3) {
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                cv::Vec3b &pixel = result.at<cv::Vec3b>(i, j);
                for (int c = 0; c < 3; c++) {
                    pixel[c] = (pixel[c] >= threshold) ? 255 : 0;
                }
            }
        }
    } else {
        std::cerr << "Unsupported number of channels: " << result.channels() << std::endl;
    }

    return result;
}


float computeThreshold(const cv::Mat &image, bool useMedian) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = image;
    }

    int histogram[256] = {0};
    int totalPixels = grayImage.rows * grayImage.cols;

    // calcul de l histo 
    for (int i = 0; i < grayImage.rows; i++) {
        for (int j = 0; j < grayImage.cols; j++) {
            int pixelValue = grayImage.at<uchar>(i, j);
            histogram[pixelValue]++;
        }
    }

    if (useMedian) { // calcul du seuil basé sur la median 
        int cumulativeSum = 0;
        for (int i = 0; i < 256; i++) {
            cumulativeSum += histogram[i];
            if (cumulativeSum >= totalPixels / 2) {
                return i;
            }
        }
    } else { // calcul du seuil basé sur la moyenne pondére
        float sum = 0.0f;
        float weightedSum = 0.0f;

        for (int i = 0; i < 256; i++) {
            weightedSum += i * histogram[i];
            sum += histogram[i];
        }

        return weightedSum / sum; 
    }

    return 0; 
}


cv::Mat applyHysteresisThresholding(const cv::Mat &image, int lowThreshold, int highThreshold) {
    cv::Mat result = image.clone();

    if (result.channels() == 1) { // uniquement si img grise 
        // pixels au-dessus du seuil haut
        for (int i = 0; i < image.rows; i++) { 
            for (int j = 0; j < image.cols; j++) {
                if (image.at<uchar>(i, j) >= highThreshold) {
                    result.at<uchar>(i, j) = 255;
                }
            }
        }

        // pixels entre le seuil bas et haut
        for (int i = 1; i < image.rows - 1; i++) {
            for (int j = 1; j < image.cols - 1; j++) {
                if (image.at<uchar>(i, j) >= lowThreshold && image.at<uchar>(i, j) < highThreshold) {
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            if (result.at<uchar>(i + di, j + dj) == 255) {
                                result.at<uchar>(i, j) = 255;
                                break;
                            }
                        }
                    }
                }
            }
        }
    } else if (result.channels() == 3) {
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
                if (pixel[0] >= highThreshold && pixel[1] >= highThreshold && pixel[2] >= highThreshold) {
                    result.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
                }
            }
        }

        for (int i = 1; i < image.rows - 1; i++) {
            for (int j = 1; j < image.cols - 1; j++) {
                cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
                if (pixel[0] >= lowThreshold && pixel[0] < highThreshold &&
                    pixel[1] >= lowThreshold && pixel[1] < highThreshold &&
                    pixel[2] >= lowThreshold && pixel[2] < highThreshold) {
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            cv::Vec3b neighbor = result.at<cv::Vec3b>(i + di, j + dj);
                            if (neighbor[0] == 255 && neighbor[1] == 255 && neighbor[2] == 255) {
                                result.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
                                break;
                            }
                        }
                    }
                }
            }
        }
    } else {
        std::cerr << "Unsupported number of channels: " << result.channels() << std::endl;
    }

    return result;
}

std::pair<double, double> computeMeanStdDev(const cv::Mat& img) {
    double sum = 0.0, sumSq = 0.0;
    double mean = 0.0, stddev = 0.0;
    int totalPixels = img.rows * img.cols;

    cv::Mat grayImage;
    if (img.channels() == 3) {
        cv::cvtColor(img, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = img;
    }
    //  la somme et la somme des carrés
    for (int i = 0; i < grayImage.rows; i++) {
        for (int j = 0; j < grayImage.cols; j++) {
            uchar pixel = grayImage.at<uchar>(i, j);
            sum += pixel;
            sumSq += pixel * pixel;
        }
    }
    mean = sum / totalPixels;
    stddev = sqrt((sumSq / totalPixels) - (mean * mean));
    return {mean, stddev};
}

std::pair<int, int> estimateHysteresisThresholds(const cv::Mat &image) {
    auto [mean, stddev] = computeMeanStdDev(image);
    return {static_cast<int>(std::max(0.0, mean + 0.5 * stddev)), static_cast<int>(std::min(255.0, mean + 1.5 * stddev))};
}


void thinningIteration(cv::Mat &img, int iter) {
    cv::Mat marker = cv::Mat::ones(img.size(), CV_8UC1); 

    for (int i = 1; i < img.rows - 1; i++) {
        for (int j = 1; j < img.cols - 1; j++) {
            if (img.at<uchar>(i, j) != 1) continue; 

            uchar p2 = img.at<uchar>(i-1, j);
            uchar p3 = img.at<uchar>(i-1, j+1);
            uchar p4 = img.at<uchar>(i, j+1);
            uchar p5 = img.at<uchar>(i+1, j+1);
            uchar p6 = img.at<uchar>(i+1, j);
            uchar p7 = img.at<uchar>(i+1, j-1);
            uchar p8 = img.at<uchar>(i, j-1);
            uchar p9 = img.at<uchar>(i-1, j-1);

            int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

            if (A == 1 && (B >= 2 && B <= 6)) {
                bool cond1 = (iter == 0) && (p2 * p4 * p6 == 0) && (p4 * p6 * p8 == 0);
                bool cond2 = (iter == 1) && (p2 * p4 * p8 == 0) && (p2 * p6 * p8 == 0);
                if (cond1 || cond2) {
                    marker.at<uchar>(i, j) = 0; //  pixel à supp
                }
            }
        }
    }

    img &= marker; 
}

void thinning(cv::Mat &img) {
    img /= 255; // pour la ormalisation  => (0/1)
    cv::Mat prev = cv::Mat::zeros(img.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningIteration(img, 0);
        thinningIteration(img, 1);
        cv::absdiff(img, prev, diff);
        img.copyTo(prev);
    } while (cv::countNonZero(diff) > 0);

    img *= 255; // retour à l'échelle 0-255
}

cv::Mat zhangSuenPrep(const cv::Mat &image)
{
    cv::Mat img = image.clone();

    if (image.channels() == 3)
    {
        cv::Mat gray, binary;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY);

        thinning(binary);

        cv::Mat skeletonColor = cv::Mat::zeros(image.size(), image.type());

        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                if (binary.at<uchar>(i, j) == 255) {  // si fait partie du squelette
                    skeletonColor.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i, j);  // on recup la couleur originale
                }
            }
        }
        return skeletonColor;
    }
    else 
    {
        cv::Mat binary;
        cv::threshold(image, binary, 127, 255, cv::THRESH_BINARY);
        thinning(binary);

        return binary;
    }
}
