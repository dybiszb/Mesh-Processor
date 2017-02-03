// author: dybisz

#include "rendering/gl_camera.h"

//------------------------------------------------------------------------------
glOrbitCamera::glOrbitCamera()
        : phi(3.14f/2.0f),
          theta(3.14f/2.0f),
          radius(1.0f),
          position(0.0f, 0.0f, 3.0f),
          target(0.0f, 0.0f, 0.0f),
          up(0.0f, 1.0f, 0.0f) { updateViewMatrix(); }

//------------------------------------------------------------------------------
Matrix4f &
glOrbitCamera::getViewMatrix() {
    updateViewMatrix();
    return viewMatrix;
}

//------------------------------------------------------------------------------
void
glOrbitCamera::rotateX(float angle) {
    theta += 0.0174533 * angle;
}

//------------------------------------------------------------------------------
void
glOrbitCamera::rotateY(float angle) {
    phi += 0.0174533 * angle;
    if(phi >= 3.14f/2.0) phi = 3.14f/2.0f;
    if(phi <= -3.14f/2.0) phi = -3.14f/2.0f;
}

//------------------------------------------------------------------------------
void
glOrbitCamera::changeRadiusBy(float delta) {
    radius += delta;
}

//------------------------------------------------------------------------------
void
glOrbitCamera::setTarget(const Vector3f &newTarget) {
    // TODO: check if it creates copy
    target = newTarget;
}

//------------------------------------------------------------------------------
void
glOrbitCamera::updateViewMatrix() {
    float x = static_cast<float> (radius * sin(phi) * cos(theta));
    float y = static_cast<float> (radius * cos(phi));
    float z = static_cast<float> (radius * sin(phi) * sin(theta));

    position(0) = x;
    position(1) = y;
    position(2) = z;

    lookAt(position, target, up, viewMatrix);
}