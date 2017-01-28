#include "gl_plane.h"

void GLPlane::mouseMoved(wxMouseEvent &event) {}

void GLPlane::mouseDown(wxMouseEvent &event) {}

void GLPlane::mouseWheelMoved(wxMouseEvent &event) {}

void GLPlane::mouseReleased(wxMouseEvent &event) {}

void GLPlane::rightClick(wxMouseEvent &event) {}

void GLPlane::mouseLeftWindow(wxMouseEvent &event) {}

void GLPlane::keyPressed(wxKeyEvent &event) {}

void GLPlane::keyReleased(wxKeyEvent &event) {}

GLPlane::GLPlane(wxFrame* parent, int* args) :
        wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

GLPlane::~GLPlane()
{
    delete m_context;
}

void GLPlane::resized(wxSizeEvent& evt)
{
//	wxGLCanvas::OnSize(evt);

    Refresh();
}

void GLPlane::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{

    glClearColor(0.0f, 255.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

int GLPlane::getWidth()
{
    return GetSize().x;
}

int GLPlane::getHeight()
{
    return GetSize().y;
}


void GLPlane::render( wxPaintEvent& evt )
{
    if(!IsShown()) return;

    prepare3DViewport(getWidth()/2,0,getWidth(), getHeight());
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
    SwapBuffers();
}