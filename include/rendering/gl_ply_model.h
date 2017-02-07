#ifndef GL_PLY_MODEL_H
#define GL_PLY_MODEL_H

#ifdef Success
#undef Success
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <iostream>
#include <fstream>
#include "rendering/gl_shader_program.h"
#include "rendering/gl_eigen.h"

using namespace std;
using namespace Eigen;

class glPlyModel {
public:
    glPlyModel(string path,
               const Vector3f &translation = Vector3f(0.0, 0.0, 0.0));

    ~glPlyModel();

    void render(glShaderProgram &shader, Matrix4f &view, Matrix4f &projection);

    void setRotation(float roll, float yaw, float pitch);

    void setTranslation(float x, float y, float z);

    void setScale(float scale);

    void loadModel(string &path);

    void printInformation();

    /**
     * NOTE: translation and rotation are applied to all points
     *
     * @return
     */
    MatrixXd getMatrixOfPoints();

    /**
     * NOTE: translation and rotation are applied to all points
     *
     * @return
     */
    MatrixXd getMatrixOfTyldaCoordinates();

    /**
     * NOTE: translation and rotation are applied to all points
     *
     * @return
     */
    Vector3f getBarycentricCoordinate();

    vector<Vector3f> getVertices();

private:
    vector<Vector3f> vertices;
    vector<GLfloat> glVertices2;
    vector<GLuint> glFaces2;

    // Transforms
    Vector3f rotation;
    Vector3f translation;
    float scale;

    string path;
    int numberOfVertices = -1;
    int numberOfFaces = -1;
    bool numberOfVerticesFound = false;
    bool numberOfFacesFound = false;
    bool modelLoaded;
    GLuint vbo, vao, ebo;
//    Matrix4f model = Matrix4f::Identity();

    void loadOpenGLData();

    Matrix4f computeModelMatrix();
};

#endif
