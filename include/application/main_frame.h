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
#include "application/model_panel.h"
#include "rendering/gl_eigen.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    void loadDefaultMesh();

    void OnShow(wxShowEvent &event);

private:
    glPlane *m_glPanel;
    wxButton *m_nextICPFrame;
    wxTreeItemId m_meshesRoot;
    wxTreeCtrl *m_treeCtrl;
    ModelPanel *m_modelPanel;
    wxSlider *m_icpSlider;

    wxStaticBitmap *m_meshesTitleBitmap;
    wxStaticBitmap *m_icpTitleBitmap;
    wxStaticBitmap *m_titleBitmap;

    void initializeMeshTree(wxBoxSizer *parent);

    void initializeICPBox(wxBoxSizer *parent);

    void initializeMeshOptionsBox(wxBoxSizer *parent);

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

    /**
     *
     * @param event
     */
    void OnMeshesTreeItemClicked(wxTreeEvent &event);

    /**
     *
     * @param event
     */
    void OnNextFrame(wxCommandEvent &event);

    /**
     *
     * @param event
     */
    void OnIdleWindow(wxIdleEvent &event);

    void OnIntroduceNoise(wxCommandEvent &event);

    void OnNormalsCheckbox(wxCommandEvent& event);

    void OnMoveCentroidToOrigin(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BUTTON_LOAD_MESH = wxID_HIGHEST + 1,
    ID_BUTTON_DELETE_MESH = wxID_HIGHEST + 2,
    ID_BUTTON_RUN_ICP = wxID_HIGHEST + 3,
    ID_BUTTON_NEXT_FRAME = wxID_HIGHEST + 4,
    ID_MESHES_TREE_CTRL = wxID_HIGHEST + 5,
    ID_ICP_SLIDER = wxID_HIGHEST + 6,
    ID_TEXT_TRANSLATION_X = wxID_HIGHEST + 7,
    ID_TEXT_TRANSLATION_Y = wxID_HIGHEST + 8,
    ID_TEXT_TRANSLATION_Z = wxID_HIGHEST + 9,
    ID_TEXT_ROTATION_X = wxID_HIGHEST + 10,
    ID_TEXT_ROTATION_Y = wxID_HIGHEST + 11,
    ID_TEXT_ROTATION_Z = wxID_HIGHEST + 12,
};


#endif
