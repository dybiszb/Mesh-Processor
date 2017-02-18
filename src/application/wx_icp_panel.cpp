// author: dybisz

#include "application/wx_icp_panel.h"

//------------------------------------------------------------------------------
ICPPanel::ICPPanel(wxWindow *parent, const wxPoint &pos)
        : wxPanel(parent, wxID_ANY, pos, wxSize(-1, -1), wxBORDER_NONE),
          __m_lastIndex(0), __m_currentIndex(0),
          __m_useNearestNeighbors(false),
          __m_useSubSampling(false),
          __m_samples(-1) {

    this->SetBackgroundColour(wxColour(255, 255, 255));
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    __InitializeNextPrevBox(vbox);
    __InitializeAnimationSliderBox(vbox);
    __InitializeFramesAndTimeBox(vbox);
    __InitializeResetBox(vbox);
    __InitializeICPTypeBox(vbox);
    __InitializeBottomCheckboxes(vbox);

    SetActive(false);
    vbox->SetSizeHints(this);
    this->SetSizer(vbox);
}

//------------------------------------------------------------------------------
void
ICPPanel::SetActive(bool isActive) { // TODO: fix indices and change this enable
    if (__m_previousFrameButton) __m_previousFrameButton->Enable(false);
    if (__m_nextFrameButton) __m_nextFrameButton->Enable(isActive);
    if (__m_animationSlider) __m_animationSlider->Enable(isActive);
    if (__m_resetButton) __m_resetButton->Enable(isActive);

    if (__m_framesRateText) {
        if (!isActive) SetFramesRate(0, 0);
        __m_framesRateText->Enable(isActive);
    }

    if (__m_timeText) {
        if (!isActive) SetComputationTime(0);
        __m_timeText->Enable(isActive);
    }
    if (!isActive) {
        __m_results.clear();
        __m_lastIndex = 0;
        __m_currentIndex = 0;
    }

    // Opposite actions for run button and the radio box
    if (__m_runButton) __m_runButton->Enable(!isActive);
    if (__m_icpTypeRadioBox) __m_icpTypeRadioBox->Enable(!isActive);
    if (__m_nearestNeighboursCheckbox) __m_nearestNeighboursCheckbox->Enable
                (!isActive);
    if(__m_subSamplingCheckbox) __m_subSamplingCheckbox->Enable(!isActive);
    if(__m_subSamplingTextCtrl) __m_subSamplingTextCtrl->Enable(__m_useSubSampling);
}

//------------------------------------------------------------------------------
void
ICPPanel::SetNearestNeighbors(bool nns) {
    __m_useNearestNeighbors = nns;
}

//------------------------------------------------------------------------------
void
ICPPanel::SetFramesRate(int frame, unsigned long outOf) {
    string s = "Iterations: " + to_string(frame) + "/" + to_string(outOf);
    __m_framesRateText->SetLabel(s);

    // Update slider
    __m_animationSlider->SetRange(0, (int) outOf);
    __m_animationSlider->SetTickFreq(1);
    __m_animationSlider->SetValue(frame);
    __m_animationSlider->Refresh();
}

//------------------------------------------------------------------------------
void
ICPPanel::SetComputationTime(double time) {
    string s = "Comp. Time: " + to_string(time) + " [s]";
    __m_timeText->SetLabel(s);
}

//------------------------------------------------------------------------------
const ICPResults &
ICPPanel::NextFrame() {
    if (__m_currentIndex >= __m_results.size()) {
        string errInfo = "ICPResults out of scope";
        throw errInfo;
    }
    SetFramesRate(__m_currentIndex + 1, __m_results.size());
    return __m_results[__m_currentIndex++];
}

//------------------------------------------------------------------------------
const ICPResults &
ICPPanel::PrevFrame() {
    if ((__m_currentIndex) < 0) {
        string errInfo = "ICPResults out of scope";
        throw errInfo;
    }
    SetFramesRate(__m_currentIndex, __m_results.size());
    int temp = __m_currentIndex - 1;
    __m_currentIndex--;
    return __m_results[temp];
}

//------------------------------------------------------------------------------
void
ICPPanel::RunICP(const PointsCloud &m1PC, const PointsCloud &m2PC) {
    // Read Samples


    clock_t begin = clock();
    __m_results = __m_icpAlgorithm.pointToPointsICP(m1PC,
                                                    m2PC,
                                                    __m_useNearestNeighbors,
                                                    __m_samples);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    // Update Widgets
    SetActive(true);
    SetFramesRate(0, __m_results.size());
    SetComputationTime(elapsed_secs);
}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeAnimationSliderBox(wxBoxSizer *parent) {
    __m_animationSlider = new wxSlider(this, ID_ANIMATION_SLIDER, 0, 0, 0,
                                       wxPoint(10, 30), wxSize(240, -1));

    parent->Add(__m_animationSlider, 0, wxALIGN_CENTER_HORIZONTAL, 4);
}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeFramesAndTimeBox(wxBoxSizer *parent) {
    __m_framesRateText = new wxStaticText(this, wxID_ANY,
                                          wxT("Iterations: 0/0 "),
                                          wxPoint(-10, 10),
                                          wxSize(200, 20));

    __m_timeText = new wxStaticText(this, wxID_ANY,
                                    wxT("Comp. Time: 0 [s]"),
                                    wxPoint(-10, 10),
                                    wxSize(200, 20));

    parent->Add(__m_framesRateText, 0, wxALIGN_CENTER_HORIZONTAL, 1);
    parent->Add(__m_timeText, 0, wxALIGN_CENTER_HORIZONTAL, 1);
}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeNextPrevBox(wxBoxSizer *parent) {
    auto hbox = new wxBoxSizer(wxHORIZONTAL);
    __m_nextFrameButton = new wxButton(this, ID_NEXT_FRAME_ICP_BUTTON,
                                       wxT("Next Frame"), wxPoint
                                               (-10, 10), wxSize(121, -1));
    __m_previousFrameButton = new wxButton(this, ID_NEXT_PREV_ICP_BUTTON,
                                           wxT("Previous Frame"), wxPoint
                                                   (-10, 10), wxSize(121, -1));
    hbox->Add(__m_previousFrameButton, 0, wxEXPAND);
    hbox->Add(__m_nextFrameButton, 0, wxEXPAND);
    parent->Add(hbox, 0, wxALIGN_CENTER_HORIZONTAL, 4);
}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeResetBox(wxBoxSizer *parent) {

}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeICPTypeBox(wxBoxSizer *parent) {
    static wxString choices[] =
            {wxT("point-to-point"), wxT("point-to-plane")};
    auto rightVbox = new wxBoxSizer(wxVERTICAL);
    auto hbox = new wxBoxSizer(wxHORIZONTAL);

    // Radio Box
    __m_icpTypeRadioBox = new wxRadioBox(this, wxID_ANY, wxT("Type"),
                                         wxDefaultPosition, wxSize(140, -1),
                                         WXSIZEOF(choices), choices,
                                         1, wxRA_SPECIFY_COLS);
    __m_icpTypeRadioBox->SetSelection(POINT_TO_POINT);
    hbox->Add(__m_icpTypeRadioBox, 0, wxALL, 2);

    // Buttons
    __m_runButton = new wxButton(this, ID_RUN_ICP_BUTTON, wxT("Run"), wxPoint
            (-10, 10), wxSize(80, -1));
    __m_resetButton = new wxButton(this, ID_RESET_ICP_BUTTON, wxT("Reset"),
                                   wxPoint(-10, 10), wxSize(80, -1));
    rightVbox->Add(__m_runButton, 0, wxALL, 1);
    rightVbox->Add(__m_resetButton, 0, wxALL, 2);
    hbox->Add(rightVbox, 0, wxALIGN_CENTER_VERTICAL, 1);

    parent->Add(hbox, 0, wxALIGN_CENTER_HORIZONTAL, 4);

}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeBottomCheckboxes(wxBoxSizer *parent) {

    __m_nearestNeighboursCheckbox = new wxCheckBox(this,
                                                   ID_NEAREST_NEIGHBOURS_CHECKBOX,
                                                   "Use nearest neighbour "
                                                           "K-tree.");
    __m_nearestNeighboursCheckbox->SetValue(__m_useNearestNeighbors);
    parent->Add(__m_nearestNeighboursCheckbox, 0, wxALIGN_LEFT, 4);

    // Sub Sampling
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    __m_subSamplingCheckbox = new wxCheckBox(this,
                                                   ID_SUB_SAMPLING_CHECKBOX,
                                                   "Use subsampling");
    __m_subSamplingCheckbox->SetValue(__m_useNearestNeighbors);
    __m_subSamplingTextCtrl = new wxTextCtrl(this,
                                             ID_SUB_SAMPLING_TEXT,
                                             wxString(""),
                                             wxDefaultPosition,
                                             wxSize(45, -1),
                                             wxTE_PROCESS_ENTER);
    __m_subSamplingTextCtrl->Enable(false);
    hbox->Add(__m_subSamplingCheckbox, 0, wxALIGN_CENTER_VERTICAL, 4);
    hbox->Add(__m_subSamplingTextCtrl, 0, wxALL, 4);
    parent->Add(hbox, 0, wxALIGN_LEFT, 4);
}


//------------------------------------------------------------------------------
void
ICPPanel::__OnSubSamplesCheckbox(wxCommandEvent &event) {
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    __m_useSubSampling = source->IsChecked();
    __m_subSamplingTextCtrl->Enable(source->IsChecked());
    if(!__m_useSubSampling) __m_samples = -1;
}

//------------------------------------------------------------------------------
void
ICPPanel::__OnSubSampleTextCtrl(wxCommandEvent &event) {
    double samples;
    (__m_subSamplingTextCtrl->GetValue()).ToDouble( &samples);
    __m_samples = (int) samples;
}

//------------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(ICPPanel, wxPanel)
                EVT_CHECKBOX(ID_SUB_SAMPLING_CHECKBOX,
                             ICPPanel::__OnSubSamplesCheckbox)
                EVT_TEXT_ENTER(ID_SUB_SAMPLING_TEXT,
                               ICPPanel::__OnSubSampleTextCtrl)
wxEND_EVENT_TABLE()
