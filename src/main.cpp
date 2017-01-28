#include "main_app.h"
#include "main_frame.h"
#include "gl_plane.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_MENU(ID_Hello, MainFrame::OnHello)
                EVT_MENU(wxID_EXIT, MainFrame::OnExit)
                EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

BEGIN_EVENT_TABLE(GLPlane, wxGLCanvas)
                EVT_MOTION(GLPlane::mouseMoved)
                EVT_LEFT_DOWN(GLPlane::mouseDown)
                EVT_LEFT_UP(GLPlane::mouseReleased)
                EVT_RIGHT_DOWN(GLPlane::rightClick)
                EVT_LEAVE_WINDOW(GLPlane::mouseLeftWindow)
                EVT_SIZE(GLPlane::resized)
                EVT_KEY_DOWN(GLPlane::keyPressed)
                EVT_KEY_UP(GLPlane::keyReleased)
                EVT_MOUSEWHEEL(GLPlane::mouseWheelMoved)
                EVT_PAINT(GLPlane::render)
END_EVENT_TABLE()

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    MainFrame *frame = new MainFrame(MAIN_WINDOW_TITLE, wxPoint(50, 50), wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT));
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

    GLPlane * glPane = new GLPlane( (wxFrame*) frame, args);
    sizer->Add(glPane, 1, wxEXPAND);

    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

    frame->Center();
    frame->Show(true);
    return true;
}

