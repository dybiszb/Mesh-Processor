// author: dybisz

#include "rendering/gl_coordinates_frame.h"

//-----------------------------------------------------------------------------
glCoordinatesFrame::glCoordinatesFrame() /*:
        model(unique_ptr<glPlyModel>(
                new glPlyModel("./res/models/coordinates.ply")))*/ {
    model = new glPlyModel("./res/models/coordinates.ply");
}

//-----------------------------------------------------------------------------
glCoordinatesFrame::~glCoordinatesFrame() {
    delete model;
}

//-----------------------------------------------------------------------------
void
glCoordinatesFrame::render(glShaderProgram &shader,
                           Matrix4f &view,
                           Matrix4f &projection) {
    model->render(shader, view, projection);
}

//-----------------------------------------------------------------------------
void
glCoordinatesFrame::position(float x, float y, float z) {

}

//-----------------------------------------------------------------------------
void
glCoordinatesFrame::scale(float x, float y, float z) {

}

//-----------------------------------------------------------------------------
void
glCoordinatesFrame::rotateY(float degrees) {

}
