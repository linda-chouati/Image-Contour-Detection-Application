#include "Base.h"


cv::Mat loadImage(const std::string & filePath) {
	cv::Mat image = cv::imread(filePath);
	if(image.empty()) {
		std::cerr<<"Image cannot load"<<filePath<<std::endl;
	}
	return image;
}

void showImage(const cv::Mat &img, const std::string &window) {
	if(img.empty()) {
		std::cerr<<"Image cannot be display"<<std::endl;
		return;
	}
	cv::imshow(window, img);
	cv::waitKey(0);
}

cv::Mat displayMultipleImages(const std::vector<cv::Mat>& images, int rows, int cols) {
    if (images.empty()) {
        std::cerr << "Aucune image à afficher !" << std::endl;
        return cv::Mat();
    }

    int imageWidth = images[0].cols;
    int imageHeight = images[0].rows;

    cv::Mat result(rows * imageHeight, cols * imageWidth, images[0].type(), cv::Scalar(0));

    for (int i = 0; i < images.size(); ++i) {
        int x = (i % cols) * imageWidth; 
        int y = (i / cols) * imageHeight; 

        if (images[i].size() != images[0].size()) {
            cv::resize(images[i], images[i], cv::Size(imageWidth, imageHeight));
        }

        images[i].copyTo(result(cv::Rect(x, y, imageWidth, imageHeight)));
    }

    return result;
}

cv::Mat addTextToImage(const cv::Mat& image, const std::string& text) {
    cv::Mat imgWithText = image.clone(); 
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    int thickness = 2;
    int baseline = 0;

    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
    cv::Point textOrg((image.cols - textSize.width) / 2, textSize.height + 10); 

    cv::rectangle(imgWithText, textOrg + cv::Point(-5, 5),
                  textOrg + cv::Point(textSize.width + 5, -textSize.height - 5),
                  cv::Scalar(0, 0, 0), cv::FILLED);

    cv::putText(imgWithText, text, textOrg, fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);

    return imgWithText;
}
