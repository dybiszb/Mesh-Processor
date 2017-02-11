#include "application/main_frame.h"

//------------------------------------------------------------------------------
MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {

    /* ----- Initialize OpenGL Plane ----- */
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    m_glPanel = new glPlane((wxFrame *) this, args);

    /* ----- Initialize Left Panel ----- */
    auto leftPanel = new wxBoxSizer(wxVERTICAL);
    initializeMeshTree(leftPanel);
    initializeMeshOperationButton(leftPanel);
    m_modelPanel = new ModelPanel(this, wxPoint(-1, -1));
    leftPanel->Add(m_modelPanel, 0, wxALL, 4);

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
    m_treeCtrl = new wxTreeCtrl(this, ID_MESHES_TREE_CTRL, wxDefaultPosition,
                                wxSize(250, 100));
    m_meshesRoot = m_treeCtrl->AddRoot("Meshes");
    parent->Add(m_treeCtrl, 1, wxALIGN_TOP);
}

//------------------------------------------------------------------------------
void
MainFrame::initializeMeshOperationButton(wxBoxSizer *parent) {
    auto hBox1 = new wxBoxSizer(wxHORIZONTAL);
    auto hBox2 = new wxBoxSizer(wxHORIZONTAL);

    auto loadMeshButton = new wxButton(this, ID_BUTTON_LOAD_MESH,
                                       wxT("      Load Mesh      "));
    auto deleteMeshButton = new wxButton(this, ID_BUTTON_DELETE_MESH,
                                         wxT("      Delete Mesh    "));
    auto runICPButton = new wxButton(this, ID_BUTTON_RUN_ICP,
                                     wxT("           Run ICP        "));
    m_nextICPFrame = new wxButton(this, ID_BUTTON_NEXT_FRAME,
                                  wxT("        Next Frame    "));
    m_nextICPFrame->Enable(false);
    hBox1->Add(loadMeshButton, 0, wxEXPAND);
    hBox1->Add(deleteMeshButton, 0, wxEXPAND);
    hBox2->Add(runICPButton, 0, wxEXPAND);
    hBox2->Add(m_nextICPFrame, 0, wxEXPAND);

    parent->Add(hBox1, 0, wxALIGN_TOP);
    parent->Add(hBox2, 0, wxALIGN_TOP);
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


void
MainFrame::loadDefaultMesh() {
    string path = "./res/models/bunny/reconstruction/bun_zipper_res4.ply";

    wxTreeItemId id1 = appendMeshToTree(path);
    m_glPanel->loadMesh(path, id1);
    wxTreeItemId id2 = appendMeshToTree(path);

    /* ----- Initialize shifted and rotated mesh ----- */
    Matrix3f r = rotationMatrix(3.14f / 4.0f, 0.0f, 0.0);
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
    wxTreeItemId selected = m_treeCtrl->GetSelection();

    // Delete only if not a root
    if (!m_treeCtrl->ItemHasChildren(selected)) {
        m_glPanel->deleteMesh(selected);
        m_treeCtrl->Delete(selected);
    }
}

//------------------------------------------------------------------------------
void
MainFrame::OnRunICP(wxCommandEvent &event) {
    m_glPanel->runICP();
    m_nextICPFrame->Enable(true);
}

//------------------------------------------------------------------------------
void
MainFrame::OnMeshesTreeItemClicked(wxTreeEvent &event) {
    wxTreeItemId selected = m_treeCtrl->GetSelection();

    // Act only if not root
    if (selected.GetID() != m_meshesRoot.GetID()) {
        m_glPanel->setSelected(selected, true);
    } else {
        m_glPanel->unselectAll();
    }
}

//------------------------------------------------------------------------------
void
MainFrame::OnNextFrame(wxCommandEvent &event) {
    m_glPanel->loadNextICPResult();
}

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
            m_modelPanel->setSelectedText(m_glPanel->getSingleSelection());
            Vector3f translation = m_glPanel->getCurrentlySelectedTranslation();
            cout << "translation: " << translation << endl;
            m_modelPanel->setTranslationText(translation(0),
                                             translation(1),
                                             translation(2));
        } else {
            // Dim the model panel
        }
    }
}

//------------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON (ID_BUTTON_LOAD_MESH, MainFrame::OnLoadMesh)
                EVT_BUTTON (ID_BUTTON_DELETE_MESH, MainFrame::OnDeleteMesh)
                EVT_BUTTON (ID_BUTTON_RUN_ICP, MainFrame::OnRunICP)
                EVT_BUTTON (ID_BUTTON_NEXT_FRAME, MainFrame::OnNextFrame)
                EVT_TREE_ITEM_ACTIVATED(ID_MESHES_TREE_CTRL,
                                        MainFrame::OnMeshesTreeItemClicked)
                EVT_TREE_ITEM_RIGHT_CLICK(ID_MESHES_TREE_CTRL,
                                          MainFrame::OnMeshesTreeItemClicked)
                EVT_SHOW(MainFrame::OnShow)
                EVT_IDLE(MainFrame::OnIdleWindow)
wxEND_EVENT_TABLE()
