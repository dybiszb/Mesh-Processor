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
    delete mesh;
    delete mainShader;
    delete coordinates;
    delete camera;
    delete m_context;
}

//------------------------------------------------------------------------------
void
glPlane::loadMesh(string path, wxTreeItemId id, const Vector3f& translation) {
    if (meshes.find(id) == meshes.end()) {
        tempModelsIndices.push_back(id);
        meshes[id] = unique_ptr<glPlyModel> (new glPlyModel(path, translation));
    } else {
        cout << "Warning: mesh assignment to existing id." << endl;
    }
}

//------------------------------------------------------------------------------
void // TODO: move this to main frame and from gl plane take only meshes points
glPlane::runICP() {
    // Align M2 -> M1
    wxTreeItemId m1ID = tempModelsIndices[0];
    wxTreeItemId m2ID = tempModelsIndices[1];

    vector<Vector3f> mesh1Points = (meshes[m1ID])->getVertices();
    vector<Vector3f> mesh2Points = (meshes[m2ID])->getVertices();

    ICPAlgorithm icp(mesh1Points, mesh2Points);
    vector<ICPResults> results = icp.pointToPointsICP();




//
//    vector<Vector3f> m1_vertices = (meshes[m1ID])->getVertices();
//    vector<Vector3f> m2_vertices = (meshes[m2ID])->getVertices();
//
//    Vector3f test1 = m1_vertices[0];
//    Vector3f test2 = m2_vertices[0];
//
//    Matrix3f testM = Matrix3f::Zero();
//
//    for(int i = 0; i < m1_vertices.size(); ++i) {
//
//        Vector3f v1 = m1_vertices[i];
//        Vector3f v2 = m2_vertices[i];
//        testM += v1 * v2.transpose();
//
//    }
//
//    cout << testM << endl;

//    JacobiSVD<MatrixXd> svdOfA(A, ComputeThinU | ComputeThinV);
//    Matrix3d R = svdOfA.matrixV() * svdOfA.matrixU().transpose();
//    cout << "New rotation:\n" << R << endl;

//    VectorXd t = bar_mesh1 - R *
}

//------------------------------------------------------------------------------
void
glPlane::deleteMesh(const wxTreeItemId& item) {
    meshes.erase(item);
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

        camera = new glOrbitCamera();
        coordinates = new glCoordinatesFrame();
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
    prepare3DViewport(0, 0, getWidth(), getHeight() / 2);
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
    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x,
               bottomrigth_y - topleft_y);
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

    prepare3DViewport(0, 0, getWidth(), getHeight());
    wxPaintDC(this);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render Meshes
    for(auto const& mesh : meshes) {
        (mesh.second)->render(*mainShader, camera->getViewMatrix(), projection);
    }

    // Render Coordinates
    coordinates->render(*mainShader, camera->getViewMatrix(), projection);

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
    camera->changeRadiusBy(sign * event.GetWheelDelta() * 0.001f);
}

//------------------------------------------------------------------------------
void
glPlane::mouseMoved(wxMouseEvent &event) {
    if (dragging) {
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
