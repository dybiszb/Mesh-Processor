// author: dybisz

#include "application/wx_model_panel.h"

//------------------------------------------------------------------------------
ModelPanel::ModelPanel(wxWindow *parent, const wxPoint &pos)
        : wxPanel(parent, wxID_ANY, pos, wxSize(-1, -1), wxBORDER_NONE) {

    this->SetBackgroundColour(wxColour(255, 255, 255));
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    __InitializeTranslationBox(vbox);
    __InitializeRotationBox(vbox);
    __InitializeScalingBox(vbox);
    __InitializeNoiseBox(vbox);
    __InitializeMoveCentroidToOrigin(vbox);
    __InitializeBottomCheckboxes(vbox);

    vbox->SetSizeHints(this);
    this->SetSizer(vbox);
}

//----------------------------------------------------------------------------
void
ModelPanel::__InitializeTranslationBox(wxBoxSizer *parent) {
    wxBoxSizer *hboxTranslation = new wxBoxSizer(wxHORIZONTAL);

    auto dummyText = new wxStaticText(this, wxID_ANY,
                                      wxT(""),
                                      wxPoint(-10, 10),
                                      wxSize(90, 8));
    auto translationText = new wxStaticText(this,
                                            wxID_ANY,
                                            wxT(" Translation:"),
                                            wxPoint(-10, 10),
                                            wxSize(85, 30));
    __m_translationXText = new wxTextCtrl(this,
                                          ID_TEXT_TRANSLATION_X,
                                          wxString(""),
                                          wxDefaultPosition,
                                          wxSize(45, -1),
                                          wxTE_PROCESS_ENTER);
    __m_translationYText = new wxTextCtrl(this,
                                          ID_TEXT_TRANSLATION_Y,
                                          wxString(""),
                                          wxDefaultPosition,
                                          wxSize(45, -1),
                                          wxTE_PROCESS_ENTER);
    __m_translationZText = new wxTextCtrl(this,
                                          ID_TEXT_TRANSLATION_Z,
                                          wxString(""),
                                          wxDefaultPosition,
                                          wxSize(45, -1),
                                          wxTE_PROCESS_ENTER);

    hboxTranslation->Add(translationText, 1, wxALL, 5);
    hboxTranslation->Add(__m_translationXText, 0, wxALL, 1);
    hboxTranslation->Add(__m_translationYText, 0, wxALL, 1);
    hboxTranslation->Add(__m_translationZText, 0, wxALL, 1);

    parent->Add(dummyText, 0, wxALL, 0);
    parent->Add(hboxTranslation, 1, wxALL, 0);
}

//----------------------------------------------------------------------------
void
ModelPanel::__InitializeRotationBox(wxBoxSizer *parent) {
    wxBoxSizer *hboxRotation = new wxBoxSizer(wxHORIZONTAL);
    auto rotationText = new wxStaticText(this, wxID_ANY, wxT(" Rotation:"),
                                         wxPoint(-10, 10), wxSize(85, 30));
    __m_rotationXText = new wxTextCtrl(this, ID_TEXT_ROTATION_X, wxString(""),
                                       wxDefaultPosition, wxSize(45, -1),
                                       wxTE_PROCESS_ENTER);
    __m_rotationYText = new wxTextCtrl(this, ID_TEXT_ROTATION_Y, wxString(""),
                                       wxDefaultPosition, wxSize(45, -1),
                                       wxTE_PROCESS_ENTER);
    __m_rotationZText = new wxTextCtrl(this, ID_TEXT_ROTATION_Z, wxString(""),
                                       wxDefaultPosition, wxSize(45, -1),
                                       wxTE_PROCESS_ENTER);
    hboxRotation->Add(rotationText, 1, wxALL, 5);
    hboxRotation->Add(__m_rotationXText, 0, wxALL, 1);
    hboxRotation->Add(__m_rotationYText, 0, wxALL, 1);
    hboxRotation->Add(__m_rotationZText, 0, wxALL, 1);


    parent->Add(hboxRotation, 1, wxALL, 0);
}

//----------------------------------------------------------------------------
void
ModelPanel::__InitializeScalingBox(wxBoxSizer *parent) {
    wxBoxSizer *hboxScaling = new wxBoxSizer(wxHORIZONTAL);
    auto scalingText = new wxStaticText(this, wxID_ANY, wxT(" Scaling:"),
                                        wxPoint(-10, 10), wxSize(85, 30));
    __m_scalingXText = new wxTextCtrl(this, ID_TEXT_SCALING_X, wxString(""),
                                      wxDefaultPosition, wxSize(45, -1),
                                      wxTE_PROCESS_ENTER);
    __m_scalingYText = new wxTextCtrl(this, ID_TEXT_SCALING_Y, wxString(""),
                                      wxDefaultPosition, wxSize(45, -1),
                                      wxTE_PROCESS_ENTER);
    __m_scalingZText = new wxTextCtrl(this, ID_TEXT_SCALING_Z, wxString(""),
                                      wxDefaultPosition, wxSize(45, -1),
                                      wxTE_PROCESS_ENTER);
    hboxScaling->Add(scalingText, 1, wxALL, 5);
    hboxScaling->Add(__m_scalingXText, 0, wxALL, 1);
    hboxScaling->Add(__m_scalingYText, 0, wxALL, 1);
    hboxScaling->Add(__m_scalingZText, 0, wxALL, 1);


    parent->Add(hboxScaling, 1, wxALL, 0);
}

//------------------------------------------------------------------------------
void
ModelPanel::__InitializeNoiseBox(wxBoxSizer *parent) {
    wxBoxSizer *hboxScaling = new wxBoxSizer(wxHORIZONTAL);

    __m_introduceNoise = new wxButton(this, ID_INTRODUCE_NOISE,
                                      wxT("Introduce Noise"), wxPoint(-10, 10),
                                      wxSize(130, -1));

    auto stdDevText = new wxStaticText(this, wxID_ANY, wxT(" std dev: "),
                                       wxPoint(-10, 10), wxSize(62, 30));
    __m_stddevText = new wxTextCtrl(this, ID_STD_DEV_TEXT, wxString("0.001"),
                                    wxDefaultPosition, wxSize(45, -1));

    hboxScaling->Add(__m_introduceNoise, 0, wxALL, 1);
    hboxScaling->Add(stdDevText, 0, wxALIGN_CENTER_VERTICAL);
    hboxScaling->Add(__m_stddevText, 0, wxALL, 1);


    parent->Add(hboxScaling, 1, wxALL, 0);
}

//------------------------------------------------------------------------------
void
ModelPanel::__InitializeMoveCentroidToOrigin(wxBoxSizer *parent) {
    __m_moveCentroidButton = new wxButton(this, ID_MOVE_TO_ORIGIN,
                                          wxT("Move Centroid to Origin"),
                                          wxPoint
                                                  (-10, 10), wxSize
                                                  (230, -1));

    parent->Add(__m_moveCentroidButton, 0, wxALIGN_CENTER_HORIZONTAL, 2);
}

//------------------------------------------------------------------------------
void
ModelPanel::__InitializeBottomCheckboxes(wxBoxSizer *parent) {
    __m_normalsCheckbox = new wxCheckBox(this, ID_CHECKBOX_NORMALS, "Show "
            "Approximated Normals");
    parent->Add(__m_normalsCheckbox, 0, wxALIGN_LEFT, 4);

    // TODO: shading checkbox to the same hbox
    __m_wireframeCheckbox = new wxCheckBox(this, ID_CHECKBOX_WIREFRAME,
                                           "Wireframe");
    parent->Add(__m_wireframeCheckbox, 0, wxALIGN_LEFT, 4);

    __m_icpBaseCheckbox = new wxCheckBox(this, ID_CHECKBOX_ICP_BASE, "Base "
            "for ICP");
    parent->Add(__m_icpBaseCheckbox, 0, wxALIGN_LEFT, 4);


}

//------------------------------------------------------------------------------
void
ModelPanel::SetActive(bool isActive) {
    __m_translationXText->Enable(isActive);
    __m_translationYText->Enable(isActive);
    __m_translationZText->Enable(isActive);

    if (!isActive) {
        __m_translationXText->Clear();
        __m_translationYText->Clear();
        __m_translationZText->Clear();
    }

    __m_rotationXText->Enable(isActive);
    __m_rotationYText->Enable(isActive);
    __m_rotationZText->Enable(isActive);

    if (!isActive) {
        __m_rotationXText->Clear();
        __m_rotationYText->Clear();
        __m_rotationZText->Clear();
    }

    __m_scalingXText->Enable(isActive);
    __m_scalingYText->Enable(isActive);
    __m_scalingZText->Enable(isActive);

    if (!isActive) {
        __m_scalingXText->Clear();
        __m_scalingYText->Clear();
        __m_scalingZText->Clear();
    }

    __m_introduceNoise->Enable(isActive);
    __m_stddevText->Enable(isActive);
    __m_moveCentroidButton->Enable(isActive);
    __m_normalsCheckbox->Enable(isActive);
    __m_icpBaseCheckbox->Enable(isActive);
    __m_wireframeCheckbox->Enable(isActive);
}


//------------------------------------------------------------------------------
void
ModelPanel::SetTranslationText(float x, float y, float z) {
    if (__m_translationXText) {
        __m_translationXText->Clear();
        *__m_translationXText << x;
    }

    if (__m_translationYText) {
        __m_translationYText->Clear();
        *__m_translationYText << y;
    }

    if (__m_translationZText) {
        __m_translationZText->Clear();
        *__m_translationZText << z;
    }
}

//------------------------------------------------------------------------------
void
ModelPanel::SetRotation(float roll, float yaw, float pitch) {
    if (__m_rotationXText) {
        __m_rotationXText->Clear();
        *__m_rotationXText << roll;
    }

    if (__m_rotationYText) {
        __m_rotationYText->Clear();
        *__m_rotationYText << yaw;
    }

    if (__m_rotationZText) {
        __m_rotationZText->Clear();
        *__m_rotationZText << pitch;
    }
}

//------------------------------------------------------------------------------
void
ModelPanel::SetScale(float x, float y, float z) {
    if (__m_scalingXText) {
        __m_scalingXText->Clear();
        *__m_scalingXText << x;
    }

    if (__m_scalingYText) {
        __m_scalingYText->Clear();
        *__m_scalingYText << y;
    }

    if (__m_scalingZText) {
        __m_scalingZText->Clear();
        *__m_scalingZText << z;
    }
}

//------------------------------------------------------------------------------
void
ModelPanel::SetShowNormals(bool showNormals) {
    if (__m_normalsCheckbox) {
        __m_normalsCheckbox->SetValue(showNormals);
    }
}

//------------------------------------------------------------------------------
void
ModelPanel::SetWireframe(bool wireframe) {
    if(__m_wireframeCheckbox) {
        __m_wireframeCheckbox->SetValue(!wireframe);
    }
}

//------------------------------------------------------------------------------
void
ModelPanel::SetICPBase(bool icpBase) {
    if(__m_icpBaseCheckbox) {
        __m_icpBaseCheckbox->SetValue(icpBase);
    }
}

//------------------------------------------------------------------------------
Eigen::Vector3f
ModelPanel::GetTranslation() {
    double x, y, z;
    (__m_translationXText->GetValue()).ToDouble(&x);
    (__m_translationYText->GetValue()).ToDouble(&y);
    (__m_translationZText->GetValue()).ToDouble(&z);

    return vec3(x, y, z);
}

//------------------------------------------------------------------------------
Eigen::Matrix3f
ModelPanel::GetRotation() {
    double x, y, z;
    (__m_rotationXText->GetValue()).ToDouble(&x);
    (__m_rotationYText->GetValue()).ToDouble(&y);
    (__m_rotationZText->GetValue()).ToDouble(&z);

    return rotationMatrix(x, y, z);
}

//------------------------------------------------------------------------------
Eigen::Vector3f
ModelPanel::GetScale() {
    double x, y, z;
    (__m_scalingXText->GetValue()).ToDouble(&x);
    (__m_scalingYText->GetValue()).ToDouble(&y);
    (__m_scalingZText->GetValue()).ToDouble(&z);

    return vec3(x, y, z);
}

//------------------------------------------------------------------------------
float
ModelPanel::GetStdDev() {
    double stdDev;
    wxString val = __m_stddevText->GetValue();
    val.ToDouble(&stdDev);
    return (float) stdDev;
}