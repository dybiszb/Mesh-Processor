#ifndef GL_CAMERA_H
#define GL_CAMERA_H
#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include "logic/typedefs.h"
#include "rendering/gl_eigen.h"

using namespace Application;

class glOrbitCamera {
public:
    glOrbitCamera();

    mat4& getViewMatrix();

    void rotateX(float angle);

    void rotateY(float angle);

    void changeRadiusBy(float delta);

    void setTarget(const vec3& newTarget);

    // TODO: check if needed
    void translate(float x, float y, float z);

private:
    float phi, theta, radius;
    vec3 position, target, up;
    mat4 viewMatrix;

    void updateViewMatrix();
};


#endif
