#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>

//------------------------------------------------------------------------------
inline void
glCheckForErrors() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error: " << err << endl;
    }
}

#endif
