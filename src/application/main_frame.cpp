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
//    initializePlyPanel(leftPanel);

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
    m_treeCtrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
                                wxSize(250, 100));
    m_treeCtrl->AddRoot("Meshes");
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
                                         wxT("      Delete Mesh      "));
    auto runICPButton = new wxButton(this, ID_BUTTON_RUN_ICP,
                                     wxT("           Run ICP        "));

    hBox1->Add(loadMeshButton, 0, wxEXPAND);
    hBox1->Add(deleteMeshButton, 0, wxEXPAND);
    hBox2->Add(runICPButton, 0, wxEXPAND);

    parent->Add(hBox1, 0, wxALIGN_TOP);
    parent->Add(hBox2, 0, wxALIGN_TOP);
}

//------------------------------------------------------------------------------
void
MainFrame::initializePlyPanel(wxBoxSizer *parent) {
    auto plyPanel = new wxBoxSizer(wxVERTICAL);
    auto idHBox = new wxBoxSizer(wxHORIZONTAL);
    auto rotationHBox = new wxBoxSizer(wxHORIZONTAL);
    auto translationHBox = new wxBoxSizer(wxHORIZONTAL);

    /* ----- Name Box ----- */
    auto lineUp = new wxStaticLine(this,wxID_ANY,wxPoint(-10,10),wxSize(250,3),
                      wxLI_HORIZONTAL,wxStaticLineNameStr);
    auto title = new wxStaticText(this, wxID_ANY, wxT("Selected Mesh"),
                                  wxPoint(-10,10), wxSize(250, 30));
    auto lineDown = new wxStaticLine(this,wxID_ANY,wxPoint(-10,10),wxSize
                                             (250,3),
                                   wxLI_HORIZONTAL,wxStaticLineNameStr);

    /* ----- Translation Box ----- */
    auto labelID = new wxStaticText(this, wxID_ANY, wxT("ID: "),
                          wxDefaultPosition, wxSize(70, -1));
    auto textID = new wxTextCtrl(this, wxID_ANY, wxString(""),
                   wxDefaultPosition, wxSize(250, -1));

    idHBox->Add(labelID);
    translationHBox->Add(textID);


    plyPanel->Add(lineUp, 0, wxALIGN_TOP);
//    plyPanel->Add(title, 0, wxALIGN_TOP);
    plyPanel->Add(lineDown, 0, wxALIGN_TOP);

    plyPanel->Add(idHBox, 0, wxALIGN_TOP);
    plyPanel->Add(translationHBox, 0, wxALIGN_TOP);
    plyPanel->Add(rotationHBox, 0, wxALIGN_TOP);
    parent->Add(plyPanel, 0, wxALIGN_TOP);
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
    m_glPanel->loadMesh(path, id2, Vector3f(0.0, 0.0, -0.05f));

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
}

//------------------------------------------------------------------------------
void
MainFrame::OnShow(wxShowEvent &event) {
    loadDefaultMesh();
}

//------------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON (ID_BUTTON_LOAD_MESH, MainFrame::OnLoadMesh)
                EVT_BUTTON (ID_BUTTON_DELETE_MESH, MainFrame::OnDeleteMesh)
                EVT_BUTTON (ID_BUTTON_RUN_ICP, MainFrame::OnRunICP)
                EVT_SHOW(MainFrame::OnShow)
wxEND_EVENT_TABLE()
