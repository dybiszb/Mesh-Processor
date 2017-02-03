#include "application/gl_plane.h"

//------------------------------------------------------------------------------
glPlane::glPlane(wxFrame *parent, int *args) :
        wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize,
                   wxFULL_REPAINT_ON_RESIZE) {
    wxGLContextAttrs ctxAttrs;
    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
    m_context = new wxGLContext(this, NULL, &ctxAttrs);

    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

//------------------------------------------------------------------------------
glPlane::~glPlane() {
    delete m_context;
    delete mesh;
    delete box;
    delete mainShader;
    delete camera;
}

//------------------------------------------------------------------------------
void
glPlane::initializeGLEW() {
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
glPlane::initializeGLContextIfNotReady() {
    if (!glReady) {
        wxGLCanvas::SetCurrent(*m_context);
        initializeGLEW();

        mainShader = new glShaderProgram
                ("./res/shaders/default_shader.vert",
                 "./res/shaders/default_shader.frag");

        box = new glBox();
        mesh = new glPlyModel("./res/models/bunny/data/bun000.ply");
        // LookAt Matrix
//        Vector3f position(0.0f, 3.0f, 3.0f);
//        Vector3f target(0.0f, 0.0f, 0.0f);
//        Vector3f up(0.0f, 1.0f, 0.0f);
//        lookAt(position, target, up, view);

        camera = new glOrbitCamera();

        // Perspective Matrix
        float fovY = 45.0f;
        float aspect = 3.0f / 4.0f;
        float near = 0.0f;
        float far = 1000.0f;
        setPerspective(fovY, aspect, near, far, projection);

        glReady = true;
    }
};

//------------------------------------------------------------------------------
void
glPlane::resized(wxSizeEvent &evt) {
//	wxGLCanvas::OnSize(evt);

    Refresh();
}

//------------------------------------------------------------------------------
void
glPlane::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x,
                           int bottomrigth_y) {

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

//------------------------------------------------------------------------------
int
glPlane::getWidth() {
    return GetSize().x;
}

//------------------------------------------------------------------------------
int
glPlane::getHeight() {
    return GetSize().y;
}

//------------------------------------------------------------------------------
void
glPlane::render(wxPaintEvent &evt) {
    if (!IsShown()) return;
    initializeGLContextIfNotReady();

    prepare3DViewport(getWidth() / 2, 0, getWidth(), getHeight());
    wxPaintDC(this);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    box->render(*mainShader, camera->getViewMatrix(), projection);

    glFlush();
    SwapBuffers();
    Refresh();
}

//------------------------------------------------------------------------------
void
glPlane::mouseDown(wxMouseEvent &event) {
    dragging = true;
    dragXOrigin = event.GetX();
    dragYOrigin = event.GetY();
}

//------------------------------------------------------------------------------
void
glPlane::mouseReleased(wxMouseEvent &event) {
    dragging = false;
}

//------------------------------------------------------------------------------
void
glPlane::mouseWheelMoved(wxMouseEvent &event) {
    float sign = event.GetWheelRotation() / abs(event.GetWheelRotation());
    camera->changeRadiusBy(sign * event.GetWheelDelta() * 0.01f);
}

//------------------------------------------------------------------------------
void
glPlane::mouseMoved(wxMouseEvent &event) {
    if(dragging) {
        float eventX = event.GetX();
        float eventY = event.GetY();
        float rotationAngleX = (eventX - dragXOrigin) * 0.3f;
        float rotationAngleY = (dragYOrigin - eventY) * 0.3f;
        camera->rotateX(rotationAngleX);
        camera->rotateY(rotationAngleY);
        dragXOrigin = eventX;
        dragYOrigin = eventY;
    }
}

//------------------------------------------------------------------------------
void
glPlane::rightClick(wxMouseEvent &event) {

}

//------------------------------------------------------------------------------
void
glPlane::mouseLeftWindow(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
glPlane::keyPressed(wxKeyEvent &event) {}

//------------------------------------------------------------------------------
void
glPlane::keyReleased(wxKeyEvent &event) {}

//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(glPlane, wxGLCanvas)
                EVT_MOTION(glPlane::mouseMoved)
                EVT_LEFT_DOWN(glPlane::mouseDown)
                EVT_LEFT_UP(glPlane::mouseReleased)
                EVT_RIGHT_DOWN(glPlane::rightClick)
                EVT_LEAVE_WINDOW(glPlane::mouseLeftWindow)
                EVT_SIZE(glPlane::resized)
                EVT_KEY_DOWN(glPlane::keyPressed)
                EVT_KEY_UP(glPlane::keyReleased)
                EVT_MOUSEWHEEL(glPlane::mouseWheelMoved)
                EVT_PAINT(glPlane::render)
END_EVENT_TABLE()
