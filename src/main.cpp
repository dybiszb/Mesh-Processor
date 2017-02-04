//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------
#include <GL/glew.h>
#include "application/main_app.h"
#include "application/main_frame.h"
#include "rendering/gl_shader_program.h"

#define MAIN_WINDOW_WIDTH  1024
#define MAIN_WINDOW_HEIGHT 768
#define MAIN_WINDOW_TITLE  "Simple Mesh Processor"

wxIMPLEMENT_APP(MainApp);

bool initGLEW() {
    glewExperimental = (GLboolean) true;
    return glewInit() == GLEW_OK;
}

bool MainApp::OnInit() {

    MainFrame *mainWindow = new MainFrame(MAIN_WINDOW_TITLE,
                                          wxPoint(50, 50),
                                          wxSize(MAIN_WINDOW_WIDTH,
                                                 MAIN_WINDOW_HEIGHT));


    mainWindow->Center();
    mainWindow->Show(true);

    return true;
}

