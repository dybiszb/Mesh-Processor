// author: dybisz

#include "entities/points_cloud.h"

PointsCloud::PointsCloud(const vector<Vector3f> &vertices,
                         const Vector3f &translation,
                         const Matrix3f &rotation,
                         const float scale)
        : m_vertices(vertices),
          m_rotation(rotation),
          m_translation(translation),
          m_scale(scale) {}

//------------------------------------------------------------------------------
void
PointsCloud::accumulateRotation(const Matrix3f &rotation) {
    m_rotation = m_rotation * rotation;
}

//------------------------------------------------------------------------------
void
PointsCloud::accumulateTranslation(const Vector3f &translation) {
    m_translation += translation;
}

//------------------------------------------------------------------------------
vector<Vector3f>
PointsCloud::getUpdatedVertices() {
    vector<Vector3f> transformedVertices;

    for (const auto &v : m_vertices) {
        Vector3f transformed = m_scale * v;
        transformed = m_rotation * transformed;
        transformed += m_translation;
        transformedVertices.push_back(transformed);
    }

    return transformedVertices;
}

//------------------------------------------------------------------------------
void
PointsCloud::setScale(const float scale) {
    m_scale = scale;
}

//------------------------------------------------------------------------------
void
PointsCloud::setRotation(const Matrix3f& rotation) {
    m_rotation = rotation;
}

//------------------------------------------------------------------------------
Matrix3f
PointsCloud::getRotation() {
    return m_rotation;
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getTranslation() {
    return m_translation;
}

//------------------------------------------------------------------------------
float
PointsCloud::getScale() {
    return m_scale;
}