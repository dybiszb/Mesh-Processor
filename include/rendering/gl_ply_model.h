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
#include <chrono>
#include <random>
#include "logic/typedefs.h"
#include "entities/points_cloud.h"
#include "rendering/gl_shader_program.h"
#include "rendering/gl_eigen.h"
#include "rendering/gl_utils.h"

class glPlyModel {
public:
    glPlyModel(std::string path,
               const vec3 &translation = vec3(0.0, 0.0, 0.0),
               const mat3 &rotation = mat3::Identity(),
               const vec4 &color = vec4(45.0f / 256.0, 137.0f / 256.0f,
                                                239.0f / 256.0, 1.0f));

    ~glPlyModel();

    void render(glShaderProgram &shader, mat4 &view,
                mat4 &projection, glShaderProgram* normalsViz = NULL);

    void printInformation();

    void resetToInitialTransform();

    void introduceGaussianNoise(float mean, float stdDev);

    void moveCentroidToOrigin();

    std::unique_ptr<PointsCloud> m_pointsCloud;

    mat4 computeModelMatrix();

    void setICPBase(bool isBase);
    void setSelected(bool isSelected);
    void setColor(const vec4 &color);
    void setWireframe(bool isWireframed);
    void setRenderNormals(bool renderNormals);
    void setShading(bool shading);

    bool getWireframe();
    bool getShading();
    GLuint getVertexArrayId();
    bool getICPBase();
    bool getRenderNormals();

private:
    std::vector<std::pair<float, float>> __m_minMaxBB;
    vec4                       __m_color;
    vec4                       __m_colorNormal;
    std::vector<vec3>           __m_vertices;
    std::vector<GLfloat>            __m_glVertices2;
    std::vector<GLuint>             __m_glFaces2;
    std::vector<std::set<int>>      __m_neighbors;
    std::vector<vec3>           __m_normals;
    std::vector<GLfloat>            __m_glNormals;
    std::vector<GLfloat>            __m_glNormalsLinesData;
    int m_isSelected = 0;
    std::string                     __m_path;
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
    vec3                   __m_initialT;
    mat3                   __m_InitialR;

    void loadModel(std::string &path);

    void approximateNormals();

    void loadPointsCloud(const vec3 &translation, const mat3 &rotation);

    void assignNeighbors(int n1, int n2, int n3);

    void loadOpenGLData();

};

#endif
