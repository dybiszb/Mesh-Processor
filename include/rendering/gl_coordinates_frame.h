#ifndef GL_COORDINATES_FRAME_H
#define GL_COORDINATES_FRAME_H

#ifdef Success
#undef Success
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include "rendering/gl_ply_model.h"

using namespace std;
using namespace Eigen;

class glCoordinatesFrame {
public:
    glCoordinatesFrame();

    ~glCoordinatesFrame();

    void position(float x, float y, float z);

    void scale(float x, float y, float z);

    void render(glShaderProgram &shader, Matrix4f &view, Matrix4f & projection);

private:
    glPlyModel *model;

    void rotateY(float degrees);
};

#endif
