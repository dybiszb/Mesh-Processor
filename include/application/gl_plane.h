//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef PIMP_GL_PLANE_H
#define PIMP_GL_PLANE_H

#include <GL/glew.h>
#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <GL/gl.h>
#include <iostream>
#include <ctime>
#include "rendering/gl_box.h"
#include "rendering/gl_shader_program.h"
#include "rendering/gl_camera.h"
#include "rendering/gl_eigen.h"
#include "application/input.h"
#include "rendering/gl_ply_model.h"

using namespace Eigen;
using namespace std;

class glPlane : public wxGLCanvas {
    wxGLContext *m_context;

public:
    bool glReady;
    glShaderProgram *mainShader;
    glBox *box;
    glPlyModel* mesh;

    Matrix4f projection;

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
