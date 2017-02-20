//==============================================================================
// The class in charge of 'ICP algorithm' section. besides encapsulating all of
// get/set functionality it also introduces procedures that run the ICP
// algorithm and returns next/previous frame of the solution. The latter is
// based on internal index pointing to the current frame, which is tightly
// binded with the section's slider. Please note that the solutions are stored
// in form of a vector that holds structures of consecutive transformations
// generated during ICP computations.
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef ICP_PANEL_H
#define ICP_PANEL_H

#include <wx/wx.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "logic/icp_algoriithm.h"
#include <ctime>

class ICPPanel : public wxPanel {
public:
    ICPPanel(wxWindow *parent, const wxPoint &pos);

    /**
     * Run ICP for specified clouds of points. Appropriate widgets will be
     * updated automatically.
     *
     * @param m1PC M1's cloud of points.
     * @param m2PC M2's cloud of points.
     */
    void RunICP(const PointsCloud& m1PC, const PointsCloud& m2PC);

    /**
     * Returns previous frame (with respect to currently selected) of the
     * ICP solutions. In case when no solutions are available or caller asks
     * for an index that is out of scope: exception will be thrown.
     *
     * @return Structure of transformations related to a next frame of the ICP
     *         solution.
     */
    ICPResults NextFrame();

    /**
     * Returns previous frame (with respect to currently selected) of the
     * ICP solutions. In case when no solutions are available or caller asks
     * for an index that is out of scope: exception will be thrown.
     *
     * @return Structure of transformations related to a previous frame of
     *        the ICP solution.
     */
    ICPResults PrevFrame();

    void SetFramesRate(int frame, unsigned long outOf);
    void SetComputationTime(double time);
    void SetActive(bool isActive);
    void SetNearestNeighbors(bool nns);



private:
    enum { POINT_TO_POINT, POINT_TO_PLANE };

    /* ----- Widgets Handlers ----- */
    wxSlider     *__m_animationSlider;
    wxStaticText *__m_framesRateText;
    wxStaticText *__m_timeText;
    wxButton     *__m_startStopAnimationButton;
    wxButton     *__m_nextFrameButton;
    wxButton     *__m_previousFrameButton;
    wxButton     *__m_runButton;
    wxButton     *__m_resetButton;
    wxRadioBox   *__m_icpTypeRadioBox;
    wxTextCtrl   *__m_deltaTimeText;
    wxCheckBox   *__m_nearestNeighboursCheckbox;
    wxCheckBox   *__m_subSamplingCheckbox;
    wxTextCtrl   *__m_subSamplingTextCtrl;

    /* ----- ICP Results ----- */
    ICPAlgorithm       __m_icpAlgorithm;
    int                __m_lastIndex;
    int                __m_currentIndex;
    bool               __m_useNearestNeighbors;
    bool               __m_useSubSampling;
    int                __m_samples;
    std::vector<ICPResults> __m_results;

    /* ----- Widgets Initializations ----- */
    void __InitializeAnimationSliderBox(wxBoxSizer* parent);
    void __InitializeFramesAndTimeBox(wxBoxSizer* parent);
    void __InitializeNextPrevBox(wxBoxSizer* parent);
    void __InitializeResetBox(wxBoxSizer* parent);
    void __InitializeICPTypeBox(wxBoxSizer* parent);
    void __InitializeBottomCheckboxes(wxBoxSizer *parent);

    /* ----- Internal Event Calls ----- */
    void __OnSubSamplesCheckbox(wxCommandEvent &event);
    void __OnSubSampleTextCtrl(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

enum {
    ID_ANIMATION_SLIDER            = wxID_HIGHEST + 100,
    ID_RUN_ICP_BUTTON              = wxID_HIGHEST + 101,
    ID_RESET_ICP_BUTTON            = wxID_HIGHEST + 102,
    ID_NEXT_PREV_ICP_BUTTON        = wxID_HIGHEST + 103,
    ID_NEXT_FRAME_ICP_BUTTON       = wxID_HIGHEST + 104,
    ID_NEAREST_NEIGHBOURS_CHECKBOX = wxID_HIGHEST + 105,
    ID_SUB_SAMPLING_CHECKBOX       = wxID_HIGHEST + 106,
    ID_SUB_SAMPLING_TEXT           = wxID_HIGHEST + 107,
};
#endif
