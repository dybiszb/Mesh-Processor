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
#include <GL/glu.h>
#include <GL/gl.h>

#include "rendering/shader_program.h"

class GLPlane : public wxGLCanvas {
    wxGLContext *m_context;

public:
    bool glReady;
    ShaderProgram* mainShader;

    GLPlane(wxFrame *parent, int *args);

    virtual ~GLPlane();

    /**
     * Initialize OpenGL context.
     */
    void initializeGLEW();

    void resized(wxSizeEvent &evt);

    int getWidth();

    int getHeight();

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
