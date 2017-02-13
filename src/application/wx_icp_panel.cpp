// author: dybisz

#include "application/wx_icp_panel.h"

//------------------------------------------------------------------------------
ICPPanel::ICPPanel(wxWindow *parent, const wxPoint &pos)
        : wxPanel(parent, wxID_ANY, pos, wxSize(-1, -1), wxBORDER_NONE) {

    this->SetBackgroundColour(wxColour(255, 255, 255));
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    __InitializeAnimationSliderBox(vbox);
    __InitializeFramesAndTimeBox(vbox);
    __InitializeNextPrevBox(vbox);
    __InitializeResetBox(vbox);
    __InitializeICPTypeBox(vbox);

    vbox->SetSizeHints(this);
    this->SetSizer(vbox);
}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeAnimationSliderBox(wxBoxSizer *parent) {
    __m_animationSlider = new wxSlider(this, ID_ANIMATION_SLIDER, 0, 0, 100,
                               wxPoint(10, 30), wxSize(240, -1));
    parent->Add(__m_animationSlider, 0, wxALIGN_CENTER_HORIZONTAL);
}

//------------------------------------------------------------------------------
void
ICPPanel::__InitializeFramesAndTimeBox(wxBoxSizer *parent) {

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

}