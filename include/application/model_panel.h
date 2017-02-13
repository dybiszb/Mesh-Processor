#ifndef MODEL_PANEL_H
#define MODEL_PANEL_H

#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <wx/wx.h>
#include <memory>
#include <string>
#include <iostream>
#include "rendering/gl_eigen.h"

using namespace std;
using namespace Eigen;

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
    wxButton *m_introduceNoise;
    wxTextCtrl *m_stddevText;
    wxButton *m_moveCentroidButton;
    wxCheckBox *m_normalsCheckbox;

    void initializeTranslationBox(wxBoxSizer *parent);

    void initializeRotationBox(wxBoxSizer *parent);

    void initializeScalingBox(wxBoxSizer *parent);

    void initializeNoiseBox(wxBoxSizer *parent);

    void initializeMoveCentroidToOrigin(wxBoxSizer *parent);

    void initializeNormalsBox(wxBoxSizer *parent);

public:
    ModelPanel(wxWindow *parent, const wxPoint &pos);

    void setActive(bool isActive);

    void setTranslationText(float x, float y, float z);

    void setRotation(float roll, float yaw, float pitch);

    void setScale(float x, float y, float z);

    void setShowNormals(bool showNormals);

    Vector3f getTranslation();

    Matrix3f getRotation();

    Vector3f getScale();

    float getStdDev();

};

enum {
    ID_INTRODUCE_NOISE = wxID_HIGHEST + 50,
    ID_STD_DEV_TEXT = wxID_HIGHEST + 51,
    ID_MOVE_TO_ORIGIN = wxID_HIGHEST + 52,
    ID_CHECKBOX_NORMALS = wxID_HIGHEST + 53,
    ID_TEXT_TRANSLATION_X = wxID_HIGHEST + 54,
    ID_TEXT_TRANSLATION_Y = wxID_HIGHEST + 55,
    ID_TEXT_TRANSLATION_Z = wxID_HIGHEST + 56,
    ID_TEXT_ROTATION_X = wxID_HIGHEST + 57,
    ID_TEXT_ROTATION_Y = wxID_HIGHEST + 58,
    ID_TEXT_ROTATION_Z = wxID_HIGHEST + 59,
    ID_TEXT_SCALING_X = wxID_HIGHEST + 60,
    ID_TEXT_SCALING_Y = wxID_HIGHEST + 61,
    ID_TEXT_SCALING_Z = wxID_HIGHEST + 62,
};

#endif
