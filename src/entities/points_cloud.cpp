// author: dybisz

#include "entities/points_cloud.h"

PointsCloud::PointsCloud(const vector<Vector3f> &vertices,
                         const Vector3f &translation,
                         const Matrix3f &rotation,
                         const Vector3f scale)
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
        Vector3f transformed(m_scale(0) * v(0),
                             m_scale(1) * v(1),
                             m_scale(2) * v(2));

        transformed = m_rotation * transformed;
        transformed += m_translation;
        transformedVertices.push_back(transformed);
    }

    return transformedVertices;
}

//------------------------------------------------------------------------------
PointsCloud::kdTreeT
PointsCloud::getKdTreeOfUpdatedVertices() {
    kdTreeT updatedVerticesKdTree(3, getUpdatedVertices() );
    updatedVerticesKdTree.index->buildIndex();
    return updatedVerticesKdTree;
}

//------------------------------------------------------------------------------
void
PointsCloud::setVertices(const vector<Vector3f>& vertices) {
    // TODO: check stdl for something more efficient
    for(int i = 0; i < m_vertices.size(); ++i) {
        m_vertices[i] = vertices[i];
    }
}

//------------------------------------------------------------------------------
void
PointsCloud::setTranslation(const Vector3f& translation) {
    m_translation = translation;
}

//------------------------------------------------------------------------------
void
PointsCloud::setRotation(const Matrix3f& rotation) {
    m_rotation = rotation;
}

//------------------------------------------------------------------------------
void
PointsCloud::setScale(const Vector3f& scale) {
    m_scale = scale;
}

//------------------------------------------------------------------------------
Matrix3f
PointsCloud::getRotation() {
    return m_rotation;
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getRotationAngles() {
    return m_rotation.eulerAngles(0, 1, 2);
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getTranslation() {
    return m_translation;
}

//------------------------------------------------------------------------------
Vector3f
PointsCloud::getScale() {
    return m_scale;
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