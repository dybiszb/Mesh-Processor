#ifndef GL_CAMERA_H
#define GL_CAMERA_H
#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "rendering/gl_eigen.h"
#include <iostream>

using namespace std;
using namespace Eigen;

class glOrbitCamera {
public:
    glOrbitCamera();

    Matrix4f& getViewMatrix();

    void rotateX(float angle);

    void rotateY(float angle);

    void changeRadiusBy(float delta);

    void setTarget(const Vector3f& newTarget);

private:
    float phi, theta, radius;
    Vector3f position, target, up;
    Matrix4f viewMatrix;

    void updateViewMatrix();
};


#endif
