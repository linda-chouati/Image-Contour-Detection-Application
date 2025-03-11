#include "ImageFrame.h"

ImageFrame::ImageFrame() 
    : wxFrame(nullptr, wxID_ANY, "Détection de contour", wxDefaultPosition, wxSize(1100, 750)) { 
        
    Maximize(true);

    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_OPEN, "Ouvrir une image\t⌘O");
    menuBar->Append(fileMenu, "File");
    SetMenuBar(menuBar);
    
    // panneau principal
    wxPanel* panel = new wxPanel(this);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // pour les images + textes
    wxBoxSizer* imageContainerSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* imageSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* textSizer = new wxBoxSizer(wxHORIZONTAL); 

    originalBitmap = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(300, 300));
    processedBitmap = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(300, 300));
    thresholdedBitmap = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(300, 300));
    refinedBitmap = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(300, 300));

    imageSizer->Add(originalBitmap, 1, wxEXPAND | wxALL, 10);
    imageSizer->Add(processedBitmap, 1, wxEXPAND | wxALL, 10);
    imageSizer->Add(thresholdedBitmap, 1, wxEXPAND | wxALL, 10);
    imageSizer->Add(refinedBitmap, 1, wxEXPAND | wxALL, 10);
    
    originalLabel = new wxStaticText(panel, wxID_ANY, "Image originale", wxDefaultPosition, wxSize(300, 20), wxALIGN_CENTER);
    processedLabel = new wxStaticText(panel, wxID_ANY, "Filtrage appliqué", wxDefaultPosition, wxSize(300, 20), wxALIGN_CENTER);
    thresholdedLabel = new wxStaticText(panel, wxID_ANY, "Seuillage appliqué", wxDefaultPosition, wxSize(300, 20), wxALIGN_CENTER);
    refinedLabel = new wxStaticText(panel, wxID_ANY, "Affinage appliqué", wxDefaultPosition, wxSize(300, 20), wxALIGN_CENTER);

    textSizer->Add(originalLabel, 1, wxEXPAND | wxALL, 10);
    textSizer->Add(processedLabel, 1, wxEXPAND | wxALL, 10);
    textSizer->Add(thresholdedLabel, 1, wxEXPAND | wxALL, 10);
    textSizer->Add(refinedLabel, 1, wxEXPAND | wxALL, 10);

    imageContainerSizer->Add(imageSizer, 1, wxEXPAND);
    imageContainerSizer->Add(textSizer, 0, wxEXPAND);

    mainSizer->Add(imageContainerSizer, 1, wxEXPAND);

    // pour la partir affiche du traitement
    wxStaticBoxSizer* optionsSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Options de traitement");
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 5, 5);

    gridSizer->Add(new wxStaticText(panel, wxID_ANY, "Choisir un filtre :"), 0, wxALIGN_CENTER_VERTICAL);
    filterChoice = new wxChoice(panel, wxID_ANY);
    filterChoice->Append("Prewitt");
    filterChoice->Append("Sobel");
    filterChoice->Append("Kirsch");
    filterChoice->SetSelection(0);
    gridSizer->Add(filterChoice, 1, wxEXPAND);

    gridSizer->Add(new wxStaticText(panel, wxID_ANY, "Choisir une stratégie :"), 0, wxALIGN_CENTER_VERTICAL);
    strategyChoice = new wxChoice(panel, wxID_ANY);
    strategyChoice->Append("Bidirectionnelle");
    strategyChoice->Append("Multidirectionnelle");
    strategyChoice->SetSelection(0);
    gridSizer->Add(strategyChoice, 1, wxEXPAND);

    optionsSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 10);

    useColorCheckBox = new wxCheckBox(panel, wxID_ANY, "Afficher en couleur");
    optionsSizer->Add(useColorCheckBox, 0, wxALIGN_CENTER | wxALL, 5);

    applyButton = new wxButton(panel, wxID_ANY, "Appliquer");
    optionsSizer->Add(applyButton, 0, wxALIGN_CENTER | wxALL, 10);

    mainSizer->Add(optionsSizer, 0, wxEXPAND | wxALL, 10);

    // affichage pour les seuillages
    thresholdSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Options de seuillage");

    thresholdChoice = new wxChoice(panel, wxID_ANY);
    thresholdChoice->Append("Seuillage moyenne");  
    thresholdChoice->Append("Seuillage median");
    thresholdChoice->Append("Seuillage manuel");  
    thresholdChoice->Append("Seuillage par hysteresis");
    thresholdChoice->SetSelection(0);  
    thresholdSizer->Add(thresholdChoice, 0, wxEXPAND | wxALL, 5);
    
    thresholdSlider = new wxSlider(panel, wxID_ANY, 128, 0, 255, wxDefaultPosition, wxSize(250, -1));
    thresholdSizer->Add(new wxStaticText(panel, wxID_ANY, ""), 0, wxALIGN_CENTER | wxTOP, 5);
    thresholdSizer->Add(thresholdSlider, 0, wxEXPAND | wxALL, 5);
    thresholdSlider->Hide();  
    

    applyThresholdButton = new wxButton(panel, wxID_ANY, "Appliquer le seuillage");
    applyThresholdButton->Hide(); 
    thresholdSizer->Add(applyThresholdButton, 0, wxALIGN_CENTER | wxALL, 10);

    refineButton = new wxButton(panel, wxID_ANY, "Activer l'affinage");
    refineButton->Hide(); 
    thresholdSizer->Add(refineButton, 0, wxALIGN_CENTER | wxALL, 10);

    thresholdSizer->Show(false);

    panel->SetSizer(mainSizer);

    Bind(wxEVT_MENU, &ImageFrame::OnLoadImage, this, wxID_OPEN);
    applyButton->Bind(wxEVT_BUTTON, &ImageFrame::OnApplyFunction, this);
    applyThresholdButton->Bind(wxEVT_BUTTON, &ImageFrame::OnApplyThreshold, this);
    thresholdChoice->Bind(wxEVT_CHOICE, &ImageFrame::OnThresholdChoiceChanged, this);
    refineButton->Bind(wxEVT_BUTTON, &ImageFrame::OnApplyRefining, this);
}


/////////////////////

void ImageFrame::OnLoadImage(wxCommandEvent&) {
    wxFileDialog openFileDialog(this, "Choisir une image", "", "", "Fichiers image (*.jpg;*.png)|*.jpg;*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    std::string filePath = openFileDialog.GetPath().ToStdString();
    originalImage = cv::imread(filePath);
    if (originalImage.empty()) {
        wxMessageBox("Impossible de charger l'image", "Erreur", wxICON_ERROR);
        return;
    }

    cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2GRAY);
    DisplayImage(originalImage, originalBitmap);
}

void ImageFrame::OnApplyFunction(wxCommandEvent&) {
    if (originalImage.empty()) return;

    if (!mainSizer->GetItem(thresholdSizer)) {
        mainSizer->Add(thresholdSizer, 0, wxEXPAND | wxALL, 10);
    }
    thresholdSizer->Show(true);  
    thresholdChoice->Show(true); 
    applyThresholdButton->Show(true); 

    thresholdSlider->Hide();
    mainSizer->Layout();

    thresholdedBitmap->SetBitmap(wxBitmap(300, 300));
    thresholdedBitmap->Refresh();
    refinedBitmap->SetBitmap(wxBitmap(300, 300));
    refinedBitmap->Refresh();

    refineButton->Show(false);
    refineButton->SetLabel("Activer l'affinage");

    processedImage = detectEdges(originalImage, static_cast<FilterType>(filterChoice->GetSelection()),
                                 static_cast<EdgeDetectionStrategy>(strategyChoice->GetSelection()), 
                                 useColorCheckBox->IsChecked());

    DisplayImage(processedImage, processedBitmap);

    mainSizer->Layout();
}


void ImageFrame::OnThresholdChoiceChanged(wxCommandEvent&) {
    bool isManual = (thresholdChoice->GetSelection() == 2);
    thresholdSlider->Show(isManual);
    applyThresholdButton->Show(true);

    thresholdedBitmap->SetBitmap(wxBitmap(300, 300));  
    thresholdedBitmap->Refresh();
    refinedBitmap->SetBitmap(wxBitmap(300, 300)); 
    refinedBitmap->Refresh();

    refineButton->Show(false);
    refineButton->SetLabel("Activer l'affinage");

    mainSizer->Layout();
}


void ImageFrame::OnApplyThreshold(wxCommandEvent&) {
    if (processedImage.empty()) return;
    
    int thresholdValue;
    switch (thresholdChoice->GetSelection()) {
        case 0: // moyenne
            thresholdValue = computeThreshold(processedImage, false);
            thresholdedImage = applyThreshold(processedImage, thresholdValue);
            break;
        case 1: // median 
            thresholdValue = computeThreshold(processedImage, true);
            thresholdedImage = applyThreshold(processedImage, thresholdValue);
            break;
        case 2: // seuil recup 
            thresholdValue = thresholdSlider->GetValue();
            thresholdedImage = applyThreshold(processedImage, thresholdValue);
            break;
        case 3: {
            auto [low, high] = estimateHysteresisThresholds(processedImage);
            thresholdedImage = applyHysteresisThresholding(processedImage, low, high);
            break;
        }
        default:
            thresholdValue = computeThreshold(processedImage, false);
            thresholdedImage = applyThreshold(processedImage, thresholdValue);
            break;
    }
    DisplayImage(thresholdedImage, thresholdedBitmap);

    refineButton->Show(true);
    mainSizer->Layout();
}


void ImageFrame::OnApplyRefining(wxCommandEvent&) {
    if (thresholdedImage.empty()) return;

    bool isRefining = (refineButton->GetLabel() == "Activer l'affinage");

    if (isRefining) {
        refinedImage = zhangSuenPrep(thresholdedImage.clone());
        DisplayImage(refinedImage, refinedBitmap);
        refineButton->SetLabel("Désactiver l'affinage");
    } else {
        refinedBitmap->SetBitmap(wxBitmap(300, 300));
        refinedBitmap->Refresh();
        refineButton->SetLabel("Activer l'affinage");
    }

    mainSizer->Layout();
}


void ImageFrame::DisplayImage(const cv::Mat& image, wxStaticBitmap* bitmap) {
    if (image.empty()) return;

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(300, 300));
    cv::Mat cloned = resizedImage.clone();

    if (cloned.channels() == 1) {
        cv::cvtColor(cloned, cloned, cv::COLOR_GRAY2RGB);
    }

    wxImage wxImg(cloned.cols, cloned.rows, cloned.data, true);
    bitmap->SetBitmap(wxBitmap(wxImg));
    bitmap->Refresh();
    bitmap->Update();
}