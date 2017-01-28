#include "main_app.h"
#include "main_frame.h"
#include "gl_plane.h"

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

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON (ID_BUTTON_LOAD_MESH, MainFrame::OnLoadMesh )
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    MainFrame *frame = new MainFrame(MAIN_WINDOW_TITLE, wxPoint(50, 50), wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT));
    frame->Center();
    frame->Show(true);
    return true;
}

