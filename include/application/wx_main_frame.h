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

    void OnShow(wxShowEvent &event);

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

    /* ----- Events Handlers: Loaded Meshes ----- */
    void __onMeshesTreeItemClicked(wxTreeEvent &event);
    void __onLoadMesh(wxCommandEvent &event);
    void __onDeleteMesh(wxCommandEvent &event);

    /* ----- Events Handlers: ICP Algorithm ----- */
    void __onPrevFrameICP(wxCommandEvent &event);
    void __onNextFrameICP(wxCommandEvent &event);
    void __onICPRun(wxCommandEvent &event);
    void __onICPReset(wxCommandEvent &event);
    void __onNearestNeighborsCheckbox(wxCommandEvent &event);

    /* ----- Events Handlers: Mesh options ----- */
    void __onTranslationEditing(wxCommandEvent &event);
    void __onRotationEditing(wxCommandEvent &event);
    void __onScalingEditing(wxCommandEvent &event);
    void __onIntroduceNoise(wxCommandEvent &event);
    void __onMoveCentroidToOrigin(wxCommandEvent &event);
    void __onNormalsCheckbox(wxCommandEvent &event);
    void __onWireframe(wxCommandEvent &event);
    void __onICPBaseCheckbox(wxCommandEvent &event);

    /* ----- Events Handlers: General ----- */
    void __onIdleWindow(wxIdleEvent &event);

wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BUTTON_LOAD_MESH = wxID_HIGHEST + 1,
    ID_BUTTON_DELETE_MESH = wxID_HIGHEST + 2,
    ID_BUTTON_RUN_ICP = wxID_HIGHEST + 3,
    ID_BUTTON_NEXT_FRAME = wxID_HIGHEST + 4,
    ID_MESHES_TREE_CTRL = wxID_HIGHEST + 5,
    ID_ICP_SLIDER = wxID_HIGHEST + 6,
};


#endif
