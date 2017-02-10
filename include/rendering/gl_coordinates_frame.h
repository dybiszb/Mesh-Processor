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
#include "rendering/gl_shader_program.h"
#include "rendering/gl_eigen.h"

using namespace std;
using namespace Eigen;

class glCoordinatesFrame {
public:
    glCoordinatesFrame();

    void render(glShaderProgram &shader,
                Matrix4f &view,
                Matrix4f &projection);

private:
    unique_ptr<glPlyModel> m_xAxisModel;
    unique_ptr<glPlyModel> m_yAxisModel;
    unique_ptr<glPlyModel> m_zAxisModel;
    unique_ptr<glPlyModel> m_plane;

    const Vector4f m_xAxisColor;
    const Vector4f m_yAxisColor;
    const Vector4f m_zAxisColor;

    const Matrix3f m_xAxisRotation;
    const Matrix3f m_yAxisRotation;
    const Matrix3f m_zAxisRotation;

    void initializeAxis(unique_ptr<glPlyModel>& axis,
                        const Vector4f& color,
                        const Matrix3f rotation);
};

#endif
