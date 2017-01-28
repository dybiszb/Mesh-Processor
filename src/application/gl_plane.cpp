#include "application/gl_plane.h"

//------------------------------------------------------------------------------
GLPlane::GLPlane(wxFrame *parent, int *args) :
        wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize,
                   wxFULL_REPAINT_ON_RESIZE) {
    wxGLContextAttrs ctxAttrs;
    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
    m_context = new wxGLContext(this, NULL, &ctxAttrs);

    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

//------------------------------------------------------------------------------
GLPlane::~GLPlane() {
    delete m_context;
    delete mainShader;
}

//------------------------------------------------------------------------------
void
GLPlane::initializeGLEW() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        wxMessageBox(
                wxString("GLEW Error: ") +
                wxString(glewGetErrorString(err)),
                _("OpenGl ERROR"),
                wxOK | wxICON_EXCLAMATION
        );
        exit(4001);
    }
}

//------------------------------------------------------------------------------
void
GLPlane::resized(wxSizeEvent &evt) {
//	wxGLCanvas::OnSize(evt);

    Refresh();
}

//------------------------------------------------------------------------------
void
GLPlane::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x,
                           int bottomrigth_y) {

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

//------------------------------------------------------------------------------
int
GLPlane::getWidth() {
    return GetSize().x;
}

//------------------------------------------------------------------------------
int
GLPlane::getHeight() {
    return GetSize().y;
}

//------------------------------------------------------------------------------
void
GLPlane::render(wxPaintEvent &evt) {
    if (!IsShown()) return;

    // Temporally in this form
    if(!glReady) {

        wxGLCanvas::SetCurrent(*m_context);
        initializeGLEW();

        mainShader = new ShaderProgram
                ("./res/shaders/default_shader.vert",
                 "./res/shaders/default_shader.frag");

        glReady = true;
    }

    prepare3DViewport(getWidth() / 2, 0, getWidth(), getHeight());



    wxPaintDC(
            this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
    SwapBuffers();
}

//------------------------------------------------------------------------------
void
GLPlane::mouseMoved(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
GLPlane::mouseDown(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
GLPlane::mouseWheelMoved(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
GLPlane::mouseReleased(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
GLPlane::rightClick(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
GLPlane::mouseLeftWindow(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
GLPlane::keyPressed(wxKeyEvent &event) {}

//------------------------------------------------------------------------------
void
GLPlane::keyReleased(wxKeyEvent &event) {}

//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(GLPlane, wxGLCanvas)
                EVT_MOTION(GLPlane::mouseMoved)
                EVT_LEFT_DOWN(GLPlane::mouseDown)
                EVT_LEFT_UP(GLPlane::mouseReleased)
                EVT_RIGHT_DOWN(GLPlane::rightClick)
                EVT_LEAVE_WINDOW(GLPlane::mouseLeftWindow)
                EVT_SIZE(GLPlane::resized)
                EVT_KEY_DOWN(GLPlane::keyPressed)
                EVT_KEY_UP(GLPlane::keyReleased)
                EVT_MOUSEWHEEL(GLPlane::mouseWheelMoved)
                EVT_PAINT(GLPlane::render)
END_EVENT_TABLE()
