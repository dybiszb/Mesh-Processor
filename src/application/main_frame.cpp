#include "application/main_frame.h"

//------------------------------------------------------------------------------
MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {

    /* ----- Initialize OpenGL Plane ----- */
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    glPane = new glPlane((wxFrame *) this, args);

    /* ----- Initialize Left Panel ----- */
    leftPanel = new wxBoxSizer(wxVERTICAL);
    loadMeshButton = new wxButton(this, ID_BUTTON_LOAD_MESH,
                                  wxT("      Load Mesh      "));

    treeCtrl = new wxTreeCtrl(this, wxID_ANY,
                             wxDefaultPosition, wxSize(250,200));


    wxTreeItemId rootId = treeCtrl->AddRoot("Meshes");

    leftPanel->Add(treeCtrl, 1, wxALIGN_TOP);
    leftPanel->Add(loadMeshButton, 0, wxALIGN_TOP);
    /* ----- Initialize Layout ----- */

    sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(leftPanel, 0, wxEXPAND);

    sizer->Add(glPane, 1, wxEXPAND);
    this->SetSizer(sizer);
    this->SetAutoLayout(true);
}

MainFrame::~MainFrame() {
//    delete sizer;
//    delete treeCtrl;
//    delete loadMeshButton;
//    delete glPane;
}


void
MainFrame::loadDefaultMesh() {
    string path = "./res/models/bunny/reconstruction/bun_zipper_res4.ply";
    wxTreeItemId id = appendMeshToTree(path);
    glPane->loadMesh(path, id);
    treeCtrl->ExpandAll();
    treeCtrl->UnselectAll ();
    treeCtrl->ClearFocusedItem ();
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
void
MainFrame::OnShow(wxShowEvent &event) {
    loadDefaultMesh();
}
//------------------------------------------------------------------------------
wxTreeItemId
MainFrame::appendMeshToTree(string path) {

    // Leave only filename
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
    {
        path.erase(0, last_slash_idx + 1);
    }

    // Add to tree and return item's ID
    return treeCtrl->AppendItem(treeCtrl->GetRootItem().GetID(), path);
}

//------------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON (ID_BUTTON_LOAD_MESH, MainFrame::OnLoadMesh)
                EVT_SHOW(MainFrame::OnShow)
wxEND_EVENT_TABLE()
