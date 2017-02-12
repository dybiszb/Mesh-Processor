// author: dybisz

#include "rendering/gl_coordinates_frame.h"

//------------------------------------------------------------------------------
glCoordinatesFrame::glCoordinatesFrame() :
        m_xAxisColor(185.0f / 256.0f, 29.0f / 256.0f, 71.0f / 256.0f, 1.0f),
        m_yAxisColor(30.0f / 256.0f, 113.0f / 256.0f, 69.0f / 256.0f, 1.0f),
        m_zAxisColor(43.0f / 256.0f, 87.0f / 256.0f, 151.0f / 256.0f, 1.0f),
        m_xAxisRotation(rotationMatrix(0.0f, 3.14f / 2.0f, 0.0f)),
        m_yAxisRotation(rotationMatrix(0.0f, 0.0f, -3.14f/ 2.0f)),
        m_zAxisRotation(rotationMatrix(0.0f, 0.0f, 0.0f)) {

    initializeAxis(m_xAxisModel, m_xAxisColor, m_xAxisRotation);
    initializeAxis(m_yAxisModel, m_yAxisColor, m_yAxisRotation);
    initializeAxis(m_zAxisModel, m_zAxisColor, m_zAxisRotation);

    m_plane = unique_ptr<glPlyModel>(new glPlyModel("./res/models/plane"
                                                              ".ply"));
    m_plane->m_pointsCloud->setRotation(rotationMatrix(0.0f, 0.0f, 3.14f/2.0f));
    float sc = 0.1;
    Vector3f scale(sc, sc, sc);
    m_plane->m_pointsCloud->setScale(scale);
    m_plane->setColor(Vector4f(0.5f, 0.5f, 0.5f, 1.0f));
}

//------------------------------------------------------------------------------
void
glCoordinatesFrame::initializeAxis(unique_ptr<glPlyModel>& axis,
                    const Vector4f& color,
                    const Matrix3f rotation) {
    axis = unique_ptr<glPlyModel>(new glPlyModel("./res/models/axis.ply"));
    axis->setColor(color);
    axis->setWireframe(false);
    float sc = 0.1;
    Vector3f scale(sc, sc, sc);
    axis->m_pointsCloud->setScale(scale);
    axis->m_pointsCloud->setRotation(rotation);
}

//------------------------------------------------------------------------------
void
glCoordinatesFrame::render(glShaderProgram &shader,
                           Matrix4f &view,
                           Matrix4f &projection) {

    m_plane->render(shader, view, projection);
    m_xAxisModel->render(shader, view, projection);
    m_yAxisModel->render(shader, view, projection);
    m_zAxisModel->render(shader, view, projection);

}
