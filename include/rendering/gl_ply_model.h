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

using namespace std;
using namespace Eigen;

class glPlyModel {
public:
    glPlyModel(string path);

    ~glPlyModel();

    void render(glShaderProgram &shader, Matrix4f &view, Matrix4f & projection);

    void loadModel(string &path);

    void printInformation(bool listData = false);

private:
    vector<Vector3f> vertices;
    vector<Vector3f> faces;
    GLfloat *glVertices = NULL;
    GLuint *glFaces = NULL;
    string path;
    int numberOfVertices = -1;
    int numberOfFaces = -1;
    bool numberOfVerticesFound = false;
    bool numberOfFacesFound = false;
    bool modelLoaded;
    GLuint vbo, vao, ebo;
    Matrix4f model = Matrix4f::Identity();

    void loadOpenGLData();
};

#endif
