//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "gl_plane.h"
#include <iostream>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    void loadDefaultMesh();

    void OnShow(wxShowEvent &event);

private:
    unique_ptr<wxButton> loadMeshButton;
    unique_ptr<wxButton> deleteMeshButton;
    unique_ptr<wxButton> runICPButton;
    unique_ptr<glPlane> glPane;
    unique_ptr<wxBoxSizer> sizer;
    unique_ptr<wxBoxSizer> leftPanel;
    unique_ptr<wxBoxSizer> loadDeletebuttonsSizer;
    unique_ptr<wxBoxSizer> runStopICPSizer;
    unique_ptr<wxTreeCtrl> treeCtrl;

    /**
     *
     * @param path
     * @return
     */
    wxTreeItemId appendMeshToTree(string path);

    /**
     * Opens file dialog in order to pick a mesh that should be loaded.
     *
     * @param event wxWidgets event.
     */
    void OnLoadMesh(wxCommandEvent &event);

    /**
     *
     * @param event
     */
    void OnDeleteMesh(wxCommandEvent &event);

    /**
     *
     * @param event
     */
    void OnRunICP(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BUTTON_LOAD_MESH = wxID_HIGHEST + 1,
    ID_BUTTON_DELETE_MESH = wxID_HIGHEST + 2,
    ID_BUTTON_RUN_ICP = wxID_HIGHEST + 3,
};


#endif
