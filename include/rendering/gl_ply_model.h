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
                Matrix4f &projection, glShaderProgram* normalsViz = NULL);

    void printInformation();

    void resetToInitialTransform();

    void introduceGaussianNoise(float mean, float stdDev);

    void moveCentroidToOrigin();

    unique_ptr<PointsCloud> m_pointsCloud;

    Matrix4f computeModelMatrix();

    void setICPBase(bool isBase);
    void setSelected(bool isSelected);
    void setColor(const Vector4f &color);
    void setWireframe(bool isWireframed);
    void setRenderNormals(bool renderNormals);
    void setShading(bool shading);

    bool getWireframe();
    bool getShading();
    GLuint getVertexArrayId();
    bool getICPBase();
    bool getRenderNormals();

private:
    vector<pair<float, float>> __m_minMaxBB;
    Vector4f                   __m_color;
    Vector4f                   __m_colorNormal;
    vector<Vector3f>           __m_vertices;
    vector<GLfloat>            __m_glVertices2;
    vector<GLuint>             __m_glFaces2;
    vector<std::set<int>>      __m_neighbors;
    vector<Vector3f>           __m_normals;
    vector<GLfloat>            __m_glNormals;
    vector<GLfloat>            __m_glNormalsLinesData;
    int m_isSelected = 0;
    string                     __m_path;
    int                        __m_numberOfVertices = -1;
    int                        __m_numberOfFaces = -1;
    bool                       __m_numberOfVerticesFound = false;
    bool                       __m_numberOfFacesFound = false;
    bool                       __m_modelLoaded;
    bool                       __m_isWireframed = true;
    bool                       __m_renderNormals = false;
    bool                       __m_isICPBased = false;
    bool                       __m_shading = false;
    float m_scale = 1.0;
    GLuint                     __m_vbo, __m_vao, __m_ebo;
    GLuint                     __m_vbo_normals, __m_vao_normals;
    GLuint                     __m_vaoShading, __m_eboShading, __m_vboshading;
    Vector3f                   __m_initialT;
    Matrix3f                   __m_InitialR;

    void loadModel(string &path);

    void approximateNormals();

    void loadPointsCloud(const Vector3f &translation, const Matrix3f &rotation);

    void assignNeighbors(int n1, int n2, int n3);

    void loadOpenGLData();

};

#endif
