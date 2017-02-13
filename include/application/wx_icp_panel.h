#ifndef ICP_PANEL_H
#define ICP_PANEL_H

#include <wx/wx.h>

class ICPPanel : public wxPanel {
public:
    ICPPanel(wxWindow *parent, const wxPoint &pos);

private:
    wxSlider   *__m_animationSlider;
    wxButton   *__m_startStopAnimationButton;
    wxButton   *__m_nextFrameButton;
    wxButton   *__m_previousFrameButton;
    wxButton   *__m_resetButton;
    wxRadioBox *__m_icpTypeRadioBox;
    wxTextCtrl *__m_deltaTimeText;

    void __InitializeAnimationSliderBox(wxBoxSizer* parent);
    void __InitializeFramesAndTimeBox(wxBoxSizer* parent);
    void __InitializeNextPrevBox(wxBoxSizer* parent);
    void __InitializeResetBox(wxBoxSizer* parent);
    void __InitializeICPTypeBox(wxBoxSizer* parent);
};

enum {
    ID_ANIMATION_SLIDER = wxID_HIGHEST + 100,
};
#endif
