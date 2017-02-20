//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef PIMP_GL_PLANE_H
#define PIMP_GL_PLANE_H

#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <GL/glew.h>
#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <GL/gl.h>
#include <wx/treectrl.h>
#include <iostream>
#include <ctime>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include "logic/typedefs.h"
#include "rendering/gl_shader_program.h"
#include "rendering/gl_camera.h"
#include "rendering/gl_eigen.h"
#include "rendering/gl_ply_model.h"
#include "rendering/gl_coordinates_frame.h"
#include "logic/icp_algoriithm.h"
#include "entities/points_cloud.h"

using namespace Application;

class glPlane : public wxGLCanvas {
    wxGLContext *m_context;
    vector<ICPResults> m_results;
public:
    glPlane(wxFrame *parent, int *args);

    Matrix4f projection;
    glCoordinatesFrame *coordinates;
    // Camera Stuff
    glOrbitCamera *camera;
    clock_t start;

    bool dragging = false;
    GLfloat dragXOrigin, dragYOrigin;
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    bool m_selectionChanged;


    const PointsCloud& getMeshCloudById(const wxTreeItemId& id);
    virtual ~glPlane();

    void loadICPResult(const wxTreeItemId& id, const ICPResults& result);

    void loadMesh(string path, wxTreeItemId id,
                  const Vector3f &translation = Vector3f(0.0, 0.0, 0.0),
                  const Matrix3f &rotation = Matrix3f::Identity());

    void setSelected(const wxTreeItemId &id, bool isSelected);

    wxTreeItemId getCurrentlySelected();

    void unselectAll();

    void deleteMesh(const wxTreeItemId &item);

    /**
     * @return True if model is selected/highlighted in the viewer, false
     * otherwise.
     */
    bool IsAnyModelSelected();

    /**
     * Method should be used coupled with EatSelectionChangeNotification
     * procedure.
     *
     * @return True if model selection changed since
     *         EatSelectionChangeNotification method was called, false otherwise.
     */
    bool SelectionChanged();


    void EatSelectionChangeNotification();

    void setRenderNormals(const wxTreeItemId &item, bool renderNormals);

    void setCurrentlySelectedRenderNormals(bool renderNormals);
    void setCurrentlySelectedAsICPBaseMesh(bool icpBase);
    void setCurrentlySelectedWireframe(bool wireframe);
    void setCurrentlySelectedToInitialTransformations();
    void setCurrentlySelectedICPResult(const ICPResults& result);

    void introduceNoise(const wxTreeItemId &item, const float stdDev);

    void moveCurrentlySelectedCentroidToOrigin();

    void setCurrentlySelectedTranslation(const Vector3f& translation);
    void setCurrentlySelectedRotation(const Matrix3f& rotation);
    void setCurrentlySelectedScale(const Vector3f& scale);

    vec3 getCurrentlySelectedTranslation();
    vec3 getCurrentlySelectedRotation();
    vec3 getCurrentlySelectedScaling();
    bool getCurrentlySelectedShowNormals();
    bool getCurrentlySelectedWireframe();
    bool getCurrentlySelectedICPBase();
    wxTreeItemId getCurrentICPBaseMeshId();

private:
    map<wxTreeItemId, unique_ptr<glPlyModel>> __m_meshes;
    bool __m_glReady;
    glShaderProgram *__m_mainShader;
    glShaderProgram *__m_normalsVisualizationShader;
    wxTreeItemId __m_currentlySelectedId = NULL;
    wxTreeItemId __m_currentICPBaseId = NULL;

    /* ----- OpenGL Context Initialization ----- */
    void __InitializeGLEW();
    void __InitializeGLContextIfNotReady();

    void __Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x,
                             int bottomrigth_y);

    /* ----- Event Handlers: Paint ----- */
    void __OnRender(wxPaintEvent &evt);
    void __OnResized(wxSizeEvent &evt);

    /* ----- Event Handlers: I/O ----- */
    void __OnMouseDown(wxMouseEvent &event);
    void __OnMouseWheelMoved(wxMouseEvent &event);
    void __OnMouseMoved(wxMouseEvent &event);
    void __OnMouseReleased(wxMouseEvent &event);
    void __OnMouseRightClick(wxMouseEvent &event);
    void __OnMouseLeftWindow(wxMouseEvent &event);
    void __OnKeyPressed(wxKeyEvent &event);
    void __OnKeyReleased(wxKeyEvent &event);

DECLARE_EVENT_TABLE()
};


#endif
