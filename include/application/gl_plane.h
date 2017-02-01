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
#include "rendering/gl_box.h"
#include "rendering/gl_shader_program.h"
#include "rendering/gl_camera.h"
#include "rendering/gl_eigen.h"

using namespace Eigen;
using namespace std;

class glPlane : public wxGLCanvas {
    wxGLContext *m_context;

public:
    bool glReady;
    glShaderProgram *mainShader;
    glBox *box;
    Matrix4f projection;
    glOrbitCamera* camera;

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
    void mouseMoved(wxMouseEvent &event);

    void mouseDown(wxMouseEvent &event);

    void mouseWheelMoved(wxMouseEvent &event);

    void mouseReleased(wxMouseEvent &event);

    void rightClick(wxMouseEvent &event);

    void mouseLeftWindow(wxMouseEvent &event);

    void keyPressed(wxKeyEvent &event);

    void keyReleased(wxKeyEvent &event);

DECLARE_EVENT_TABLE()
};


#endif
