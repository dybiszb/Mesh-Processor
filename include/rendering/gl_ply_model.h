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
#include <memory>
#include <string>
#include <string>
#include <iostream>
#include <fstream>
#include "entities/points_cloud.h"
#include "rendering/gl_shader_program.h"
#include "rendering/gl_eigen.h"

using namespace std;
using namespace Eigen;

class glPlyModel {
public:
    glPlyModel(string path,
               const Vector3f &translation = Vector3f(0.0, 0.0, 0.0),
               const Matrix3f &rotation = Matrix3f::Identity());

    ~glPlyModel();

    void render(glShaderProgram &shader, Matrix4f &view, Matrix4f &projection);


    void printInformation();

    void setSelected(bool isSelected);

    shared_ptr<PointsCloud> m_pointsCloud;
private:
    vector<Vector3f> m_vertices;
    vector<GLfloat> glVertices2;
    vector<GLuint> glFaces2;
    int m_isSelected = 0;
    string path;
    int numberOfVertices = -1;
    int numberOfFaces = -1;
    bool numberOfVerticesFound = false;
    bool numberOfFacesFound = false;
    bool modelLoaded;
    GLuint vbo, vao, ebo;

    void loadModel(string &path);

    void loadPointsCloud(const Vector3f &translation, const Matrix3f &rotation);

    void loadOpenGLData();

    Matrix4f computeModelMatrix();
};

#endif
