//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include "gl_plane.h"
#include <iostream>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxButton* loadMeshButton;
    glPlane* glPane;
    wxBoxSizer* sizer;

    /**
     * Opens file dialog in order to pick a mesh that should be loaded.
     *
     * @param event wxWidgets event.
     */
    void OnLoadMesh(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_BUTTON_LOAD_MESH = wxID_HIGHEST + 1
};



#endif
