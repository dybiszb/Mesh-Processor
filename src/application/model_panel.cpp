// author: dybisz

#include "application/model_panel.h"

//------------------------------------------------------------------------------
ModelPanel::ModelPanel(wxWindow *parent, const wxPoint &pos)
        : m_selectedMeshText(NULL),
          wxPanel(parent, wxID_ANY, pos, wxSize(-1, -1), wxBORDER_NONE) {

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    /* ----- Mesh Name ----- */
    initializeMeshName(vbox);


    /* ----- Mesh Translation ----- */
    wxBoxSizer *hboxTranslation = new wxBoxSizer(wxHORIZONTAL);
    auto translation = new wxStaticText(this, wxID_ANY, wxT(" Translation:"),
                                        wxPoint(-10, 10), wxSize(105, 30));
    m_translationXText = new wxTextCtrl(this, wxID_ANY, wxString("0x0"),
                                        wxDefaultPosition, wxSize(40, -1));
    m_translationYText = new wxTextCtrl(this, wxID_ANY, wxString("10x0"),
                                        wxDefaultPosition, wxSize(40, -1));
    m_translationZText = new wxTextCtrl(this, wxID_ANY, wxString("20x0"),
                                        wxDefaultPosition, wxSize(40, -1));
    hboxTranslation->Add(translation, 1, wxALL, 5);
    hboxTranslation->Add(m_translationXText, 0, wxALL, 1);
    hboxTranslation->Add(m_translationYText, 0, wxALL, 1);
    hboxTranslation->Add(m_translationZText, 0, wxALL, 1);


    vbox->Add(hboxTranslation, 1, wxALL, 0);
    vbox->SetSizeHints(this);
    this->SetSizer(vbox);
}

//------------------------------------------------------------------------------
void ModelPanel::initializeMeshName(wxBoxSizer *parent) {
    wxBoxSizer *hboxName = new wxBoxSizer(wxHORIZONTAL);
    auto title = new wxStaticText(this, wxID_ANY, wxT(" Selected Mesh:"),
                                  wxPoint(-10, 10), wxSize(105, 30));
    m_selectedMeshText = new wxTextCtrl(this, wxID_ANY, wxString("0x0"),
                                        wxDefaultPosition, wxSize(120, -1));
    m_selectedMeshText->Disable();
    hboxName->Add(title, 0, wxALIGN_CENTER_VERTICAL);
    hboxName->Add(m_selectedMeshText, 0, wxALL, 1);

    (*parent).Add(hboxName, 1, wxALL, 0);
}

//------------------------------------------------------------------------------
void
ModelPanel::setSelectedText(const string &text) {
    if(m_selectedMeshText) {
        m_selectedMeshText->Clear();
        *m_selectedMeshText << text;
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

