// author: dybisz

#include "rendering/gl_camera.h"

//------------------------------------------------------------------------------
glOrbitCamera::glOrbitCamera()
        : phi(0.899793),
          theta(2.33445),
          radius(0.4f),
          position(0.0f, 0.0f, 3.0f),
          target(0.0f, 0.05f, 0.0f),
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
    if(phi >= 3.14f) phi = 3.14f;
    if(phi <= 0.0f) phi = 0.0001f;
}

//------------------------------------------------------------------------------
void
glOrbitCamera::changeRadiusBy(float delta) {
    radius += delta;
    if(radius <= 0.1) radius = 0.1;
}

void
glOrbitCamera::translate(float x, float y, float z) {
    target(0) += x;
    target(1) += y;
    target(2) += z;

    updateViewMatrix();
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