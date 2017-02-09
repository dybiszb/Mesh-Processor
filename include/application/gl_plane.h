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
#include "rendering/gl_box.h"
#include "rendering/gl_shader_program.h"
#include "rendering/gl_camera.h"
#include "rendering/gl_eigen.h"
#include "application/input.h"
#include "rendering/gl_ply_model.h"
#include "rendering/gl_coordinates_frame.h"
#include "logic/icp_algoriithm.h"
#include "entities/points_cloud.h"

using namespace Eigen;
using namespace std;

class glPlane : public wxGLCanvas {
    wxGLContext *m_context;
    vector<ICPResults> m_results;
public:
    map<wxTreeItemId, unique_ptr<glPlyModel>> meshes;
    bool glReady;
    glShaderProgram *mainShader;
    glPlyModel *mesh;
    vector<wxTreeItemId> tempModelsIndices;
    Matrix4f projection;
    glCoordinatesFrame *coordinates;
    // Camera Stuff
    glOrbitCamera *camera;
    Input input;
    clock_t start;

    bool dragging = false;
    GLfloat dragXOrigin, dragYOrigin;
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;


    glPlane(wxFrame *parent, int *args);

    virtual ~glPlane();

    void loadNextICPResult();

    void loadMesh(string path, wxTreeItemId id,
                  const Vector3f &translation = Vector3f(0.0, 0.0, 0.0),
                  const Matrix3f &rotation = Matrix3f::Identity());

    void setSelected(wxTreeItemId id, bool isSelected);

    void deleteMesh(const wxTreeItemId &item);

    void runICP();

    /**
     * Initialize OpenGL context.
     */
    void initializeGLEW();

    void resized(wxSizeEvent &evt);

    int getWidth();

    int getHeight();

    void initializeGLContextIfNotReady();

    void render(wxPaintEvent &evt);

    void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x,
                           int bottomrigth_y);

    // events


    void mouseDown(wxMouseEvent &event);

    void mouseWheelMoved(wxMouseEvent &event);

    void mouseMoved(wxMouseEvent &event);

    void mouseReleased(wxMouseEvent &event);

    void rightClick(wxMouseEvent &event);

    void mouseLeftWindow(wxMouseEvent &event);

    void keyPressed(wxKeyEvent &event);

    void keyReleased(wxKeyEvent &event);

DECLARE_EVENT_TABLE()
};


#endif
