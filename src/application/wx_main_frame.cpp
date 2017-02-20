#include "application/wx_main_frame.h"

//------------------------------------------------------------------------------
MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size),
          __m_meshesTitlePath("res/tex/loaded_meshes.png"),
          __m_icpTitlePath("res/tex/icp_algorithm.png"),
          __m_meshOptionsTitlePath("res/tex/mesh_options.png") {

    /* ----- Initialize OpenGL Plane ----- */
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    __m_glPanel = new glPlane((wxFrame *) this, args);
    this->SetBackgroundColour(wxColour(51, 51, 51));
    /* ----- Initialize Left Panel ----- */
    auto leftPanel = new wxBoxSizer(wxVERTICAL);

    __InitializeMeshTree(leftPanel);
    __InitializeICPBox(leftPanel);
    __InitializeMeshOptionsBox(leftPanel);

    /* ----- Initialize Layout ----- */
    auto vBox = new wxBoxSizer(wxHORIZONTAL);
    vBox->Add(leftPanel, 0, wxEXPAND);
    vBox->Add(__m_glPanel, 1, wxEXPAND);
    this->SetSizer(vBox);
    this->SetAutoLayout(true);
}

//------------------------------------------------------------------------------
void
MainFrame::__InitializeMeshTree(wxBoxSizer *parent) {
    // Title
    __m_meshesTitleBitmap = new wxStaticBitmap(
            this, wxID_ANY,
            wxBitmap("res/tex/loaded_meshes.png", wxBITMAP_TYPE_PNG),
            wxPoint(0, 0), wxSize(200, 30));
    __m_meshesTitleBitmap->SetBackgroundColour(this->GetBackgroundColour());
    parent->Add(__m_meshesTitleBitmap, 0, wxALIGN_CENTER_HORIZONTAL);

    // Tree
    __m_treeCtrl = new wxTreeCtrl(this, ID_MESHES_TREE_CTRL, wxDefaultPosition,
                                wxSize(240, 50));
    __m_treeCtrl->SetBackgroundColour(wxColour(255, 255, 255));
    __m_meshesRoot = __m_treeCtrl->AddRoot("Meshes");
    parent->Add(__m_treeCtrl, 1, wxALL, 4);

    // Buttons
    auto hBox1 = new wxBoxSizer(wxHORIZONTAL);
    auto loadMeshButton = new wxButton(this, ID_BUTTON_LOAD_MESH,
                                       wxT("      Load Mesh      "), wxPoint
                                               (-10, 10), wxSize(121, -1));
    auto deleteMeshButton = new wxButton(this, ID_BUTTON_DELETE_MESH,
                                         wxT("      Delete Mesh    "), wxPoint
                                                 (-10, 10), wxSize(121, -1));
    hBox1->Add(loadMeshButton, 0, wxEXPAND);
    hBox1->Add(deleteMeshButton, 0, wxEXPAND);
    parent->Add(hBox1, 0, wxALIGN_CENTER_HORIZONTAL, 4);
}

//------------------------------------------------------------------------------
void
MainFrame::__InitializeICPBox(wxBoxSizer *parent) {
    __m_icpTitleBitmap = new wxStaticBitmap(this, wxID_ANY,
            wxBitmap(__m_icpTitlePath, wxBITMAP_TYPE_PNG),
            wxPoint(0, 0), wxSize(200, 30));

    __m_icpTitleBitmap->SetBackgroundColour(this->GetBackgroundColour());
    parent->Add(__m_icpTitleBitmap, 0, wxALIGN_CENTER_HORIZONTAL);

    __m_icpPanel = new ICPPanel(this, wxPoint(-1, -1));
    parent->Add(__m_icpPanel, 0, wxALL, 4);

}

//------------------------------------------------------------------------------
void
MainFrame::__InitializeMeshOptionsBox(wxBoxSizer *parent) {
    __m_meshOptionsBitmap = new wxStaticBitmap(this, wxID_ANY,
            wxBitmap(__m_meshesTitlePath, wxBITMAP_TYPE_PNG),
            wxPoint(0, 0), wxSize(200, 30));

    __m_meshOptionsBitmap->SetBackgroundColour(this->GetBackgroundColour());
    parent->Add(__m_meshOptionsBitmap, 0, wxALIGN_CENTER_HORIZONTAL);

    __m_modelPanel = new ModelPanel(this, wxPoint(-1, -1));
    parent->Add(__m_modelPanel, 0, wxALL, 4);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnLoadMesh(wxCommandEvent &event) {
    wxFileDialog *OpenDialog = new wxFileDialog(this,
                                                _("Choose a file to open"),
                                                wxEmptyString, wxEmptyString,
                                                _("PLY files (*.ply)|*.ply"),
                                                wxFD_OPEN, wxDefaultPosition);

    if (OpenDialog->ShowModal() == wxID_OK) {
        string path = OpenDialog->GetPath().ToStdString();
        wxTreeItemId id = __AppendMeshToTree(path);
        __m_glPanel->loadMesh(path, id);
    }
}

//------------------------------------------------------------------------------
void
MainFrame::__LoadDefaultMesh() {
    string path = "./res/models/bunny/reconstruction/bun_zipper_res4.ply";

    wxTreeItemId id1 = __AppendMeshToTree(path);
    __m_glPanel->loadMesh(path, id1);
    wxTreeItemId id2 = __AppendMeshToTree(path);

    /* ----- Initialize shifted and rotated mesh ----- */
    Matrix3f r = rotationMatrix((float) 0.0, 0.0f, 0.0);
    __m_glPanel->loadMesh(path, id2, Vector3f(-0.01f, 0.00, -0.00f), r);

    // Prepare Tree Entries
    __m_treeCtrl->ExpandAll();
    __m_treeCtrl->UnselectAll();
    __m_treeCtrl->ClearFocusedItem();
}

//------------------------------------------------------------------------------
wxTreeItemId
MainFrame::__AppendMeshToTree(string path) {

    // Leave only filename
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
        path.erase(0, last_slash_idx + 1);
    }

    // Add to tree and return item's ID
    return __m_treeCtrl->AppendItem(__m_treeCtrl->GetRootItem().GetID(), path);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnDeleteMesh(wxCommandEvent &event) {
    if (__m_glPanel->isAnyModelSelected()) {
        wxTreeItemId glSelected = __m_glPanel->getCurrentlySelected();
        __m_glPanel->unselectAll();

        if (!__m_treeCtrl->ItemHasChildren(glSelected)) {
            __m_glPanel->deleteMesh(glSelected);
            __m_treeCtrl->Delete(glSelected);
        }
    }

}

//------------------------------------------------------------------------------
void
MainFrame::__OnMeshesTreeItemClicked(wxTreeEvent &event) {
    wxTreeItemId selected = __m_treeCtrl->GetSelection();

    // Act only if not a root
    if (selected.GetID() != __m_meshesRoot.GetID()) {
        __m_glPanel->setSelected(selected, true);
    } else {
        __m_glPanel->unselectAll();
    }

    // Reset ICP panel
    __m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnShow(wxShowEvent &event) {
    __LoadDefaultMesh();
}

//------------------------------------------------------------------------------
void // TODO: For the love of God - Bartek, make a setter for this : D
MainFrame::__OnIdleWindow(wxIdleEvent &event) {
    if (__m_glPanel->selectionChanged()) {
        if (__m_glPanel->isAnyModelSelected()) {
            __m_modelPanel->setActive(true);

            /* ----- Translation ----- */
            Vector3f translation = __m_glPanel->getCurrentlySelectedTranslation();
            __m_modelPanel->setTranslationText(translation(0),
                                             translation(1),
                                             translation(2));

            /* ----- Rotation ----- */
            Vector3f rotation = __m_glPanel->getCurrentlySelectedRotation();
            __m_modelPanel->setRotation(degToRad(rotation(0)),
                                      degToRad(rotation(1)),
                                      degToRad(rotation(2)));


            /* ----- Scaling ----- */
            Vector3f scaling = __m_glPanel->getCurrentlySelectedScaling();
            __m_modelPanel->setScale(scaling(0), scaling(1), scaling(2));

            /* ----- Normals ----- */
            bool showNormals = __m_glPanel->getCurrentlySelectedShowNormals();
            __m_modelPanel->setShowNormals(showNormals);

            /* ----- Normals ----- */
            bool wireframe = __m_glPanel->getCurrentlySelectedWireframe();
            __m_modelPanel->setWireframe(wireframe);

            /* ----- ICP Base ----- */
            bool icpBase = __m_glPanel->getCurrentlySelectedICPBase();
            __m_modelPanel->setICPBase(icpBase);

        } else {
            __m_modelPanel->setActive(false);
        }
        __m_glPanel->eatSelectionChangeNotification();
    }
}

//------------------------------------------------------------------------------
void
MainFrame::__OnIntroduceNoise(wxCommandEvent &event) {
    wxTreeItemId selected = __m_treeCtrl->GetSelection();
    float stdDev = __m_modelPanel->getStdDev();
    __m_glPanel->introduceNoise(selected, stdDev);
    __m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnNormalsCheckbox(wxCommandEvent &event) {
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    __m_glPanel->setCurrentlySelectedRenderNormals(source->IsChecked());
}

//------------------------------------------------------------------------------
void
MainFrame::__OnICPBaseCheckbox(wxCommandEvent &event) {
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    __m_glPanel->setCurrentlySelectedAsICPBaseMesh(source->IsChecked());
    __m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnMoveCentroidToOrigin(wxCommandEvent &event) {
    __m_glPanel->moveCurrentlySelectedCentroidToOrigin();
    __m_icpPanel->SetActive(false);
};

//------------------------------------------------------------------------------
void
MainFrame::__OnTranslationEditing(wxCommandEvent &event) {
    Vector3f translation = __m_modelPanel->getTranslation();
    __m_glPanel->setCurrentlySelectedTranslation(translation);
    __m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnRotationEditing(wxCommandEvent &event) {
    Matrix3f rotation = __m_modelPanel->getRotation();
    __m_glPanel->setCurrentlySelectedRotation(rotation);
    __m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnScalingEditing(wxCommandEvent &event) {
    Vector3f scale = __m_modelPanel->getScale();
    __m_glPanel->setCurrentlySelectedScale(scale);
    __m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnICPRun(wxCommandEvent &event) {
    wxTreeItemId baseId;
    wxTreeItemId selectedId;
    try {
        baseId = __m_glPanel->getCurrentICPBaseMeshId();
    } catch (string info) {
        wxMessageBox(info, "No base Mesh.", wxICON_INFORMATION);
        return;
    }
    try {
        selectedId = __m_glPanel->getCurrentlySelected();
    } catch (string info) {
        wxMessageBox(info, "No mesh selected.", wxICON_INFORMATION);
        return;
    }

    if (selectedId.GetID() == baseId.GetID()) {
        string info = "Base mesh for ICP is the same as selected one. Please "
                "chage it.";
        wxMessageBox(info, "The same mesh.", wxICON_INFORMATION);
        return;
    }
    try {
        const PointsCloud basePC = __m_glPanel->getMeshCloudById(baseId);
        const PointsCloud selectedPC = __m_glPanel->getMeshCloudById(selectedId);
        __m_icpPanel->RunICP(basePC, selectedPC);
    } catch (string info) {
        wxMessageBox(info, "Wrong id", wxICON_INFORMATION);
        return;
    }

}

//------------------------------------------------------------------------------
void
MainFrame::__OnICPReset(wxCommandEvent &event) {
    __m_glPanel->setCurrentlySelectedToInitialTransformations();
    __m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::__OnWireframe(wxCommandEvent &event) {
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    __m_glPanel->setCurrentlySelectedWireframe(!source->IsChecked());
}

//------------------------------------------------------------------------------
void
MainFrame::__OnPrevFrameICP(wxCommandEvent &event) {
    try {
        ICPResults prevFrame = __m_icpPanel->PrevFrame();
        prevFrame.m_t = -prevFrame.m_t;
        prevFrame.m_R = prevFrame.m_R.inverse();
        __m_glPanel->setCurrentlySelectedICPResult(prevFrame);
    } catch (string info) {
        return;
    }
}

//------------------------------------------------------------------------------
void
MainFrame::__OnNextFrameICP(wxCommandEvent &event) {
    try {
        ICPResults nextFrame = __m_icpPanel->NextFrame();
        __m_glPanel->setCurrentlySelectedICPResult(nextFrame);
    } catch (string info) {
        return;
    }
}

//------------------------------------------------------------------------------
void
MainFrame::__OnNearestNeighborsCheckbox(wxCommandEvent &event) {
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    __m_icpPanel->SetNearestNeighbors(source->IsChecked());
}

//------------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)

                /* ----- General ----- */
                EVT_SHOW(MainFrame::__OnShow)
                EVT_IDLE(MainFrame::__OnIdleWindow)

                /* ----- Loaded Meshes ----- */
                EVT_BUTTON(ID_BUTTON_LOAD_MESH,   MainFrame::__OnLoadMesh)
                EVT_BUTTON(ID_BUTTON_DELETE_MESH, MainFrame::__OnDeleteMesh)
                EVT_TREE_ITEM_ACTIVATED(ID_MESHES_TREE_CTRL, MainFrame::__OnMeshesTreeItemClicked)
                EVT_TREE_ITEM_RIGHT_CLICK(ID_MESHES_TREE_CTRL, MainFrame::__OnMeshesTreeItemClicked)

                /* ----- ICP Algorithm ----- */
                EVT_BUTTON(ID_NEXT_FRAME_ICP_BUTTON, MainFrame::__OnNextFrameICP)
                EVT_BUTTON(ID_NEXT_PREV_ICP_BUTTON, MainFrame::__OnPrevFrameICP)
                EVT_BUTTON(ID_RUN_ICP_BUTTON, MainFrame::__OnICPRun)
                EVT_BUTTON  (ID_RESET_ICP_BUTTON, MainFrame::__OnICPReset)
                EVT_CHECKBOX(ID_NEAREST_NEIGHBOURS_CHECKBOX, MainFrame::__OnNearestNeighborsCheckbox)

                /* ----- Mesh Options ----- */
                EVT_TEXT_ENTER(ID_TEXT_TRANSLATION_X, MainFrame::__OnTranslationEditing)
                EVT_TEXT_ENTER(ID_TEXT_TRANSLATION_Y, MainFrame::__OnTranslationEditing)
                EVT_TEXT_ENTER(ID_TEXT_TRANSLATION_Z, MainFrame::__OnTranslationEditing)
                EVT_TEXT_ENTER(ID_TEXT_ROTATION_X, MainFrame::__OnRotationEditing)
                EVT_TEXT_ENTER(ID_TEXT_ROTATION_Y, MainFrame::__OnRotationEditing)
                EVT_TEXT_ENTER(ID_TEXT_ROTATION_Z,MainFrame::__OnRotationEditing)
                EVT_TEXT_ENTER(ID_TEXT_SCALING_X, MainFrame::__OnScalingEditing)
                EVT_TEXT_ENTER(ID_TEXT_SCALING_Y, MainFrame::__OnScalingEditing)
                EVT_TEXT_ENTER(ID_TEXT_SCALING_Z, MainFrame::__OnScalingEditing)
                EVT_BUTTON(ID_INTRODUCE_NOISE, MainFrame::__OnIntroduceNoise)
                EVT_BUTTON(ID_MOVE_TO_ORIGIN, MainFrame::__OnMoveCentroidToOrigin)
                EVT_CHECKBOX(ID_CHECKBOX_NORMALS, MainFrame::__OnNormalsCheckbox)
                EVT_CHECKBOX(ID_CHECKBOX_WIREFRAME, MainFrame::__OnWireframe)
                EVT_CHECKBOX(ID_CHECKBOX_ICP_BASE, MainFrame::__OnICPBaseCheckbox)

wxEND_EVENT_TABLE()
