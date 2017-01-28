#include "main_frame.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {

    /* ----- Initialize OpenGL Plane ----- */
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    glPane = new GLPlane( (wxFrame*) this, args);

    /* ----- Initialize Left Panel ----- */
    loadMeshButton = new wxButton(this, ID_BUTTON_LOAD_MESH, wxT("Load Mesh"));

    /* ----- Initialize Layout ----- */
    sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(loadMeshButton, 0, wxALIGN_TOP);
    sizer->Add(glPane, 1, wxEXPAND);
    this->SetSizer(sizer);
    this->SetAutoLayout(true);
}

void MainFrame::OnLoadMesh(wxCommandEvent &event) {
    Close(true);
}
