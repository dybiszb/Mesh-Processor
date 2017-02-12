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
    wxTextCtrl *m_translationXText;
    wxTextCtrl *m_translationYText;
    wxTextCtrl *m_translationZText;
    wxTextCtrl *m_rotationXText;
    wxTextCtrl *m_rotationYText;
    wxTextCtrl *m_rotationZText;
    wxTextCtrl *m_scalingXText;
    wxTextCtrl *m_scalingYText;
    wxTextCtrl *m_scalingZText;

    void initializeTranslationBox(wxBoxSizer *parent);

    void initializeRotationBox(wxBoxSizer *parent);

    void initializeScalingBox(wxBoxSizer *parent);

public:
    ModelPanel(wxWindow *parent, const wxPoint &pos);

    void setActive(bool isActive);

    void setTranslationText(float x, float y, float z);

    void setRotation(float roll, float yaw, float pitch);

    void setScale(float x, float y, float z);
};

#endif
