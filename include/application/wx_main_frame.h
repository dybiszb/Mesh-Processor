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
#include "wx_gl_plane.h"
#include <iostream>
#include "application/wx_model_panel.h"
#include "application/wx_icp_panel.h"
#include "rendering/gl_eigen.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    glPlane *m_glPanel;
    wxTreeItemId m_meshesRoot;
    wxTreeCtrl *m_treeCtrl;
    ModelPanel *m_modelPanel;

    ICPPanel* m_icpPanel;

    wxStaticBitmap *m_meshesTitleBitmap;
    wxStaticBitmap *m_icpTitleBitmap;
    wxStaticBitmap *m_titleBitmap;


    void loadDefaultMesh();
    wxTreeItemId appendMeshToTree(string path);

    void initializeMeshTree(wxBoxSizer *parent);
    void initializeICPBox(wxBoxSizer *parent);
    void initializeMeshOptionsBox(wxBoxSizer *parent);

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
    ID_BUTTON_LOAD_MESH = wxID_HIGHEST + 1,
    ID_BUTTON_DELETE_MESH = wxID_HIGHEST + 2,
    ID_MESHES_TREE_CTRL = wxID_HIGHEST + 5,
    ID_ICP_SLIDER = wxID_HIGHEST + 6,
};


#endif
