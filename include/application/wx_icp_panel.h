#ifndef ICP_PANEL_H
#define ICP_PANEL_H

#include <wx/wx.h>
#include <iostream>
#include <sstream>
#include <string>
#include "logic/icp_algoriithm.h"
#include <ctime>

using namespace std;

class ICPPanel : public wxPanel {
public:
    ICPPanel(wxWindow *parent, const wxPoint &pos);

    /**
     * Updates the label with frame rates.
     *
     * @param frame Current frame number.
     * @param outOf Maximum number of frames.
     */
    void SetFramesRate(int frame, unsigned long outOf);

    /**
     * Updates the label with computation time of ICP algorithm.
     *
     * @param time New computation time.
     */
    void SetComputationTime(double time);

    // TODO: excpt etc
    const ICPResults& NextFrame();

    // TODO
    const ICPResults& PrevFrame();

    /**
     * Dim all widgets that are useless without the ICP results or activate
     * all of them.
     *
     * @param isActive Activate or dim?
     */
    void SetActive(bool isActive);

    // NOTE: Will update/activate widgets
    void RunICP(const PointsCloud& m1PC, const PointsCloud& m2PC);

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

    /* ----- ICP Results ----- */
    ICPAlgorithm       __m_icpAlgorithm;
    int                __m_lastIndex;
    int                __m_currentIndex;
    vector<ICPResults> __m_results;

    /* ----- Widgets Initializations ----- */
    void __InitializeAnimationSliderBox(wxBoxSizer* parent);
    void __InitializeFramesAndTimeBox(wxBoxSizer* parent);
    void __InitializeNextPrevBox(wxBoxSizer* parent);
    void __InitializeResetBox(wxBoxSizer* parent);
    void __InitializeICPTypeBox(wxBoxSizer* parent);

};

enum {
    ID_ANIMATION_SLIDER = wxID_HIGHEST + 100,
    ID_RUN_ICP_BUTTON   = wxID_HIGHEST + 101,
    ID_RESET_ICP_BUTTON = wxID_HIGHEST + 102,
    ID_NEXT_PREV_ICP_BUTTON = wxID_HIGHEST + 103,
    ID_NEXT_FRAME_ICP_BUTTON = wxID_HIGHEST + 104,
};
#endif
