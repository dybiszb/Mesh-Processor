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
    delete __m_mainShader;
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
    if (__m_meshes.find(id.GetID()) == __m_meshes.end()) {
        __m_meshes[id.GetID()] = unique_ptr
                <glPlyModel>(new glPlyModel(path, translation, rotation));
    } else {
        cout << "Warning: mesh assignment to existing id." << endl;
    }
}

//------------------------------------------------------------------------------
void
glPlane::setSelected(const wxTreeItemId &id, bool isSelected) {
    if (__m_meshes.count(id.GetID()) != 0) {
        if (__m_currentlySelectedId != NULL) {
            (__m_meshes[__m_currentlySelectedId.GetID()])->setSelected(!isSelected);
        }
        (__m_meshes[id.GetID()])->setSelected(isSelected);
        __m_currentlySelectedId = id;
        m_selectionChanged = true;
    } else {
        cout << "Warning: Cannot find mesh id." << endl;
    }
}

//------------------------------------------------------------------------------
wxTreeItemId
glPlane::getCurrentlySelected() {
    if(!__m_currentlySelectedId) {
        string errorInfo = "No mesh selected. Please right click on one of "
                "the mesh in the tree view. Please do not select the one that"
                " was selected as a base for ICP algorithm.";
        throw errorInfo;
    }
    return __m_currentlySelectedId;
}

//------------------------------------------------------------------------------
void
glPlane::unselectAll() {
    if (__m_currentlySelectedId != NULL) {
        (__m_meshes[__m_currentlySelectedId.GetID()])->setSelected(false);
    }
    __m_currentlySelectedId = NULL;
    m_selectionChanged = true;
}

void glPlane::loadICPResult(const wxTreeItemId& id, const ICPResults& result) {
    (__m_meshes[id.GetID()])->m_pointsCloud->accumulateRotation(result.m_R);
    (__m_meshes[id.GetID()])->m_pointsCloud->accumulateTranslation(result.m_t);
}
//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedToInitialTransformations() {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->resetToInitialTransform();
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedICPResult(const ICPResults& result) {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->m_pointsCloud->accumulateRotation(result.m_R);
    selectedModel->m_pointsCloud->accumulateTranslation(result.m_t);
}


//------------------------------------------------------------------------------
wxTreeItemId
glPlane::getCurrentICPBaseMeshId() {
    // For all meshes check if any is a base and return
    for(const auto& pair: __m_meshes) {
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
    __m_meshes.erase(item);
}

//------------------------------------------------------------------------------
void
glPlane::setRenderNormals(const wxTreeItemId &item, bool renderNormals) {
    if (__m_meshes.count(item.GetID()) != 0) {
        (__m_meshes[item.GetID()])->setRenderNormals(renderNormals);
    } else {
        cout << "Warning: Cannot find mesh id." << endl;
    }
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedRenderNormals(bool renderNormals) {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->setRenderNormals(renderNormals);
}

//------------------------------------------------------------------------------
void  // TODO: rewrite or split <== too complicated
glPlane::setCurrentlySelectedAsICPBaseMesh(bool icpBase) {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();

        // Inform current base that it is no longer a base
        if(__m_currentICPBaseId != NULL) {
            glPlyModel *currentICPBase
                    = (__m_meshes[__m_currentICPBaseId.GetID()]).get();
            currentICPBase->setICPBase(false);
        }

        // Store new base id if requested
        if(icpBase) {
            __m_currentICPBaseId = __m_currentlySelectedId;
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
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->setWireframe(wireframe);
}

//------------------------------------------------------------------------------
void
glPlane::introduceNoise(const wxTreeItemId &item, const float stdDev) {
    if (__m_meshes.count(item.GetID()) != 0) {
        (__m_meshes[item.GetID()])->introduceGaussianNoise(0.0f, stdDev);
    } else {
        cout << "Warning: Cannot find mesh id." << endl;
    }
}

//------------------------------------------------------------------------------
void
glPlane::moveCurrentlySelectedCentroidToOrigin() {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->moveCentroidToOrigin();
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedTranslation(const Vector3f& translation) {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->m_pointsCloud->setTranslation(translation);
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedRotation(const Matrix3f& rotation) {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->m_pointsCloud->setRotation(rotation);
}

//------------------------------------------------------------------------------
void
glPlane::setCurrentlySelectedScale(const Vector3f& scale) {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return;
    selectedModel->m_pointsCloud->setScale(scale);
}

//------------------------------------------------------------------------------
Vector3f
glPlane::getCurrentlySelectedTranslation() {
    const glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return Vector3f(0.0, 0.0, 0.0);
    return selectedModel->m_pointsCloud->getTranslation();
}

//------------------------------------------------------------------------------
Vector3f
glPlane::getCurrentlySelectedRotation() {
    const glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return Vector3f(0.0, 0.0, 0.0);
    return selectedModel->m_pointsCloud->getRotationAngles();
}

//------------------------------------------------------------------------------
Vector3f
glPlane::getCurrentlySelectedScaling() {
    const glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else return Vector3f(0.0, 0.0, 0.0);
    return selectedModel->m_pointsCloud->getScale();
}

//------------------------------------------------------------------------------
bool
glPlane::getCurrentlySelectedShowNormals() {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else false;
    return selectedModel->getRenderNormals();
}

//------------------------------------------------------------------------------
bool
glPlane::getCurrentlySelectedWireframe() {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else false;
    return selectedModel->getWireframe();
}

//------------------------------------------------------------------------------
bool
glPlane::getCurrentlySelectedICPBase() {
    glPlyModel *selectedModel;
    if (__m_currentlySelectedId != NULL) {
        selectedModel = (__m_meshes[__m_currentlySelectedId.GetID()]).get();
    } else false;
    return selectedModel->getICPBase();
}

//------------------------------------------------------------------------------
bool
glPlane::IsAnyModelSelected() {
    return (__m_currentlySelectedId != NULL);
}

//------------------------------------------------------------------------------
const PointsCloud&
glPlane::getMeshCloudById(const wxTreeItemId& id) {
    if (__m_meshes.count(id.GetID()) != 0) {
        return *(__m_meshes[id.GetID()])->m_pointsCloud.get();
    } else {
        string errorInfo = "No mesh with provided id found.";
        throw errorInfo;
    }
}

//------------------------------------------------------------------------------
void
glPlane::__InitializeGLEW() {
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
glPlane::__InitializeGLContextIfNotReady() {
    if (!__m_glReady) {
        wxGLCanvas::SetCurrent(*m_context);
        __InitializeGLEW();

        __m_mainShader = new glShaderProgram
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

        __m_glReady = true;
    }
};

//------------------------------------------------------------------------------
void
glPlane::__OnResized(wxSizeEvent &evt) {
//	wxGLCanvas::OnSize(evt);
    __Prepare3DViewport(0, 0,  GetSize().x,  GetSize().y);
    Refresh();
}

//------------------------------------------------------------------------------
bool
glPlane::SelectionChanged() {
    return m_selectionChanged;
}

//------------------------------------------------------------------------------
void
glPlane::EatSelectionChangeNotification() {
    m_selectionChanged = false;
}

//------------------------------------------------------------------------------
void
glPlane::__Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x,
                             int bottomrigth_y) {

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x,
               bottomrigth_y - topleft_y);
}

//------------------------------------------------------------------------------
void
glPlane::__OnRender(wxPaintEvent &evt) {
    if (!IsShown()) return;
    __InitializeGLContextIfNotReady();

    __Prepare3DViewport(0, 0, GetSize().x, GetSize().y);
    wxPaintDC(this);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render Coordinates
    coordinates->render(*__m_mainShader, camera->getViewMatrix(), projection);

    // Render Meshes
    for (auto const &mesh : __m_meshes) {
        (mesh.second)->render(*__m_mainShader,
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
glPlane::__OnMouseDown(wxMouseEvent &event) {
    dragging = true;
    dragXOrigin = event.GetX();
    dragYOrigin = event.GetY();
}

//------------------------------------------------------------------------------
void
glPlane::__OnMouseReleased(wxMouseEvent &event) {
    dragging = false;
}

//------------------------------------------------------------------------------
void
glPlane::__OnMouseWheelMoved(wxMouseEvent &event) {
    float sign = event.GetWheelRotation() / abs(event.GetWheelRotation());
    camera->changeRadiusBy(sign * event.GetWheelDelta() * 0.0001f);
}

//------------------------------------------------------------------------------
void
glPlane::__OnMouseMoved(wxMouseEvent &event) {
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
glPlane::__OnMouseRightClick(wxMouseEvent &event) {

}

//------------------------------------------------------------------------------
void
glPlane::__OnMouseLeftWindow(wxMouseEvent &event) {}

//------------------------------------------------------------------------------
void
glPlane::__OnKeyPressed(wxKeyEvent &event) {}

//------------------------------------------------------------------------------
void
glPlane::__OnKeyReleased(wxKeyEvent &event) {}

//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(glPlane, wxGLCanvas)
                EVT_MOTION(glPlane::__OnMouseMoved)
                EVT_LEFT_DOWN(glPlane::__OnMouseDown)
                EVT_LEFT_UP(glPlane::__OnMouseReleased)
                EVT_RIGHT_DOWN(glPlane::__OnMouseRightClick)
                EVT_LEAVE_WINDOW(glPlane::__OnMouseLeftWindow)
                EVT_SIZE(glPlane::__OnResized)
                EVT_KEY_DOWN(glPlane::__OnKeyPressed)
                EVT_KEY_UP(glPlane::__OnKeyReleased)
                EVT_MOUSEWHEEL(glPlane::__OnMouseWheelMoved)
                EVT_PAINT(glPlane::__OnRender)
END_EVENT_TABLE()
