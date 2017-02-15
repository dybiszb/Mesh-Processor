#include "application/wx_gl_plane.h"

//------------------------------------------------------------------------------
glPlane::glPlane(wxFrame *parent, int *args) : m_selectionChanged(true),
                                               wxGLCanvas(parent, wxID_ANY,
                                                          args,
                                                          wxDefaultPosition,
                                                          wxDefaultSize,
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
    delete __m_normalsVisualizationShader;
    delete coordinates;
    delete camera;
    delete m_context;
}

//------------------------------------------------------------------------------
void
glPlane::loadMesh(string path,
                  wxTreeItemId id,
                  const Vector3f &translation,
                  const Matrix3f &rotation) {
    if (meshes.find(id.GetID()) == meshes.end()) {
        tempModelsIndices.push_back(id.GetID());
        meshes[id.GetID()] = unique_ptr
                <glPlyModel>(new glPlyModel(path, translation, rotation));
    } else {
        cout << "Warning: mesh assignment to existing id." << endl;
    }
}

//------------------------------------------------------------------------------
void
glPlane::setSelected(const wxTreeItemId &id, bool isSelected) {
    if (meshes.count(id.GetID()) != 0) {
        if (m_currentlySelectedId != NULL) {
            (meshes[m_currentlySelectedId.GetID()])->setSelected(!isSelected);
        }
        (meshes[id.GetID()])->setSelected(isSelected);
        m_currentlySelectedId = id;
        m_selectionChanged = true;
    } else {
        cout << "Warning: Cannot find mesh id." << endl;
    }
}

//------------------------------------------------------------------------------
wxTreeItemId
glPlane::getCurrentlySelected() {
    return m_currentlySelectedId;
}

//------------------------------------------------------------------------------
void
glPlane::unselectAll() {
    if (m_currentlySelectedId != NULL) {
        (meshes[m_currentlySelectedId.GetID()])->setSelected(false);
    }
    m_currentlySelectedId = NULL;
    m_selectionChanged = true;
}

// TODO: DELETE
void glPlane::loadNextICPResult() {
    if (m_results.size() < 1) {
        cout << "out of frames\n";
        return;
    }
    ICPResults res = m_results.back();
    wxTreeItemId m2ID = tempModelsIndices[1];
    (meshes[m2ID])->m_pointsCloud->accumulateRotation(res.m_R);
    (meshes[m2ID])->m_pointsCloud->accumulateTranslation(res.m_t);

    m_results.pop_back();
}

//------------------------------------------------------------------------------
void // TODO: move this to main frame and from gl plane take only meshes points
glPlane::runICP() {
    // Align M2 -> M1
    wxTreeItemId m1ID = tempModelsIndices[0];
    wxTreeItemId m2ID = tempModelsIndices[1];

    PointsCloud m1_pc = *meshes[m1ID]->m_pointsCloud;
    PointsCloud m2_pc = *meshes[m2ID]->m_pointsCloud;

    ICPAlgorithm icp;
    m_results = icp.pointToPointsICP(m1_pc, m2_pc);
}


//------------------------------------------------------------------------------
wxTreeItemId
glPlane::getCurrentICPBaseMeshId() {
    // For all meshes check if any is a base and return
    for(const auto& pair: meshes) {
        if(pair.second->getICPBase()) {
            return pair.first;
        }
    }

    // If not - throw error info
    string errorInfo = "No mesh selected as a base for ICP Algorithm. Please "
            "select a mesh and in 'Mesh Options' tick box that says 'Base for"
            " ICP'.";
    throw errorInfo;
}


//------------------------------------------------------------------------------
void
glPlane::deleteMesh(const wxTreeItemId &item) {
    meshes.erase(item);
}

//------------------------------------------------------------------------------
void
glPlane::setRenderNormals(const wxTreeItemId &item, bool renderNormals) {
    if (meshes.count(item.GetID()) != 0) {
        (meshes[item.GetID()])->setRenderNormals(renderNormals);
    } else {
        cout << "Warning: Cannot find mesh id." << endl;
    }
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedRenderNormals(bool renderNormals) {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->setRenderNormals(renderNormals);
}

//------------------------------------------------------------------------------
void  // TODO: rewrite or split <== too complicated
glPlane::setCurrentlySelectedAsICPBaseMesh(bool icpBase) {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();

        // Inform current base that it is no longer a base
        if(m_currentICPBaseId != NULL) {
            glPlyModel *currentICPBase
                    = (meshes[m_currentICPBaseId.GetID()]).get();
            currentICPBase->setICPBase(false);
        }

        // Store new base id if requested
        if(icpBase) {
            m_currentICPBaseId = m_currentlySelectedId;
        }
    } else {
        string errorInfo = "No mesh currently selected.";
        throw errorInfo;
    };

    if(icpBase) {
        selectedModel->setICPBase(icpBase);
    }

}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedWireframe(bool wireframe) {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->setWireframe(wireframe);
}

//------------------------------------------------------------------------------
void
glPlane::introduceNoise(const wxTreeItemId &item, const float stdDev) {
    if (meshes.count(item.GetID()) != 0) {
        (meshes[item.GetID()])->introduceGaussianNoise(0.0f, stdDev);
    } else {
        cout << "Warning: Cannot find mesh id." << endl;
    }
}

//------------------------------------------------------------------------------
void
glPlane::moveCurrentlySelectedCentroidToOrigin() {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->moveCentroidToOrigin();
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedTranslation(const Vector3f& translation) {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->m_pointsCloud->setTranslation(translation);
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedRotation(const Matrix3f& rotation) {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->m_pointsCloud->setRotation(rotation);
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedScale(const Vector3f& scale) {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->m_pointsCloud->setScale(scale);
}

//------------------------------------------------------------------------------
Vector3f
glPlane::getCurrentlySelectedTranslation() {
    const glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return Vector3f(0.0, 0.0, 0.0);
    return selectedModel->m_pointsCloud->getTranslation();
}

//------------------------------------------------------------------------------
Vector3f
glPlane::getCurrentlySelectedRotation() {
    const glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return Vector3f(0.0, 0.0, 0.0);
    return selectedModel->m_pointsCloud->getRotationAngles();
}

//------------------------------------------------------------------------------
Vector3f
glPlane::getCurrentlySelectedScaling() {
    const glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else return Vector3f(0.0, 0.0, 0.0);
    return selectedModel->m_pointsCloud->getScale();
}

//------------------------------------------------------------------------------
bool
glPlane::getCurrentlySelectedShowNormals() {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else false;
    return selectedModel->getRenderNormals();
}

//------------------------------------------------------------------------------
bool
glPlane::getCurrentlySelectedWireframe() {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else false;
    return selectedModel->getWireframe();
}

//------------------------------------------------------------------------------
bool
glPlane::getCurrentlySelectedICPBase() {
    glPlyModel *selectedModel;
    if (m_currentlySelectedId != NULL) {
        selectedModel = (meshes[m_currentlySelectedId.GetID()]).get();
    } else false;
    return selectedModel->getICPBase();
}

//------------------------------------------------------------------------------
bool
glPlane::isAnyModelSelected() {
    return (m_currentlySelectedId != NULL);
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

        __m_normalsVisualizationShader = new glShaderProgram
                ("./res/shaders/normals_visualization.vert",
                 "./res/shaders/normals_visualization.frag");

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
const string
glPlane::getSingleSelection() {
    m_selectionChanged = false;
    return "test";
}

//------------------------------------------------------------------------------
bool
glPlane::selectionChanged() {
    return m_selectionChanged;
}

//------------------------------------------------------------------------------
void
glPlane::eatSelectionChangeNotification() {
    m_selectionChanged = false;
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
    // Render Coordinates
    coordinates->render(*mainShader, camera->getViewMatrix(), projection);

    // Render Meshes
    for (auto const &mesh : meshes) {
        (mesh.second)->setShading(true); // TODO: temp
        (mesh.second)->render(*mainShader,
                              camera->getViewMatrix(),
                              projection,
                              __m_normalsVisualizationShader);
    }
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
