#include <iostream>
#include "Base.h"
#include "Filters.h"
#include "Thresholding.h"

const std::string filePath = "../data/ExempleSimple.jpg"; 
const std::string filePath2 = "../data/lena.jpg"; 
const std::string filePath3 = "../data/cathedraleLyon.jpg"; 
const std::string filePath4 = "../data/output.png"; 

// int main() {
//     std::cout << "test" << std::endl;
//     cv::Mat img = loadImage(filePath2);
//     std::cout << "test####" << std::endl;

//     cv::Mat sobelBidirectional = detectEdges(img, FilterType::Sobel, EdgeDetectionStrategy::Bidirectional, false);
//     std::cout << "test1111" << std::endl;

//     // float seuil = computeThreshold(sobelBidirectional, false); // moyenne 
//     // cv::Mat sobelBidirectionalTrshold = applyThreshold(sobelBidirectional, seuil); 
//     // std::cout << "teestWWWWWWWWWW" << std::endl;

//     // float seuilMed = computeThreshold(sobelBidirectional, true); //median 
//     // cv::Mat sobelBidirectionalTrsholdMed = applyThreshold(sobelBidirectional, seuilMed); 

//     auto [low, high] = estimateHysteresisThresholds(sobelBidirectional);
//     cv::Mat hysteresisResult = applyHysteresisThresholding(sobelBidirectional, low, high);

//     // cv::Mat thinnedimage = zhangSuenPrep(sobelBidirectional);


//     // cv::Mat prewittBidirectional = detectEdgesColor(img, FilterType::Prewitt, EdgeDetectionStrategy::Bidirectional, false);
//     // std::cout << "test2222" << std::endl;
//     // cv::Mat kirschBidirectional = detectEdgesColor(img, FilterType::Kirsch, EdgeDetectionStrategy::Bidirectional, false);
//     // std::cout << "test33333" << std::endl;

//     // cv::Mat sobelMulti = detectEdges(img, FilterType::Sobel, EdgeDetectionStrategy::Multidirectional, false);
//     // std::cout << "test44444" << std::endl;
//     // cv::Mat prewittMulti = detectEdges(img, FilterType::Prewitt, EdgeDetectionStrategy::Multidirectional, false);
//     // std::cout << "test55555" << std::endl;
//     // cv::Mat kirschMulti = detectEdges(img, FilterType::Kirsch, EdgeDetectionStrategy::Multidirectional, false);
//     // std::cout << "test55555" << std::endl;

//     showImage(sobelBidirectional, "sobel biddirectionnel"); 
//     // showImage(sobelBidirectionalTrshold, "sobel biddirectionnel threshold"); 
//     // showImage(sobelBidirectionalTrsholdMed, "sobel biddirectionnel threshold median"); 
//     showImage(hysteresisResult, "sobel biddirectionnel hysteris"); 
//     // showImage(thinnedimage, "thinned image"); 
//     // showImage(img, "original"); 
//     // showImage(sobelMulti, "sobel multidi rectionnel");
//     // showImage(prewittBidirectional, "prewit birirectinnel "); 
//     // showImage(kirschBidirectional, "kirschen bidirectionnel "); 
//     // showImage(sobelMulti, "sobel multni multtirectionell"); 
//     // showImage(prewittMulti, "prewit multirectinnel "); 
//     // showImage(kirschMulti, "kirschen multirectionnel "); 

//     return 0;
// }

#include <wx/wx.h>
#include "ui/ImageFrame.h"

class ImageApp : public wxApp {
public:
    virtual bool OnInit() {
        ImageFrame* frame = new ImageFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(ImageApp);


