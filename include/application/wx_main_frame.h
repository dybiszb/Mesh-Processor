//==============================================================================
// The class constitutes a core place for transferring data between widgets.
// It is in charge of the following panels:
// [:] Main Viewer
// [:] 'Loaded Meshes' Panel
// [:] 'ICP Algorithm' Panel
// [:] 'Mesh Options' Panel
// Most of the aforementioned widgets only presents the class with id's of their
// internal members (like buttons or text controls) and leave it with event
// handling routine. Exceptions in the rule only occur if a single widgets's
// control functionality does not depend on any other external widget.
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
#include "wx_gl_plane.h"
#include <iostream>
#include "application/wx_model_panel.h"
#include "application/wx_icp_panel.h"
#include "rendering/gl_eigen.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    /* ----- Widgets ----- */
    glPlane     *__m_glPanel;
    wxTreeItemId __m_meshesRoot;
    wxTreeCtrl  *__m_treeCtrl;
    ModelPanel  *__m_modelPanel;
    ICPPanel    *__m_icpPanel;

    /* ----- Headers Bitmaps ----- */
    wxStaticBitmap *__m_meshesTitleBitmap;
    wxStaticBitmap *__m_icpTitleBitmap;
    wxStaticBitmap *__m_meshOptionsBitmap;
    std::string     __m_meshesTitlePath; // TODO; try const
    std::string     __m_icpTitlePath;
    std::string     __m_meshOptionsTitlePath;

    /* ----- Auxiliary ----- */
    void __LoadDefaultMesh();
    wxTreeItemId __AppendMeshToTree(string path);

    /* ----- Widgets Initialization ----- */
    void __InitializeMeshTree(wxBoxSizer *parent);
    void __InitializeICPBox(wxBoxSizer *parent);
    void __InitializeMeshOptionsBox(wxBoxSizer *parent);

    /* ----- Events Handlers: General ----- */
    void __OnIdleWindow(wxIdleEvent &event);
    void __OnShow(wxShowEvent &event);

    /* ----- Events Handlers: Loaded Meshes ----- */
    void __OnMeshesTreeItemClicked(wxTreeEvent &event);
    void __OnLoadMesh(wxCommandEvent &event);
    void __OnDeleteMesh(wxCommandEvent &event);

    /* ----- Events Handlers: ICP Algorithm ----- */
    void __OnPrevFrameICP(wxCommandEvent &event);
    void __OnNextFrameICP(wxCommandEvent &event);
    void __OnICPRun(wxCommandEvent &event);
    void __OnICPReset(wxCommandEvent &event);
    void __OnNearestNeighborsCheckbox(wxCommandEvent &event);

    /* ----- Events Handlers: Mesh options ----- */
    void __OnTranslationEditing(wxCommandEvent &event);
    void __OnRotationEditing(wxCommandEvent &event);
    void __OnScalingEditing(wxCommandEvent &event);
    void __OnIntroduceNoise(wxCommandEvent &event);
    void __OnMoveCentroidToOrigin(wxCommandEvent &event);
    void __OnNormalsCheckbox(wxCommandEvent &event);
    void __OnWireframe(wxCommandEvent &event);
    void __OnICPBaseCheckbox(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BUTTON_LOAD_MESH   = wxID_HIGHEST + 1,
    ID_BUTTON_DELETE_MESH = wxID_HIGHEST + 2,
    ID_MESHES_TREE_CTRL   = wxID_HIGHEST + 3,
    ID_ICP_SLIDER         = wxID_HIGHEST + 4,
};


#endif
