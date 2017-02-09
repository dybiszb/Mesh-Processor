// author: dybisz

#include "entities/points_cloud.h"

PointsCloud::PointsCloud(const vector<Vector3f> &vertices,
                         const Vector3f &translation,
                         const Matrix3f &rotation)
        : m_vertices(vertices), m_rotation(rotation),
          m_translation(translation) {

}

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
//    Matrix4f modelMatrix = computeModelMatrix();

    for (const auto& v : m_vertices) {
        // Extend dimensions to homogeneous in order to include translation
        // from model matrix
//        Vector4f homoVector(vertices[i](0),
//                            vertices[i](1),
//                            vertices[i](2),
//                            1.0);
//
//        Vector4f transformedVertex = modelMatrix * homoVector;
//
//        Vector3f outputVertex(transformedVertex(0),
//                              transformedVertex(1),
//                              transformedVertex(2));
        Vector3f transformed = m_rotation * v;
        transformed += m_translation;
        transformedVertices.push_back(transformed);
    }

    return transformedVertices;
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