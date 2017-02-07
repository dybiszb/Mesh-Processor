//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H
#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/statline.h>
#include <memory>
#include "gl_plane.h"
#include <iostream>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    void loadDefaultMesh();

    void OnShow(wxShowEvent &event);

private:
//    auto m_loadMeshButton;
//    auto m_deleteMeshButton;
//    auto m_runICPButton;
    glPlane* m_glPanel;
//    auto m_sizer;
//    auto m_leftPanel;
//    auto m_loadDeletebuttonsSizer;
//    auto m_runStopICPSizer;
    wxTreeCtrl* m_treeCtrl;
//    glPlane* m_plyPanel;
    void initializeMeshTree(wxBoxSizer* parent);

    void initializeMeshOperationButton(wxBoxSizer* parent);

    /**
     * Initializes m_plyPanel box and its members.
     */
    void initializePlyPanel(wxBoxSizer* parent);

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
    ID_TEXT_TRANSLATION_X = wxID_HIGHEST + 7,
    ID_TEXT_TRANSLATION_Y = wxID_HIGHEST + 8,
    ID_TEXT_TRANSLATION_Z = wxID_HIGHEST + 9,
    ID_TEXT_ROTATION_X = wxID_HIGHEST + 10,
    ID_TEXT_ROTATION_Y = wxID_HIGHEST + 11,
    ID_TEXT_ROTATION_Z = wxID_HIGHEST + 12,
};


#endif
