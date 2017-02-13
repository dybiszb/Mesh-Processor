#ifndef MODEL_PANEL_H
#define MODEL_PANEL_H

#include <wx/wx.h>
#include <memory>
#include <string>
#include <iostream>

using namespace std;

class ModelPanel : public wxPanel {
private:

    wxTextCtrl *m_translationXText;
    wxTextCtrl *m_translationYText;
    wxTextCtrl *m_translationZText;
    wxTextCtrl *m_rotationXText;
    wxTextCtrl *m_rotationYText;
    wxTextCtrl *m_rotationZText;
    wxTextCtrl *m_scalingXText;
    wxTextCtrl *m_scalingYText;
    wxTextCtrl *m_scalingZText;
    wxButton   *m_introduceNoise;
    wxTextCtrl *m_stddevText;
    wxButton * m_moveCentroidButton;
    wxCheckBox *m_normalsCheckbox;

    void initializeTranslationBox(wxBoxSizer *parent);

    void initializeRotationBox(wxBoxSizer *parent);

    void initializeScalingBox(wxBoxSizer *parent);

    void initializeNoiseBox(wxBoxSizer *parent);

    void initializeMoveCentroidToOrigin(wxBoxSizer * parent);

    void initializeNormalsBox(wxBoxSizer *parent);

public:
    ModelPanel(wxWindow *parent, const wxPoint &pos);

    void setActive(bool isActive);

    void setTranslationText(float x, float y, float z);

    void setRotation(float roll, float yaw, float pitch);

    void setScale(float x, float y, float z);

    void setShowNormals(bool showNormals);

    float getStdDev();
};

enum {
    ID_INTRODUCE_NOISE = wxID_HIGHEST + 50,
    ID_STD_DEV_TEXT = wxID_HIGHEST + 51,
    ID_MOVE_TO_ORIGIN = wxID_HIGHEST + 52,
    ID_CHECKBOX_NORMALS = wxID_HIGHEST + 53,
};

#endif
