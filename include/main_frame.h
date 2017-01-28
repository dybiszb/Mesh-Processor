#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include "main_app_config.h"
#include "gl_plane.h"
#include <iostream>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxButton* loadMeshButton;
    GLPlane* glPane;
    wxBoxSizer* sizer;

    void OnLoadMesh(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_BUTTON_LOAD_MESH = wxID_HIGHEST + 1
};



#endif
