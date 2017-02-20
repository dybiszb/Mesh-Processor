#ifndef GL_EIGEN_H
#define GL_EIGEN_H

#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "logic/typedefs.h"

using namespace Application;

//------------------------------------------------------------------------------
inline void
lookAt(vec3 &position, vec3 &target, vec3 &up, mat4 &mViewMatrix) {
    mat3 R;
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
               mat4 &mProjectionMatrix) {
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

inline Eigen::Affine3d
createRotationMatrix(const vec3 &perAxisAngles) {
    Eigen::Affine3d rx =
            Eigen::Affine3d(Eigen::AngleAxisd(perAxisAngles(0), Eigen::Vector3d
                    (1, 0, 0)));
    Eigen::Affine3d ry =
            Eigen::Affine3d(Eigen::AngleAxisd(perAxisAngles(1),
                                              Eigen::Vector3d(0, 1, 0)));
    Eigen::Affine3d rz =
            Eigen::Affine3d(Eigen::AngleAxisd(perAxisAngles(2),
                                              Eigen::Vector3d(0, 0, 1)));
    return rz * ry * rx;
}

//------------------------------------------------------------------------------
inline mat3
rotationMatrix(float roll, float yaw, float pitch) {
    Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitX());

    Eigen::Quaternion<double> q = rollAngle * yawAngle * pitchAngle;

    Eigen::Matrix3d rotationMatrix = q.matrix();
    mat3 r = rotationMatrix.cast<float>();

    return r;
}

//------------------------------------------------------------------------------
inline float
degToRad(float deg) { return deg * (180.0f / M_PI); }

//------------------------------------------------------------------------------
inline float
radToDeg(float rad) { return rad * (M_PI / 180.0f); }

#endif
