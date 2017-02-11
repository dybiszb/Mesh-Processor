#ifndef MODEL_PANEL_H
#define MODEL_PANEL_H

#include <wx/wx.h>
#include <memory>
#include <string>
#include <iostream>

using namespace std;

class ModelPanel : public wxPanel {
private:
    wxButton *m_minusButton;
    wxButton *m_plusButton;
    wxTextCtrl* m_selectedMeshText;
    wxTextCtrl* m_translationXText;
    wxTextCtrl* m_translationYText;
    wxTextCtrl* m_translationZText;

    void initializeMeshName(wxBoxSizer* parent);
public:
    ModelPanel(wxWindow *parent, const wxPoint &pos);
    void setSelectedText(const string& text);
    void setTranslationText(float x, float y, float z);
};

#endif
