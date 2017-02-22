// author: dybisz


#include "rendering/gl_ply_model.h"

//-----------------------------------------------------------------------------
glPlyModel::glPlyModel(std::string path,
                       const vec3 &translation,
                       const mat3 &rotation,
                       const vec4 &color)
        : __m_path(path),
          __m_modelLoaded(false),
          __m_color(color),
          __m_initialT(translation),
          __m_InitialR(rotation) {
    this->loadModel(path);
    this->approximateNormals();
    this->loadPointsCloud(translation, rotation);
    this->loadOpenGLData();
    __m_colorNormal = vec4(1.0f, 0.0f, 151.0f / 256.0f, 1.0f);
}

//-----------------------------------------------------------------------------
glPlyModel::~glPlyModel() {
    glDeleteVertexArrays(1, &__m_vao);
    glDeleteBuffers(1, &__m_vbo);
    glDeleteBuffers(1, &__m_ebo);
}

//------------------------------------------------------------------------------
GLuint
glPlyModel::getVertexArrayId() {
    return __m_vbo;
}

//------------------------------------------------------------------------------
void
glPlyModel::render(glShaderProgram &shader, mat4 &view, mat4 &projection,
                   glShaderProgram* normalsViz) {
    if(__m_isWireframed) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShaderProgram* currShdr;
    if(__m_shading && normalsViz != NULL && __m_glNormalsLinesData.size()) {
        currShdr = normalsViz;
        glBindVertexArray(__m_vaoShading);
    } else {
        currShdr = &shader;
        glBindVertexArray(__m_vao);
    }
        currShdr->use();

        mat4 modelMatrix = computeModelMatrix();

        glUniform1i(glGetUniformLocation(currShdr->getId(), "u_isSelected"),
                    m_isSelected);
        glCheckForErrors();
        glUniform4fv(glGetUniformLocation(currShdr->getId(), "u_color"),
                     1,
                     __m_color.data());

        glUniformMatrix4fv(glGetUniformLocation(currShdr->getId(), "u_model"), 1,
                           GL_FALSE, modelMatrix.data());

        glUniformMatrix4fv(glGetUniformLocation(currShdr->getId(), "u_view"), 1,
                           GL_FALSE, view.data());
        glCheckForErrors();
        glUniformMatrix4fv(glGetUniformLocation(currShdr->getId(), "u_projection"), 1,
                           GL_FALSE, projection.data());
        glCheckForErrors();
        glDrawElements(GL_TRIANGLES,
                       (GLsizei) (__m_glFaces2.size()) /*3*numberOfFaces*/,
                       GL_UNSIGNED_INT,
                       0);
        //Clean

    currShdr->unuse();
    currShdr = NULL;
    glBindVertexArray(0);
    //////////////////////////////////////////////////
    // Render Normals
    //////////////////////////////////////////////////
    if(__m_renderNormals) {
        shader.use();
        glBindVertexArray(__m_vao_normals);
        glBindBuffer(GL_ARRAY_BUFFER, __m_vbo_normals);
        glUniform1i(glGetUniformLocation(shader.getId(), "u_isSelected"),
                    false);
        glUniform4fv(glGetUniformLocation(shader.getId(), "u_color"),
                     1,
                     __m_colorNormal.data());

        mat4 modelMatrixNormal = computeModelMatrix();


        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_model"), 1,
                           GL_FALSE, modelMatrixNormal.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_view"), 1,
                           GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_projection"), 1,
                           GL_FALSE, projection.data());
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINES, 0, (GLsizei) __m_glNormalsLinesData.size());

        //Clean
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        glCheckForErrors();
        shader.unuse();
    }

}

//-----------------------------------------------------------------------------
void
glPlyModel::loadModel(std::string &path) {
    if (!__m_modelLoaded) {


        // Open File
        std::ifstream file(path, std::ifstream::in);
        if (file.fail()) {
            std::cout << "Failed to load: " << path << std::endl;
        }

        // Parse File
        int verticesCount = -1;
        int facesCount = -1;
        bool headerEnd = false;
        std::pair<float, float> minMaxX(INT8_MAX, -INT8_MAX);
        std::pair<float, float> minMaxY(INT8_MAX, -INT8_MAX);
        std::pair<float, float> minMaxZ(INT8_MAX, -INT8_MAX);

        while (!file.eof()) {

            std::string line;
            std::getline(file, line);

            // Number of vertices and faces unknown
            if (!__m_numberOfVerticesFound || !__m_numberOfFacesFound) {

                // Look for number of vertices
                std::size_t foundVertices = line.find("element vertex");
                if (foundVertices != std::string::npos) {
                    std::stringstream lineStream(line);
                    std::string _;
                    lineStream >> _ >> _ >> __m_numberOfVertices;

                    verticesCount = __m_numberOfVertices;
                    __m_numberOfVerticesFound = true;
                }

                // Look for number of faces
                std::size_t foundFaces = line.find("element face");
                if (foundFaces != std::string::npos) {
                    std::stringstream lineStream(line);
                    std::string _;
                    lineStream >> _ >> _ >> __m_numberOfFaces;

                    facesCount = __m_numberOfFaces;
                    __m_numberOfFacesFound = true;
                }


            } else if (!headerEnd) {
                // Look for header end
                std::size_t found = line.find("end_header");
                if (found != std::string::npos) headerEnd = true;

            } else if (verticesCount > 0) {
                // Load Vertices
                std::stringstream lineStream(line);
                verticesCount--;
                float x, y, z;
                lineStream >> x >> y >> z;
                vec3 vertex(x, y, z);
                __m_vertices.push_back(vertex);

                __m_glVertices2.push_back(x);
                __m_glVertices2.push_back(y);
                __m_glVertices2.push_back(z);

                // Store min.max
                if(minMaxX.first > x) minMaxX.first = x;
                if(minMaxX.second < x) minMaxX.second = x;
                if(minMaxY.first > y) minMaxY.first = y;
                if(minMaxY.second < y) minMaxY.second = y;
                if(minMaxZ.first > z) minMaxZ.first = z;
                if(minMaxZ.second < z) minMaxZ.second = z;

            } else if (facesCount > 0) {
                // Load Faces
                std::stringstream lineStream(line);
                facesCount--;
                unsigned int type, i1, i2, i3, i4;
                lineStream >> type;

                if (type == 3) {
                    //
                    // Triangle
                    //
                    lineStream >> i1 >> i2 >> i3;
                    __m_glFaces2.push_back(i1);
                    __m_glFaces2.push_back(i2);
                    __m_glFaces2.push_back(i3);
                    assignNeighbors(i1, i2, i3);

                } else if (type == 4) {
                    //
                    // Quad
                    //
                    lineStream >> i1 >> i2 >> i3 >> i4;
                    __m_glFaces2.push_back(i1);
                    __m_glFaces2.push_back(i2);
                    __m_glFaces2.push_back(i3);
                    assignNeighbors(i1, i2, i3);

                    __m_glFaces2.push_back(i4);
                    __m_glFaces2.push_back(i1);
                    __m_glFaces2.push_back(i3);
                    assignNeighbors(i4, i1, i3);
                }


            } else break;

        }

        // Close File
        file.close();
        __m_minMaxBB.push_back(minMaxX);
        __m_minMaxBB.push_back(minMaxY);
        __m_minMaxBB.push_back(minMaxZ);

        __m_modelLoaded = true;
    } else {
        std::cout << "Model from path: " << path << " already loaded." << std::endl;
    }
}

//------------------------------------------------------------------------------
void
glPlyModel::approximateNormals() {
    if(__m_neighbors.size() <= 0) {
        std::cout << "Vector m_niegbors empty\n";
        return;
    }

    if(__m_normals.size() != 0 ||__m_glNormals.size() != 0) {
        std::cout << "Vector m_normals or m_glNormals not empty\n";
        return;
    }
    if(__m_glNormalsLinesData.size() != 0) {
        std::cout << "Vector m_glNormalsLinesData not empty\n";
        return;
    }

    for(int i = 0; i < __m_vertices.size(); ++i) {
        // Calculate covariance matrix
        mat3 cov(mat3::Zero());
        const std::set<int>& neigh = __m_neighbors[i];

        for(const auto neighbor : neigh) {
            if(i == neighbor) continue;

            vec3 const toNeighbor = __m_vertices[neighbor] - __m_vertices[i];
            cov += toNeighbor * toNeighbor.transpose();
        }

        // Get normal from Eigens
        Eigen::SelfAdjointEigenSolver<mat3> es(cov);

        int const smallestEigenValueId = // 0, 1 2
                static_cast<int>(
                        std::distance(
                                es.eigenvalues().data(),
                                std::min_element(
                                        es.eigenvalues().data(),
                                        es.eigenvalues().data() + 3
                                )
                        )
                );
        mat3 temp = es.eigenvectors();
        vec3 normal = temp.col(smallestEigenValueId).normalized();
        // Save normal in m_ and m_gl array
        __m_normals.push_back(normal);
        float x = normal(0);
        float y = normal(1);
        float z = normal(2);
        __m_glNormals.push_back(x);
        __m_glNormals.push_back(y);
        __m_glNormals.push_back(z);
    }

    for(int i = 0; i < __m_vertices.size(); ++i) {
        float ver_x = __m_vertices[i](0);
        float ver_y = __m_vertices[i](1);
        float ver_z = __m_vertices[i](2);

        __m_glNormalsLinesData.push_back(ver_x);
        __m_glNormalsLinesData.push_back(ver_y);
        __m_glNormalsLinesData.push_back(ver_z);

        float norm_x = ver_x + __m_normals[i](0) / 100.0f;
        float norm_y = ver_y + __m_normals[i](1) / 100.0f;
        float norm_z = ver_z + __m_normals[i](2) / 100.0f;

        __m_glNormalsLinesData.push_back(norm_x);
        __m_glNormalsLinesData.push_back(norm_y);
        __m_glNormalsLinesData.push_back(norm_z);
    }

}

//------------------------------------------------------------------------------
void
glPlyModel::loadPointsCloud(const vec3 &translation,
                            const mat3 &rotation) {
    if (__m_vertices.size()) {
        m_pointsCloud = std::unique_ptr<PointsCloud>(new PointsCloud(__m_vertices,
                                                                translation,
                                                                rotation));
    } else std::cout << "No vertices loaded to cloud\n";
}

//------------------------------------------------------------------------------
void
glPlyModel::assignNeighbors(int n1, int n2, int n3) {
    // Check if vector has enough entries to store n1
    if(__m_neighbors.size() <= n1) {
        for(int i = (int) __m_neighbors.size(); i < n1+1; ++i) {
            std::set<int> temp;
            __m_neighbors.push_back(temp);
        }
    }
    // Assign n1 with n2, n3
    __m_neighbors[n1].insert(n2);
    __m_neighbors[n1].insert(n3);

    // Check if vector has enough entries to store n2
    if(__m_neighbors.size() <= n2) {
        for(int i = (int) __m_neighbors.size(); i < n2+1; ++i) {
            std::set<int> temp;
            __m_neighbors.push_back(temp);
        }
    }
    // Assign n2 with n1, n3
    __m_neighbors[n2].insert(n1);
    __m_neighbors[n2].insert(n3);

    // Check if vector has enough entries to store n3
    if(__m_neighbors.size() <= n3) {
        for(int i = (int) __m_neighbors.size(); i < n3+1; ++i) {
            std::set<int> temp;
            __m_neighbors.push_back(temp);
        }
    }
    // Assign n3 with n1, n2
    __m_neighbors[n3].insert(n1);
    __m_neighbors[n3].insert(n2);
}

//------------------------------------------------------------------------------
void
glPlyModel::loadOpenGLData() {
    // Init Buffers
    glGenVertexArrays(1, &__m_vao);
    glGenBuffers(1, &__m_vbo);
    glGenBuffers(1, &__m_ebo);
    glCheckForErrors();

    // Init Vertices/Indices
    glBindVertexArray(__m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, __m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 __m_glVertices2.size() * sizeof(GLfloat),
                 &__m_glVertices2[0],
                 GL_STATIC_DRAW);
    glCheckForErrors();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 __m_glFaces2.size() * sizeof(GLuint),
                 &__m_glFaces2[0],
                 GL_STATIC_DRAW);
    glCheckForErrors();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    //Clean

    glBindVertexArray(0);

    glCheckForErrors();

    //////////////////////////////////////
    // Shading
    //////////////////////////////////////
    // Init Buffers
    glGenVertexArrays(1, &__m_vaoShading);
    glGenBuffers(1, &__m_vboshading);
    glGenBuffers(1, &__m_eboShading);
    glCheckForErrors();

    // Init Vertices/Indices
    glBindVertexArray(__m_vaoShading);
    glBindBuffer(GL_ARRAY_BUFFER, __m_vboshading);
    glBufferData(GL_ARRAY_BUFFER, __m_glNormalsLinesData.size() * sizeof(GLfloat), &__m_glNormalsLinesData[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glCheckForErrors();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __m_eboShading);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 __m_glFaces2.size() * sizeof(GLuint),
                 &__m_glFaces2[0],
                 GL_STATIC_DRAW);
    glCheckForErrors();

    //Clean
    glBindVertexArray(0);

    glCheckForErrors();

    /////////////////////////////////////////
    // Normals
    glGenVertexArrays(1, &__m_vao_normals);
    glGenBuffers(1, &__m_vbo_normals);

    glBindVertexArray(__m_vao_normals);
    glBindBuffer(GL_ARRAY_BUFFER, __m_vbo_normals);

    glBufferData(GL_ARRAY_BUFFER,
                 __m_glNormalsLinesData.size() * sizeof(GLfloat),
                 &__m_glNormalsLinesData[0],
                 GL_STATIC_DRAW);
    glCheckForErrors();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    //Clean
    glBindVertexArray(0);
}


//------------------------------------------------------------------------------
mat4
glPlyModel::computeModelMatrix() {
    mat4 scaleTranslation = mat4::Identity();
    mat4 rotation = mat4::Identity();

    /* ----- Scale + Translate ----- */
    vec3 scale = m_pointsCloud->getScale();
    scaleTranslation(0, 0) = scale(0);
    scaleTranslation(1, 1) = scale(1);
    scaleTranslation(2, 2) = scale(2);

    vec3 t = m_pointsCloud->getTranslation();
    scaleTranslation(0, 3) = t(0);
    scaleTranslation(1, 3) = t(1);
    scaleTranslation(2, 3) = t(2);

    /* ----- Rotation ------ */
    rotation.block(0, 0, 3, 3) = m_pointsCloud->getRotation();

    return scaleTranslation * rotation;
}


//------------------------------------------------------------------------------
void
glPlyModel::printInformation() {

    std::cout << "Model Path:  " << this->__m_path << std::endl;
    if (__m_numberOfVerticesFound)
        std::cout << "# vertices: " << this->__m_numberOfVertices << std::endl;
    else
        std::cout << "# vertices: " << "Not Found" << std::endl;

    if (__m_numberOfFacesFound)
        std::cout << "# faces     " << this->__m_numberOfFaces << std::endl;
    else
        std::cout << "# faces     " << "Not Found" << std::endl;

    if(__m_minMaxBB.size()) {
        std::cout << "Bounding Box:" << std::endl;
        std::cout << "Min/max x: " << __m_minMaxBB[0].first
             << "/" << __m_minMaxBB[0].second << std::endl;

        std::cout << "Min/max y: " << __m_minMaxBB[1].first
             << "/" << __m_minMaxBB[1].second << std::endl;

        std::cout << "Min/max z: " << __m_minMaxBB[2].first
             << "/" << __m_minMaxBB[2].second << std::endl;
    }
}

//------------------------------------------------------------------------------
void
glPlyModel::setSelected(bool isSelected) {
    m_isSelected = (isSelected) ? 1 : 0;
}

//------------------------------------------------------------------------------
void
glPlyModel::setColor(const vec4 &color) {
    __m_color = color;
}

//------------------------------------------------------------------------------
void
glPlyModel::setWireframe(bool isWireframed) {
    __m_isWireframed = isWireframed;
}

//------------------------------------------------------------------------------
void
glPlyModel::setRenderNormals(bool renderNormals) {
    __m_renderNormals = renderNormals;
}

//------------------------------------------------------------------------------
bool
glPlyModel::getRenderNormals() {
    return __m_renderNormals;
}

//------------------------------------------------------------------------------
void
glPlyModel::setShading(bool shading) {
    __m_shading = shading;
}

//------------------------------------------------------------------------------
bool
glPlyModel::getShading() {
    return __m_shading;
}

//------------------------------------------------------------------------------
void
glPlyModel::resetToInitialTransform() {
    m_pointsCloud->setTranslation(__m_initialT);
    m_pointsCloud->setRotation(__m_InitialR);
}

//------------------------------------------------------------------------------
bool
glPlyModel::getWireframe() {
    return __m_isWireframed;
}


//------------------------------------------------------------------------------
void
glPlyModel::introduceGaussianNoise(float mean, float stdDev) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::normal_distribution<double> distribution (mean, stdDev);

    for(int i = 0; i < __m_vertices.size(); ++i) {
        // update m_vertices
        // update m_glVertice

        float x = (float) distribution(generator);
        float y = (float) distribution(generator);
        float z = (float) distribution(generator);

        __m_vertices[i](0) += x;
        __m_vertices[i](1) += y;
        __m_vertices[i](2) += z;

        __m_glVertices2[3 * i + 0] += x;
        __m_glVertices2[3 * i + 1] += y;
        __m_glVertices2[3 * i + 2] += z;
    }

    // update PointsCloud
    m_pointsCloud->setVertices(__m_vertices);

    // Clear current normals
    __m_normals.clear();
    __m_glNormals.clear();
    __m_glNormalsLinesData.clear();

    // Reload Normals
    this->approximateNormals();
    this->loadOpenGLData();
}

//------------------------------------------------------------------------------
void
glPlyModel::moveCentroidToOrigin() {
    vec3 centroid = m_pointsCloud->getCentroidFromUpdatedVertices();
    centroid = -centroid;
    m_pointsCloud->accumulateTranslation(centroid);
}


//------------------------------------------------------------------------------
void
glPlyModel::setICPBase(bool isBase) {
    __m_isICPBased = isBase;
}

//------------------------------------------------------------------------------
bool
glPlyModel::getICPBase() {
    return __m_isICPBased;
}