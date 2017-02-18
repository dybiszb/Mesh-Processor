//==============================================================================
// Naming convention:
// > '__' (double underscore) <=> private function/variable
// > '_' (single underscore) <=> protected function/variable
// > 'm_' <=> member function/variable
// > methods starts from upper case letter
// > variables starts from lower case letter
// > both: methods and variables use camelcase
//
// Max line length: 80 digits.
//
// I only return exception in form of string objects. It helps in transparent
// dialog creation and nothing more sophisticated is really needed for this
// project.
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------
#include <GL/glew.h>
#include <cstdlib>
#include "application/wx_main_app.h"
#include "application/wx_main_frame.h"

#define MAIN_WINDOW_WIDTH  1024
#define MAIN_WINDOW_HEIGHT 768
#define MAIN_WINDOW_TITLE  "Simple Mesh Processor"

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    std::srand(unsigned(std::time(0)));
    MainFrame *mainWindow =
            new MainFrame(MAIN_WINDOW_TITLE,
                          wxPoint(50, 50),
                          wxSize(MAIN_WINDOW_WIDTH,
                                 MAIN_WINDOW_HEIGHT));

    mainWindow->Center();
    mainWindow->Show(true);

    return true;
}

