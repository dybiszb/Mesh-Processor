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
#include "logic/typedefs.h"

using namespace Application;

class PointsCloud {
public:
    typedef std::vector<vec3> kdTreePointsT;
    typedef KDTreeVectorOfVectorsAdaptor<kdTreePointsT, float> kdTreeT;

    /**
     *
     * @param vertices    Vertices loaded from file or somehow created that
     *                    represent the cloud.
     * @param translation Initial translation of the cloud.
     * @param rotation    Initial rotation of the cloud.
     */
    PointsCloud(const std::vector<vec3> &vertices,
                const vec3 &translation = vec3(0.0, 0.0, 0.0),
                const mat3 &rotation = Eigen::Matrix3f::Identity(),
                const vec3 scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    /**
     * Multiplies current rotation with provided one.
     *
     * @param rotation Rotation matrix that will be included in model matrix.
     */
    void accumulateRotation(const mat3 &rotation);

    /**
     * Adds provided translation vector to the current one.
     *
     * @param translation Vector of translation that will be included in
     *                    model matrix.
     */
    void accumulateTranslation(const vec3 &translation);

    /**
     * Applies stored transformations (rotation adn translation) to the
     * points and returns them in form of a vector.
     *
     * @return Vector of Eigen's Vector3f objects representing points of the
     *         cloud.
     */
    std::vector<vec3> getUpdatedVertices();

    /**
     * Creates Kd-Tree for points transformed by stored transformatins
     * (rotation, translation and scaling).
     *
     * @return Kd-Tree of the cloud's points.
     */
    kdTreeT getKdTreeOfUpdatedVertices();

    /* ----- Setters for internal members ----- */
    void setVertices(const std::vector<vec3> &vertices);

    void setTranslation(const vec3 &translation);

    void setScale(const vec3 &scale);

    void setRotation(const mat3 &rotation);

    /* ----- Getters for internal members ----- */
    mat3 getRotation();
    vec3 getRotationAngles();
    vec3 getTranslation();
    vec3 getScale();
    vec3 getCentroidFromUpdatedVertices();

private:
    std::vector<vec3> __m_vertices;
    std::vector<vec3> __m_normals;
    mat3 __m_rotation;
    vec3 __m_translation;
    vec3 __m_scale;

};

#endif
