// author: dybisz

#include "entities/points_cloud.h"

PointsCloud::PointsCloud(const std::vector<Eigen::Vector3f> &vertices,
                         const Eigen::Vector3f &translation,
                         const Eigen::Matrix3f &rotation,
                         const Eigen::Vector3f scale)
        : __m_vertices(vertices),
          __m_rotation(rotation),
          __m_translation(translation),
          __m_scale(scale) {}

//------------------------------------------------------------------------------
void
PointsCloud::accumulateRotation(const Eigen::Matrix3f &rotation) {
    __m_rotation = __m_rotation * rotation;
}

//------------------------------------------------------------------------------
void
PointsCloud::accumulateTranslation(const Eigen::Vector3f &translation) {
    __m_translation += translation;
}

//------------------------------------------------------------------------------
std::vector<Eigen::Vector3f>
PointsCloud::getUpdatedVertices() {
    std::vector<Eigen::Vector3f> transformedVertices;

    for (const auto &v : __m_vertices) {
        Eigen::Vector3f transformed(__m_scale(0) * v(0),
                             __m_scale(1) * v(1),
                             __m_scale(2) * v(2));

        transformed = __m_rotation * transformed;
        transformed += __m_translation;
        transformedVertices.push_back(transformed);
    }

    return transformedVertices;
}

//------------------------------------------------------------------------------
PointsCloud::kdTreeT
PointsCloud::getKdTreeOfUpdatedVertices() {
    kdTreeT updatedVerticesKdTree(3, getUpdatedVertices(), 10);
    updatedVerticesKdTree.index->buildIndex();
    return updatedVerticesKdTree;
}

//------------------------------------------------------------------------------
void
PointsCloud::setVertices(const std::vector<Eigen::Vector3f>& vertices) {
    // TODO: check stdl for something more efficient
    for(int i = 0; i < __m_vertices.size(); ++i) {
        __m_vertices[i] = vertices[i];
    }
}

//------------------------------------------------------------------------------
void
PointsCloud::setTranslation(const Eigen::Vector3f& translation) {
    __m_translation = translation;
}

//------------------------------------------------------------------------------
void
PointsCloud::setRotation(const Eigen::Matrix3f& rotation) {
    __m_rotation = rotation;
}

//------------------------------------------------------------------------------
void
PointsCloud::setScale(const Eigen::Vector3f& scale) {
    __m_scale = scale;
}

//------------------------------------------------------------------------------
Eigen::Matrix3f
PointsCloud::getRotation() {
    return __m_rotation;
}

//------------------------------------------------------------------------------
Eigen::Vector3f
PointsCloud::getRotationAngles() {
    return __m_rotation.eulerAngles(0, 1, 2);
}

//------------------------------------------------------------------------------
Eigen::Vector3f
PointsCloud::getTranslation() {
    return __m_translation;
}

//------------------------------------------------------------------------------
Eigen::Vector3f
PointsCloud::getScale() {
    return __m_scale;
}

//------------------------------------------------------------------------------
Eigen::Vector3f
PointsCloud::getCentroidFromUpdatedVertices() {
    Eigen::Vector3f centroid(0.0, 0.0, 0.0);
    std::vector<Eigen::Vector3f> updVert = getUpdatedVertices();

    for (auto const &vert : updVert) centroid += vert;
    centroid /= (double) updVert.size();

    return centroid;
}