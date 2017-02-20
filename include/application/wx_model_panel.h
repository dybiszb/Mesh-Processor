//==============================================================================
// The class is in charge of 'Mesh Options' panel and encapsulates all 'get/set'
// functionality, normally expected from an interface. Note that no
// calculations regarding the model currently displayed in the viewer are done
// within the class' body. It only displays and is able to share entries of its
// fields.
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

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

class ModelPanel : public wxPanel {
public:
    ModelPanel(wxWindow *parent, const wxPoint &pos);

    /**
     * Enables or disables all child widgets. Note that all text-related
     * widgets' entries will be cleared.
     *
     * @param isActive Activate or deactivate the panel.
     */
    void SetActive(bool isActive);

    /**
     * Updates 'Translation' widget's fields.
     *
     * @param x New value of translation on X axis.
     * @param y New value of translation on Y axis.
     * @param z New value of translation on Z axis.
     */
    void SetTranslationText(float x, float y, float z);

    /**
     * Updates 'Rotation' widget's fields.
     *
     * @param roll  New value of rotation with respect to X axis.
     * @param yaw   New value of rotation with respect to Y axis.
     * @param pitch New value of rotation with respect to Z axis.
     */
    void SetRotation(float roll, float yaw, float pitch);

    /**
     * Updates 'Scale' widget's fields.
     *
     * @param x New value of scale of X coordinates.
     * @param y New value of scale of Y coordinates.
     * @param z New value of scale of Z coordinates.
     */
    void SetScale(float x, float y, float z);

    /**
     * Updates 'Show approximated normals' checkbox.
     *
     * @param showNormals Check or uncheck (true or false) the checkbox.
     */
    void SetShowNormals(bool showNormals);

    /**
     * Updates 'Wireframe' checkbox
     *
     * @param wireframe Check or uncheck (true or false) the checkbox
     */
    void SetWireframe(bool wireframe);

    /**
     * Updates 'Set as ICP base' checkbox,
     *
     * @param icpBase Check or uncheck (true or false) the checkbox.
     */
    void SetICPBase(bool icpBase);

    /**
     * Reads data stored in 'Translation' widget and returns them in form of
     * a vector.
     *
     * @return A vector of the following form: (t_x, t_y, t_z).
     */
    vec3 GetTranslation();

    /**
     * Reads data stored in 'Rotation' widget and returns them in form of
     * a vector.
     *
     * @return A vector of the following form: (roll, yaw, pitch). The entries
     *         are not guaranteed to be in a particular units like e.g. degrees.
     */
    mat3 GetRotation();

    /**
     * Reads data stored in 'Scale' widget and returns them in form of
     * a vector
     *
     * @return A vector of the following form: (s_x, s_y, s_z).
     */
    vec3 GetScale();

    /**
     * Reads data stored in 'Introduce Noise' section and returns it in form of
     * a float.
     *
     * @return Current value of standard deviation stored in the corresponding
     *         widget.
     */
    float GetStdDev();

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
    wxCheckBox *__m_wireframeCheckbox;

    /* ----- Widgets Initializers ----- */
    void __InitializeTranslationBox(wxBoxSizer *parent);
    void __InitializeRotationBox(wxBoxSizer *parent);
    void __InitializeScalingBox(wxBoxSizer *parent);
    void __InitializeNoiseBox(wxBoxSizer *parent);
    void __InitializeMoveCentroidToOrigin(wxBoxSizer *parent);
    void __InitializeBottomCheckboxes(wxBoxSizer *parent);

};

enum {
    ID_INTRODUCE_NOISE    = wxID_HIGHEST + 50,
    ID_STD_DEV_TEXT       = wxID_HIGHEST + 51,
    ID_MOVE_TO_ORIGIN     = wxID_HIGHEST + 52,
    ID_CHECKBOX_NORMALS   = wxID_HIGHEST + 53,
    ID_TEXT_TRANSLATION_X = wxID_HIGHEST + 54,
    ID_TEXT_TRANSLATION_Y = wxID_HIGHEST + 55,
    ID_TEXT_TRANSLATION_Z = wxID_HIGHEST + 56,
    ID_TEXT_ROTATION_X    = wxID_HIGHEST + 57,
    ID_TEXT_ROTATION_Y    = wxID_HIGHEST + 58,
    ID_TEXT_ROTATION_Z    = wxID_HIGHEST + 59,
    ID_TEXT_SCALING_X     = wxID_HIGHEST + 60,
    ID_TEXT_SCALING_Y     = wxID_HIGHEST + 61,
    ID_TEXT_SCALING_Z     = wxID_HIGHEST + 62,
    ID_CHECKBOX_ICP_BASE  = wxID_HIGHEST + 63,
    ID_CHECKBOX_WIREFRAME = wxID_HIGHEST + 64,
};

#endif
