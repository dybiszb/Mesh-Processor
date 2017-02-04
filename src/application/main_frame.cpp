#include "application/main_frame.h"

//------------------------------------------------------------------------------
MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {

    /* ----- Initialize OpenGL Plane ----- */
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    glPane = unique_ptr<glPlane>(new glPlane((wxFrame *) this, args));

    /* ----- Initialize Left Panel ----- */
    leftPanel = unique_ptr<wxBoxSizer>(new wxBoxSizer(wxVERTICAL));
    loadDeletebuttonsSizer = unique_ptr<wxBoxSizer>(new wxBoxSizer
                                                            (wxHORIZONTAL));
    loadMeshButton = unique_ptr<wxButton>(
            new wxButton(this, ID_BUTTON_LOAD_MESH,
                         wxT("      Load Mesh      ")));
    deleteMeshButton = unique_ptr<wxButton>(
            new wxButton(this, ID_BUTTON_DELETE_MESH,
                         wxT("      Delete Mesh      ")));
    treeCtrl = unique_ptr<wxTreeCtrl>(new wxTreeCtrl(this, wxID_ANY,
                                                     wxDefaultPosition,
                                                     wxSize(250, 200)));


    treeCtrl->AddRoot("Meshes");

    leftPanel->Add(treeCtrl.get(), 1, wxALIGN_TOP);
    loadDeletebuttonsSizer->Add(loadMeshButton.get(), 0, wxEXPAND);
    loadDeletebuttonsSizer->Add(deleteMeshButton.get(), 0, wxEXPAND);
    leftPanel->Add(loadDeletebuttonsSizer.get(), 0, wxALIGN_TOP);
    /* ----- Initialize Layout ----- */

    sizer = unique_ptr<wxBoxSizer>(new wxBoxSizer(wxHORIZONTAL));
    sizer->Add(leftPanel.get(), 0, wxEXPAND);

    sizer->Add(glPane.get(), 1, wxEXPAND);
    this->SetSizer(sizer.get());
    this->SetAutoLayout(true);
}

void
MainFrame::loadDefaultMesh() {
    string path = "./res/models/bunny/reconstruction/bun_zipper_res4.ply";
    wxTreeItemId id = appendMeshToTree(path);
    glPane->loadMesh(path, id);
    treeCtrl->ExpandAll();
    treeCtrl->UnselectAll();
    treeCtrl->ClearFocusedItem();
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
        glPane->loadMesh(path, id);
    }
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
    return treeCtrl->AppendItem(treeCtrl->GetRootItem().GetID(), path);
}

//------------------------------------------------------------------------------
void
MainFrame::OnDeleteMesh(wxCommandEvent &event) {
    wxTreeItemId selected = treeCtrl->GetSelection();

    // Delete only if not a root
    if (!treeCtrl->ItemHasChildren(selected)) {
        glPane->deleteMesh(selected);
        treeCtrl->Delete(selected);
    }
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
                EVT_SHOW(MainFrame::OnShow)
wxEND_EVENT_TABLE()
