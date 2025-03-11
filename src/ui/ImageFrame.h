#ifndef IMAGE_FRAME_H
#define IMAGE_FRAME_H

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <opencv2/opencv.hpp>
#include "Base.h"
#include "Filters.h"
#include "Thresholding.h"

class ImageFrame : public wxFrame {
    public:
        ImageFrame();

    private:
        wxStaticBitmap* originalBitmap;
        wxStaticBitmap* processedBitmap;
        wxStaticBitmap* thresholdedBitmap;
        wxStaticBitmap* refinedBitmap;
        wxStaticText* originalLabel;
        wxStaticText* processedLabel;
        wxStaticText* thresholdedLabel;
        wxStaticText* refinedLabel;

        wxChoice* filterChoice;
        wxChoice* strategyChoice;
        wxCheckBox* useColorCheckBox;

        wxChoice* thresholdChoice;
        wxSlider* thresholdSlider;
        wxBoxSizer* mainSizer;
        wxStaticBoxSizer* thresholdSizer;

        wxButton* applyButton;
        wxButton* applyThresholdButton;
        wxButton* refineButton;

        cv::Mat originalImage;
        cv::Mat processedImage;
        cv::Mat thresholdedImage;
        cv::Mat refinedImage;

        void OnLoadImage(wxCommandEvent&);
        void OnApplyFunction(wxCommandEvent&);
        void OnApplyThreshold(wxCommandEvent&);
        void OnThresholdChoiceChanged(wxCommandEvent&);
        void OnApplyRefining(wxCommandEvent&);
        void DisplayImage(const cv::Mat& image, wxStaticBitmap* bitmap);
};

#endif
