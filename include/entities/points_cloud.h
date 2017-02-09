#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <vector>

using namespace std;
using namespace Eigen;

class PointsCloud {
public:
    PointsCloud(const vector<Vector3f> &vertices,
                const Vector3f &translation = Vector3f(0.0, 0.0, 0.0),
                const Matrix3f &rotation = Matrix3f::Identity());

    void accumulateRotation(const Matrix3f &rotation);

    void accumulateTranslation(const Vector3f &translation);

    vector<Vector3f> getUpdatedVertices();

    Matrix3f getRotation();

    Vector3f getTranslation();

protected:
    vector<Vector3f> m_vertices;
    Matrix3f m_rotation;
    Vector3f m_translation;


};

#endif
