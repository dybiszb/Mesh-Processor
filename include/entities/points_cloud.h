#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <vector>
#include "nanoflann.hpp"
#include "KDTreeVectorOfVectorsAdaptor.h"

using namespace std;
using namespace Eigen;
using namespace nanoflann;

class PointsCloud {
public:
    typedef vector<Vector3f> kdTreePointsT;
    typedef KDTreeVectorOfVectorsAdaptor<kdTreePointsT, double> kdTreeT;

    /**
     *
     * @param vertices    Vertices loaded from file or somehow created that
     *                    represent the cloud.
     * @param translation Initial translation of the cloud.
     * @param rotation    Initial rotation of the cloud.
     */
    PointsCloud(const vector<Vector3f> &vertices,
                const Vector3f &translation = Vector3f(0.0, 0.0, 0.0),
                const Matrix3f &rotation = Matrix3f::Identity(),
                const Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f));

    /**
     * Multiplies current rotation with provided one.
     *
     * @param rotation Rotation matrix that will be included in model matrix.
     */
    void accumulateRotation(const Matrix3f &rotation);

    /**
     * Adds provided translation vector to the current one.
     *
     * @param translation Vector of translation that will be included in
     *                    model matrix.
     */
    void accumulateTranslation(const Vector3f &translation);

    /**
     * Applies stored transformations (rotation adn translation) to the
     * points and returns them in form of a vector.
     *
     * @return Vector of Eigen's Vector3f objects representing points of the
     *         cloud.
     */
    vector<Vector3f> getUpdatedVertices();

    /**
     *
     */
    kdTreeT getKdTreeOfUpdatedVertices();

    void setVertices(const vector<Vector3f>& vertices);

    /**
     *
     * @param scale
     */
    void setScale(const Vector3f &scale);

    /**
     *
     * @param rotation
     */
    void setRotation(const Matrix3f &rotation);

    /**
     * @return Rotation matrix of the cloud.
     */
    Matrix3f getRotation();

    /**
     *
     * @return
     */
    Vector3f getRotationAngles();

    /**
     * @return Translation vector of the cloud.
     */
    Vector3f getTranslation();

    /**
     * @return Scaling factor of the cloud.
     */
    Vector3f getScale();

protected:
    vector<Vector3f> m_vertices;
    vector<Vector3f> m_normals;
    Matrix3f m_rotation;
    Vector3f m_translation;
    Vector3f m_scale;

};

#endif
