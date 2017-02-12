// author: dybisz

#include "application/model_panel.h"

//------------------------------------------------------------------------------
ModelPanel::ModelPanel(wxWindow *parent, const wxPoint &pos)
        : wxPanel(parent, wxID_ANY, pos, wxSize(-1, -1), wxBORDER_NONE) {

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    initializeTranslationBox(vbox);
    initializeRotationBox(vbox);
    initializeScalingBox(vbox);

    vbox->SetSizeHints(this);
    this->SetSizer(vbox);
}

//----------------------------------------------------------------------------
void
ModelPanel::initializeTranslationBox(wxBoxSizer* parent) {
    wxBoxSizer *hboxTranslation = new wxBoxSizer(wxHORIZONTAL);
    auto translationText = new wxStaticText(this, wxID_ANY, wxT(" Translation:"),
                                            wxPoint(-10, 10), wxSize(90, 30));
    m_translationXText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                        wxDefaultPosition, wxSize(45, -1));
    m_translationYText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                        wxDefaultPosition, wxSize(45, -1));
    m_translationZText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                        wxDefaultPosition, wxSize(45, -1));
    hboxTranslation->Add(translationText, 1, wxALL, 5);
    hboxTranslation->Add(m_translationXText, 0, wxALL, 1);
    hboxTranslation->Add(m_translationYText, 0, wxALL, 1);
    hboxTranslation->Add(m_translationZText, 0, wxALL, 1);


    parent->Add(hboxTranslation, 1, wxALL, 0);
}

//----------------------------------------------------------------------------
void
ModelPanel::initializeRotationBox(wxBoxSizer* parent) {
    wxBoxSizer *hboxRotation = new wxBoxSizer(wxHORIZONTAL);
    auto rotationText = new wxStaticText(this, wxID_ANY, wxT(" Rotation:"),
                                            wxPoint(-10, 10), wxSize(90, 30));
    m_rotationXText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                        wxDefaultPosition, wxSize(45, -1));
    m_rotationYText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                        wxDefaultPosition, wxSize(45, -1));
    m_rotationZText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                        wxDefaultPosition, wxSize(45, -1));
    hboxRotation->Add(rotationText, 1, wxALL, 5);
    hboxRotation->Add(m_rotationXText, 0, wxALL, 1);
    hboxRotation->Add(m_rotationYText, 0, wxALL, 1);
    hboxRotation->Add(m_rotationZText, 0, wxALL, 1);


    parent->Add(hboxRotation, 1, wxALL, 0);
}

//----------------------------------------------------------------------------
void
ModelPanel::initializeScalingBox(wxBoxSizer* parent) {
    wxBoxSizer *hboxScaling = new wxBoxSizer(wxHORIZONTAL);
    auto scalingText = new wxStaticText(this, wxID_ANY, wxT(" Scaling:"),
                                         wxPoint(-10, 10), wxSize(90, 30));
    m_scalingXText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                     wxDefaultPosition, wxSize(45, -1));
    m_scalingYText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                     wxDefaultPosition, wxSize(45, -1));
    m_scalingZText = new wxTextCtrl(this, wxID_ANY, wxString(""),
                                     wxDefaultPosition, wxSize(45, -1));
    hboxScaling->Add(scalingText, 1, wxALL, 5);
    hboxScaling->Add(m_scalingXText, 0, wxALL, 1);
    hboxScaling->Add(m_scalingYText, 0, wxALL, 1);
    hboxScaling->Add(m_scalingZText, 0, wxALL, 1);


    parent->Add(hboxScaling, 1, wxALL, 0);
}

//----------------------------------------------------------------------------
void
ModelPanel::setActive(bool isActive) {
    m_translationXText->Enable(isActive);
    m_translationYText->Enable(isActive);
    m_translationZText->Enable(isActive);

    if(!isActive) {
        m_translationXText->Clear();
        m_translationYText->Clear();
        m_translationZText->Clear();
    }

    m_rotationXText->Enable(isActive);
    m_rotationYText->Enable(isActive);
    m_rotationZText->Enable(isActive);

    if(!isActive) {
        m_rotationXText->Clear();
        m_rotationYText->Clear();
        m_rotationZText->Clear();
    }

    m_scalingXText->Enable(isActive);
    m_scalingYText->Enable(isActive);
    m_scalingZText->Enable(isActive);

    if(!isActive) {
        m_scalingXText->Clear();
        m_scalingYText->Clear();
        m_scalingZText->Clear();
    }

}


//------------------------------------------------------------------------------
void
ModelPanel::setTranslationText(float x, float y, float z) {
    if(m_translationXText) {
        m_translationXText->Clear();
        *m_translationXText << x;
    }

    if(m_translationYText) {
        m_translationYText->Clear();
        *m_translationYText << y;
    }

    if(m_translationZText) {
        m_translationZText->Clear();
        *m_translationZText << z;
    }
}

//------------------------------------------------------------------------------
void
ModelPanel::setRotation(float roll, float yaw, float pitch) {
    if(m_rotationXText) {
        m_rotationXText->Clear();
        *m_rotationXText << roll;
    }

    if(m_rotationYText) {
        m_rotationYText->Clear();
        *m_rotationYText << yaw;
    }

    if(m_rotationZText) {
        m_rotationZText->Clear();
        *m_rotationZText << pitch;
    }
}

//------------------------------------------------------------------------------
void
ModelPanel::setScale(float x, float y, float z) {
    if(m_scalingXText) {
        m_scalingXText->Clear();
        *m_scalingXText << x;
    }

    if(m_scalingYText) {
        m_scalingYText->Clear();
        *m_scalingYText << y;
    }

    if(m_scalingZText) {
        m_scalingZText->Clear();
        *m_scalingZText << z;
    }
}