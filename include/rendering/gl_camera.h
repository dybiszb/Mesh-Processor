#ifndef GL_CAMERA_H
#define GL_CAMERA_H
#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

//------------------------------------------------------------------------------
inline void
lookAt(Vector3f &position, Vector3f &target,
       Vector3f &up, Matrix4f &mViewMatrix) {
    Matrix3f R;
    R.col(2) = (position - target).normalized();
    R.col(0) = up.cross(R.col(2)).normalized();
    R.col(1) = R.col(2).cross(R.col(0));
    mViewMatrix.topLeftCorner<3, 3>() = R.transpose();
    mViewMatrix.topRightCorner<3, 1>() = -R.transpose() * position;
    mViewMatrix(3, 3) = 1.0f;
}

//------------------------------------------------------------------------------
inline void
setPerspective(float fovY, float aspect, float near, float far,
               Matrix4f &mProjectionMatrix) {
    float theta = fovY * 0.5;
    float range = far - near;
    float invtan = 1. / tan(theta);

    mProjectionMatrix(0, 0) = invtan / aspect;
    mProjectionMatrix(1, 1) = invtan;
    mProjectionMatrix(2, 2) = -(near + far) / range;
    mProjectionMatrix(3, 2) = -1;
    mProjectionMatrix(2, 3) = -2 * near * far / range;
    mProjectionMatrix(3, 3) = 0;
}


#endif
