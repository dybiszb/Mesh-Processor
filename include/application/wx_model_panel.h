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
public:
    ModelPanel(wxWindow *parent, const wxPoint &pos);

    void setActive(bool isActive);

    void setTranslationText(float x, float y, float z);

    void setRotation(float roll, float yaw, float pitch);

    void setScale(float x, float y, float z);

    void setShowNormals(bool showNormals);

    void setICPBase(bool icpBase);

    Vector3f getTranslation();

    Matrix3f getRotation();

    Vector3f getScale();

    float getStdDev();

private:
    wxTextCtrl *__m_translationXText;
    wxTextCtrl *__m_translationYText;
    wxTextCtrl *__m_translationZText;
    wxTextCtrl *__m_rotationXText;
    wxTextCtrl *__m_rotationYText;
    wxTextCtrl *__m_rotationZText;
    wxTextCtrl *__m_scalingXText;
    wxTextCtrl *__m_scalingYText;
    wxTextCtrl *__m_scalingZText;
    wxButton   *__m_introduceNoise;
    wxTextCtrl *__m_stddevText;
    wxButton   *__m_moveCentroidButton;
    wxCheckBox *__m_normalsCheckbox;
    wxCheckBox *__m_icpBaseCheckbox;

    void __InitializeTranslationBox(wxBoxSizer *parent);
    void __InitializeRotationBox(wxBoxSizer *parent);
    void __InitializeScalingBox(wxBoxSizer *parent);
    void __InitializeNoiseBox(wxBoxSizer *parent);
    void __InitializeMoveCentroidToOrigin(wxBoxSizer *parent);
    void __InitializeBottomCheckboxes(wxBoxSizer *parent);

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
    ID_CHECKBOX_ICP_BASE = wxID_HIGHEST + 63,
};

#endif
