// author: dybisz

#include "entities/points_cloud.h"

PointsCloud::PointsCloud(const vector<Vector3f> &vertices,
                         const Vector3f &translation,
                         const Matrix3f &rotation,
                         const Vector3f scale)
        : __m_vertices(vertices),
          __m_rotation(rotation),
          __m_translation(translation),
          __m_scale(scale) {}

//------------------------------------------------------------------------------
void
PointsCloud::accumulateRotation(const Matrix3f &rotation) {
    __m_rotation = __m_rotation * rotation;
}

//------------------------------------------------------------------------------
void
PointsCloud::accumulateTranslation(const Vector3f &translation) {
    __m_translation += translation;
}

//------------------------------------------------------------------------------
vector<Vector3f>
PointsCloud::getUpdatedVertices() {
    vector<Vector3f> transformedVertices;

    for (const auto &v : __m_vertices) {
        Vector3f transformed(__m_scale(0) * v(0),
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
PointsCloud::setVertices(const vector<Vector3f>& vertices) {
    // TODO: check stdl for something more efficient
    for(int i = 0; i < __m_vertices.size(); ++i) {
        __m_vertices[i] = vertices[i];
    }
}

//------------------------------------------------------------------------------
void
PointsCloud::setTranslation(const Vector3f& translation) {
    __m_translation = translation;
}

//------------------------------------------------------------------------------
void
PointsCloud::setRotation(const Matrix3f& rotation) {
    __m_rotation = rotation;
}

//------------------------------------------------------------------------------
void
PointsCloud::setScale(const Vector3f& scale) {
    __m_scale = scale;
}

//------------------------------------------------------------------------------
Matrix3f
PointsCloud::getRotation() {
    return __m_rotation;
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getRotationAngles() {
    return __m_rotation.eulerAngles(0, 1, 2);
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getTranslation() {
    return __m_translation;
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getScale() {
    return __m_scale;
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getCentroidFromUpdatedVertices() {
    Vector3f centroid(0.0, 0.0, 0.0);
    vector<Vector3f> updVert = getUpdatedVertices();

    for (auto const &vert : updVert) centroid += vert;
    centroid /= (double) updVert.size();

    return centroid;
}