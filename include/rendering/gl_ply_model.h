#ifndef GL_PLY_MODEL_H
#define GL_PLY_MODEL_H

#ifdef Success
#undef Success
#endif

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

    void render(glShaderProgram &shader);

    void loadModel(string& path);
    void printInformation(bool listData=false);
private:
    vector<Vector3f> vertices;
    vector<Vector3f> faces;
    string path;
    int numberOfVertices = -1;
    int numberOfFaces = -1;
    bool numberOfVerticesFound = false;
    bool numberOfFacesFound = false;
    bool modelLoaded;
};

#endif
