// author: dybisz

#include "application/wx_icp_panel.h"

//------------------------------------------------------------------------------
ICPPanel::ICPPanel(wxWindow *parent, const wxPoint &pos)
        : wxPanel(parent, wxID_ANY, pos, wxSize(-1, -1), wxBORDER_NONE),
          __m_lastIndex(0), __m_currentIndex(0) {

    this->SetBackgroundColour(wxColour(255, 255, 255));
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    __InitializeAnimationSliderBox(vbox);
    __InitializeFramesAndTimeBox(vbox);
    __InitializeNextPrevBox(vbox);
    __InitializeResetBox(vbox);
    __InitializeICPTypeBox(vbox);

    SetActive(false);
    vbox->SetSizeHints(this);
    this->SetSizer(vbox);
}

//------------------------------------------------------------------------------
void
ICPPanel::SetFramesRate(int frame, int outOf) {
    stringstream ss;
    ss << "Frames: " << frame << "/" << outOf;
    __m_framesRateText->SetLabel(ss.str());
}

//------------------------------------------------------------------------------
void
ICPPanel::SetActive(bool isActive) {
    if(__m_animationSlider) __m_animationSlider->Enable(isActive);
    if(__m_resetButton) __m_resetButton->Enable(isActive);

    if(__m_framesRateText) {
        if(!isActive) SetFramesRate(0,0);
        __m_framesRateText->Enable(isActive);
    }

    if(__m_timeText) {
        __m_timeText->Enable(isActive);
    }

    __m_results.clear();
    __m_lastIndex = 0;
    __m_currentIndex = 0;

    // Opposite actions for run button and the radio box
    if(__m_runButton) __m_runButton->Enable(!isActive);
    if(__m_icpTypeRadioBox) __m_icpTypeRadioBox->Enable(!isActive);
}

//------------------------------------------------------------------------------
void
ICPPanel::RunICP(const PointsCloud& m1PC, const PointsCloud& m2PC) {
    __m_results = __m_icpAlgorithm.pointToPointsICP(m1PC, m2PC);

    // Update Widgets
    SetActive(true);
    SetFramesRate(0, (int) __m_results.size());
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
                                          wxT("Frames: 0/0 "),
                                          wxPoint(-10, 10),
                                          wxSize(200, 20));

    __m_timeText = new wxStaticText(this, wxID_ANY,
                                          wxT("Time: 0 [ms]"),
                                          wxPoint(-10, 10),
                                          wxSize(200, 20));

    parent->Add(__m_framesRateText, 0, wxALIGN_CENTER_HORIZONTAL, 1);
    parent->Add(__m_timeText, 0, wxALIGN_CENTER_HORIZONTAL, 1);
}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeNextPrevBox(wxBoxSizer *parent) {

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

