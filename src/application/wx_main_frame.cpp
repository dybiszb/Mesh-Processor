#include "application/wx_main_frame.h"

//------------------------------------------------------------------------------
MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {

    /* ----- Initialize OpenGL Plane ----- */
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    m_glPanel = new glPlane((wxFrame *) this, args);
    this->SetBackgroundColour(wxColour(51, 51, 51));
    /* ----- Initialize Left Panel ----- */
    auto leftPanel = new wxBoxSizer(wxVERTICAL);

    initializeMeshTree(leftPanel);
    initializeICPBox(leftPanel);
    initializeMeshOptionsBox(leftPanel);


    /* ----- Initialize Layout ----- */
    auto vBox = new wxBoxSizer(wxHORIZONTAL);
    vBox->Add(leftPanel, 0, wxEXPAND);
    vBox->Add(m_glPanel, 1, wxEXPAND);
    this->SetSizer(vBox);
    this->SetAutoLayout(true);
}

//------------------------------------------------------------------------------
void
MainFrame::initializeMeshTree(wxBoxSizer *parent) {
    // Title
    m_meshesTitleBitmap = new wxStaticBitmap(
            this, wxID_ANY,
            wxBitmap("res/tex/loaded_meshes.png", wxBITMAP_TYPE_PNG),
            wxPoint(0, 0), wxSize(200, 30));
    m_meshesTitleBitmap->SetBackgroundColour(this->GetBackgroundColour());
    parent->Add(m_meshesTitleBitmap, 0, wxALIGN_CENTER_HORIZONTAL);

    // Tree
    m_treeCtrl = new wxTreeCtrl(this, ID_MESHES_TREE_CTRL, wxDefaultPosition,
                                wxSize(240, 50));
    m_treeCtrl->SetBackgroundColour(wxColour(255, 255, 255));
    m_meshesRoot = m_treeCtrl->AddRoot("Meshes");
    parent->Add(m_treeCtrl, 1, wxALL, 4);

    // Buttons
    auto hBox1 = new wxBoxSizer(wxHORIZONTAL);
    auto loadMeshButton = new wxButton(this, ID_BUTTON_LOAD_MESH,
                                       wxT("      Load Mesh      "),wxPoint
                                               (-10, 10), wxSize(121, -1));
    auto deleteMeshButton = new wxButton(this, ID_BUTTON_DELETE_MESH,
                                         wxT("      Delete Mesh    "),wxPoint
                                                 (-10, 10), wxSize(121, -1));
    hBox1->Add(loadMeshButton, 0, wxEXPAND);
    hBox1->Add(deleteMeshButton, 0, wxEXPAND);
    parent->Add(hBox1, 0, wxALIGN_CENTER_HORIZONTAL, 4);
}

//------------------------------------------------------------------------------
void
MainFrame::initializeICPBox(wxBoxSizer *parent) {
    auto hBox2 = new wxBoxSizer(wxHORIZONTAL);
    m_icpTitleBitmap = new wxStaticBitmap(
            this, wxID_ANY,
            wxBitmap("res/tex/icp_algorithm.png", wxBITMAP_TYPE_PNG),
            wxPoint(0, 0), wxSize(200, 30));
    m_icpTitleBitmap->SetBackgroundColour(this->GetBackgroundColour());

    parent->Add(m_icpTitleBitmap, 0, wxALIGN_CENTER_HORIZONTAL);

    m_icpPanel = new ICPPanel(this, wxPoint(-1, -1));
    parent->Add(m_icpPanel, 0, wxALL, 4);

}

//------------------------------------------------------------------------------
void
MainFrame::initializeMeshOptionsBox(wxBoxSizer *parent) {
    m_titleBitmap = new wxStaticBitmap(
            this, wxID_ANY,
            wxBitmap("res/tex/mesh_options.png", wxBITMAP_TYPE_PNG),
            wxPoint(0, 0), wxSize(200, 30));
    m_titleBitmap->SetBackgroundColour(this->GetBackgroundColour());
    parent->Add(m_titleBitmap, 0, wxALIGN_CENTER_HORIZONTAL);

    m_modelPanel = new ModelPanel(this, wxPoint(-1, -1));
    parent->Add(m_modelPanel, 0, wxALL, 4);
}

//------------------------------------------------------------------------------
void
MainFrame::OnLoadMesh(wxCommandEvent &event) {
    wxFileDialog *OpenDialog = new wxFileDialog(this,
                                                _("Choose a file to open"),
                                                wxEmptyString, wxEmptyString,
                                                _("PLY files (*.ply)|*.ply"),
                                                wxFD_OPEN, wxDefaultPosition);

    if (OpenDialog->ShowModal() == wxID_OK) {
        string path = OpenDialog->GetPath().ToStdString();
        wxTreeItemId id = appendMeshToTree(path);
        m_glPanel->loadMesh(path, id);
    }
}

//------------------------------------------------------------------------------
void
MainFrame::loadDefaultMesh() {
    string path = "./res/models/bunny/reconstruction/bun_zipper_res4.ply";

    wxTreeItemId id1 = appendMeshToTree(path);
    m_glPanel->loadMesh(path, id1);
//    m_glPanel->setRenderNormals(id1, true);
    wxTreeItemId id2 = appendMeshToTree(path);

    /* ----- Initialize shifted and rotated mesh ----- */
    Matrix3f r = rotationMatrix((float) (M_PI / 4.0f), 0.0f, 0.0);
    m_glPanel->loadMesh(path, id2, Vector3f(-0.08f, 0.00, -0.00f), r);

    // Prepare Tree Entries
    m_treeCtrl->ExpandAll();
    m_treeCtrl->UnselectAll();
    m_treeCtrl->ClearFocusedItem();
}

//------------------------------------------------------------------------------
wxTreeItemId
MainFrame::appendMeshToTree(string path) {

    // Leave only filename
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
        path.erase(0, last_slash_idx + 1);
    }

    // Add to tree and return item's ID
    return m_treeCtrl->AppendItem(m_treeCtrl->GetRootItem().GetID(), path);
}

//------------------------------------------------------------------------------
void
MainFrame::OnDeleteMesh(wxCommandEvent &event) {
    if (m_glPanel->isAnyModelSelected()) {
        wxTreeItemId glSelected = m_glPanel->getCurrentlySelected();
        m_glPanel->unselectAll();

        if (!m_treeCtrl->ItemHasChildren(glSelected)) {
            m_glPanel->deleteMesh(glSelected);
            m_treeCtrl->Delete(glSelected);
        }
    }

}

//------------------------------------------------------------------------------
//void
//MainFrame::OnRunICP(wxCommandEvent &event) {
//    m_glPanel->runICP();
////    m_nextICPFrame->Enable(true);
//}

//------------------------------------------------------------------------------
void
MainFrame::OnMeshesTreeItemClicked(wxTreeEvent &event) {
    wxTreeItemId selected = m_treeCtrl->GetSelection();

    // Act only if not a root
    if (selected.GetID() != m_meshesRoot.GetID()) {
        m_glPanel->setSelected(selected, true);
    } else {
        m_glPanel->unselectAll();
    }

    // Reset ICP panel
    m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
//void
//MainFrame::OnNextFrame(wxCommandEvent &event) {
//    m_glPanel->loadNextICPResult();
//}

//------------------------------------------------------------------------------
void
MainFrame::OnShow(wxShowEvent &event) {
    loadDefaultMesh();
}

//------------------------------------------------------------------------------
void
MainFrame::OnIdleWindow(wxIdleEvent &event) {
    if (m_glPanel->selectionChanged()) {
        if (m_glPanel->isAnyModelSelected()) {
            m_modelPanel->setActive(true);

            /* ----- Translation ----- */
            Vector3f translation = m_glPanel->getCurrentlySelectedTranslation();
            m_modelPanel->setTranslationText(translation(0),
                                             translation(1),
                                             translation(2));

            /* ----- Rotation ----- */
            Vector3f rotation = m_glPanel->getCurrentlySelectedRotation();
            m_modelPanel->setRotation((rotation(0)),
                                      (rotation(1)),
                                      (rotation(2)));


            /* ----- Scaling ----- */
            Vector3f scaling = m_glPanel->getCurrentlySelectedScaling();
            m_modelPanel->setScale(scaling(0), scaling(1), scaling(2));

            /* ----- Normals ----- */
            bool showNormals = m_glPanel->getCurrentlySelectedShowNormals();
            m_modelPanel->setShowNormals(showNormals);

            /* ----- Normals ----- */
            bool wireframe = m_glPanel->getCurrentlySelectedWireframe();
            m_modelPanel->setWireframe(wireframe);

            /* ----- ICP Base ----- */
            bool icpBase = m_glPanel->getCurrentlySelectedICPBase();
            m_modelPanel->setICPBase(icpBase);

        } else {
            m_modelPanel->setActive(false);
        }
        m_glPanel->eatSelectionChangeNotification();
    }
}

//------------------------------------------------------------------------------
void
MainFrame::OnIntroduceNoise(wxCommandEvent &event) {
    wxTreeItemId selected = m_treeCtrl->GetSelection();
    float stdDev = m_modelPanel->getStdDev();
    m_glPanel->introduceNoise(selected, stdDev);
    m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::OnNormalsCheckbox(wxCommandEvent &event) {
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    m_glPanel->setCurrentlySelectedRenderNormals(source->IsChecked());
}

//------------------------------------------------------------------------------
void
MainFrame::OnICPBaseCheckbox(wxCommandEvent& event) {
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    m_glPanel->setCurrentlySelectedAsICPBaseMesh(source->IsChecked());
    m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::OnMoveCentroidToOrigin(wxCommandEvent &event) {
    m_glPanel->moveCurrentlySelectedCentroidToOrigin();
    m_icpPanel->SetActive(false);
};

//------------------------------------------------------------------------------
void
MainFrame::OnTranslationEditing(wxCommandEvent &event) {
    Vector3f translation = m_modelPanel->getTranslation();
    m_glPanel->setCurrentlySelectedTranslation(translation);
    m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::OnRotationEditing(wxCommandEvent &event) {
    Matrix3f rotation = m_modelPanel->getRotation();
    m_glPanel->setCurrentlySelectedRotation(rotation);
    m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::OnScaleEditing(wxCommandEvent &event) {
    cout << "edit scaling\n";
    Vector3f scale = m_modelPanel->getScale();
    m_glPanel->setCurrentlySelectedScale(scale);
    m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::OnICPRun(wxCommandEvent &event) {
    wxTreeItemId baseId;
    wxTreeItemId selectedId;
    try {
        baseId = m_glPanel->getCurrentICPBaseMeshId();
    }catch (string info) {
        wxMessageBox(info, "No base Mesh.", wxICON_INFORMATION);
        return;
    }
    try {
        selectedId = m_glPanel->getCurrentlySelected();
    } catch(string info) {
        wxMessageBox(info, "No mesh selected.", wxICON_INFORMATION);
        return;
    }

    if(selectedId.GetID() == baseId.GetID()) {
        string info = "Base mesh for ICP is the same as selected one. Please "
                "chage it.";
        wxMessageBox(info, "The same mesh.", wxICON_INFORMATION);
        return;
    }
    try {
        const PointsCloud basePC = m_glPanel->getMeshCloudById(baseId);
        const PointsCloud selectedPC = m_glPanel->getMeshCloudById(selectedId);
        m_icpPanel->RunICP(basePC, selectedPC);
    } catch (string info) {
        wxMessageBox(info, "Wrong id", wxICON_INFORMATION);
        return;
    }

}

//------------------------------------------------------------------------------
void
MainFrame::OnICPReset(wxCommandEvent &event) {
    m_icpPanel->SetActive(false);
}

//------------------------------------------------------------------------------
void
MainFrame::OnWireframe(wxCommandEvent &event) {
    cout << "click"<<endl;
    wxCheckBox *source = (wxCheckBox *) event.GetEventObject();
    m_glPanel->setCurrentlySelectedWireframe(!source->IsChecked());
}

//------------------------------------------------------------------------------
void
MainFrame::OnPrevFrameICP(wxCommandEvent &event) {
    try {
        ICPResults prevFrame = m_icpPanel->PrevFrame();
        prevFrame.m_t = - prevFrame.m_t;
        prevFrame.m_R = prevFrame.m_R.inverse();
        m_glPanel->setCurrentlySelectedICPResult(prevFrame);
    } catch(string info) {
        return;
    }
}

//------------------------------------------------------------------------------
void
MainFrame::OnNextFrameICP(wxCommandEvent &event) {
    try {
        ICPResults nextFrame = m_icpPanel->NextFrame();
        m_glPanel->setCurrentlySelectedICPResult(nextFrame);
    } catch(string info) {
        return;
    }
}

//------------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON (ID_BUTTON_LOAD_MESH, MainFrame::OnLoadMesh)
                EVT_BUTTON (ID_BUTTON_DELETE_MESH, MainFrame::OnDeleteMesh)
                EVT_BUTTON (ID_BUTTON_RUN_ICP, MainFrame::OnICPRun)
                EVT_BUTTON (ID_BUTTON_NEXT_FRAME, MainFrame::OnICPReset)
                EVT_BUTTON (ID_RUN_ICP_BUTTON, MainFrame::OnICPRun)
                EVT_BUTTON (ID_RESET_ICP_BUTTON, MainFrame::OnICPReset)

                EVT_BUTTON (ID_NEXT_FRAME_ICP_BUTTON, MainFrame::OnNextFrameICP)
                EVT_BUTTON (ID_NEXT_PREV_ICP_BUTTON, MainFrame::OnPrevFrameICP)

                // Translation
                EVT_TEXT_ENTER(ID_TEXT_TRANSLATION_X,
                               MainFrame::OnTranslationEditing)
                EVT_TEXT_ENTER(ID_TEXT_TRANSLATION_Y,
                               MainFrame::OnTranslationEditing)
                EVT_TEXT_ENTER(ID_TEXT_TRANSLATION_Z,
                               MainFrame::OnTranslationEditing)

                // Rotation
                EVT_TEXT_ENTER(ID_TEXT_ROTATION_X,
                               MainFrame::OnRotationEditing)
                EVT_TEXT_ENTER(ID_TEXT_ROTATION_Y,
                               MainFrame::OnRotationEditing)
                EVT_TEXT_ENTER(ID_TEXT_ROTATION_Z,
                               MainFrame::OnRotationEditing)

                // Scale
                EVT_TEXT_ENTER(ID_TEXT_SCALING_X,
                               MainFrame::OnScaleEditing)
                EVT_TEXT_ENTER(ID_TEXT_SCALING_Y,
                               MainFrame::OnScaleEditing)
                EVT_TEXT_ENTER(ID_TEXT_SCALING_Z,
                               MainFrame::OnScaleEditing)

                EVT_BUTTON (ID_INTRODUCE_NOISE, MainFrame::OnIntroduceNoise)
                EVT_BUTTON (ID_MOVE_TO_ORIGIN,
                            MainFrame::OnMoveCentroidToOrigin)
                EVT_CHECKBOX(ID_CHECKBOX_NORMALS, MainFrame::OnNormalsCheckbox)
                EVT_CHECKBOX(ID_CHECKBOX_ICP_BASE, MainFrame::OnICPBaseCheckbox)
                EVT_CHECKBOX(ID_CHECKBOX_WIREFRAME, MainFrame::OnWireframe)
                EVT_TREE_ITEM_ACTIVATED(ID_MESHES_TREE_CTRL,
                                        MainFrame::OnMeshesTreeItemClicked)
                EVT_TREE_ITEM_RIGHT_CLICK(ID_MESHES_TREE_CTRL,
                                          MainFrame::OnMeshesTreeItemClicked)
                EVT_SHOW(MainFrame::OnShow)
                EVT_IDLE(MainFrame::OnIdleWindow)
wxEND_EVENT_TABLE()
