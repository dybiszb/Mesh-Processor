//==============================================================================
//
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef GL_BOX_H
#define GL_BOX_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "rendering/gl_shader_program.h"
#include "rendering/gl_utils.h"
#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>

using namespace Eigen;

class glBox {
public:
    glBox();

    ~glBox();

    void render(glShaderProgram &shader, Matrix4f& view, Matrix4f& projection);

private:
    GLuint vbo, vao, ebo;
    Matrix4f model = Matrix4f::Identity();

    GLfloat vertices[32] = {
            // Positions          // Colors           // Texture Coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left
    };
    GLuint indices[6] = {  // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
    };
};

#endif
