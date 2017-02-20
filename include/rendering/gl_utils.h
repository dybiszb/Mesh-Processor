#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include "logic/typedefs.h"

//------------------------------------------------------------------------------
inline void
glCheckForErrors() {
    GLenum errCode;
    std::string errString;
    if ((errCode = glGetError()) !=
        GL_NO_ERROR)
    {
        errString = (const char*)gluErrorString(errCode);
        std::cout << "OpenGL Error:" <<  errString << std::endl;
    }

}

#endif
