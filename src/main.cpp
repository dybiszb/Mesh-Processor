//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#include "main_app.h"
#include "main_frame.h"

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    MainFrame *mainWindow = new MainFrame(MAIN_WINDOW_TITLE,
                                          wxPoint(50, 50),
                                          wxSize(MAIN_WINDOW_WIDTH,
                                                 MAIN_WINDOW_HEIGHT));
    mainWindow->Center();
    mainWindow->Show(true);
    return true;
}

