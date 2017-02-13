#ifndef GL_PLY_MODEL_H
#define GL_PLY_MODEL_H

#ifdef Success
#undef Success
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Eigenvalues>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include "rendering/gl_utils.h"
#include <chrono>
#include <random>
#include "entities/points_cloud.h"
#include "rendering/gl_shader_program.h"
#include "rendering/gl_eigen.h"

using namespace std;
using namespace Eigen;

class glPlyModel {
public:
    glPlyModel(string path,
               const Vector3f &translation = Vector3f(0.0, 0.0, 0.0),
               const Matrix3f &rotation = Matrix3f::Identity(),
               const Vector4f &color = Vector4f(45.0f / 256.0, 137.0f / 256.0f,
                                                239.0f / 256.0, 1.0f));

    ~glPlyModel();

    void render(glShaderProgram &shader, Matrix4f &view,
                Matrix4f &projection);


    void printInformation();

    void setSelected(bool isSelected);

    void setColor(const Vector4f &color);

    void setWireframe(bool isWireframed);

    void setRenderNormals(bool renderNormals);

    bool getRenderNormals();

    void introduceGaussianNoise(float mean, float stdDev);

    void moveCentroidToOrigin();

    void setICPBase(bool isBase);
    bool getICPBase();

    unique_ptr<PointsCloud> m_pointsCloud;

    GLuint getVertexArrayId();

    Matrix4f computeModelMatrix();

private:
    Vector4f m_color;
    Vector4f m_colorNormal;
    vector<Vector3f> m_vertices;
    vector<GLfloat> glVertices2;
    vector<GLuint> glFaces2;
    // each entry corresponds to one vertex in m_vertices
    // in each entry indices of neighbors are stored
    vector<std::set<int>> m_neighbors;
    vector<Vector3f> m_normals;
    vector<GLfloat> m_glNormals;
    vector<GLfloat> m_glNormalsLinesData;
    int m_isSelected = 0;
    string path;
    int numberOfVertices = -1;
    int numberOfFaces = -1;
    bool numberOfVerticesFound = false;
    bool numberOfFacesFound = false;
    bool modelLoaded;
    bool m_isWireframed = true;
    bool m_renderNormals = false;
    bool m_isICPBased = false;
    float m_scale = 1.0;
    GLuint vbo, vao, ebo, vbo_normals, vao_normals;

    void loadModel(string &path);

    void approximateNormals();

    void loadPointsCloud(const Vector3f &translation, const Matrix3f &rotation);

    void assignNeighbors(int n1, int n2, int n3);

    void loadOpenGLData();

};

#endif
