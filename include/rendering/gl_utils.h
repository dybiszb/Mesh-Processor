#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
using namespace std;

//------------------------------------------------------------------------------
inline void
glCheckForErrors() {
//    GLenum err;
//    while ((err = glGetError()) != GL_NO_ERROR) {
//        cerr << "OpenGL error: " << err << endl;
//    }

    GLenum errCode;
    string errString;
    if ((errCode = glGetError()) !=
        GL_NO_ERROR)
    {
        errString = (const char*)gluErrorString(errCode);
        cout << "error:" <<  errString << endl;
    }



}

#endif
