// author: dybisz


#include "rendering/gl_ply_model.h"

//-----------------------------------------------------------------------------
glPlyModel::glPlyModel(string path,
                       const Vector3f &translation,
                       const Matrix3f &rotation,
                       const Vector4f &color)
        : path(path),
          modelLoaded(false),
          m_color(color) {
    this->loadModel(path);
    this->approximateNormals();
    this->loadPointsCloud(translation, rotation);
    this->loadOpenGLData();
    m_colorNormal = Vector4f(1.0f, 0.0f, 151.0f / 256.0f, 1.0f);
}

//-----------------------------------------------------------------------------
glPlyModel::~glPlyModel() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

//------------------------------------------------------------------------------
GLuint
glPlyModel::getVertexArrayId() {
    return vbo;
}

//------------------------------------------------------------------------------
void
glPlyModel::render(glShaderProgram &shader, Matrix4f &view, Matrix4f
&projection, glShaderProgram* normalsViz) {
    if(m_isWireframed) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShaderProgram* currShdr;
    if(__m_shading && normalsViz != NULL && m_glNormalsLinesData.size()) {
        currShdr = normalsViz;
        glBindVertexArray(vaoShading);
    } else {
        currShdr = &shader;
        glBindVertexArray(vao);
    }
        currShdr->use();

        Matrix4f modelMatrix = computeModelMatrix();

        glUniform1i(glGetUniformLocation(currShdr->getId(), "u_isSelected"),
                    m_isSelected);
        glCheckForErrors();
        glUniform4fv(glGetUniformLocation(currShdr->getId(), "u_color"),
                     1,
                     m_color.data());

        glUniformMatrix4fv(glGetUniformLocation(currShdr->getId(), "u_model"), 1,
                           GL_FALSE, modelMatrix.data());

        glUniformMatrix4fv(glGetUniformLocation(currShdr->getId(), "u_view"), 1,
                           GL_FALSE, view.data());
        glCheckForErrors();
        glUniformMatrix4fv(glGetUniformLocation(currShdr->getId(), "u_projection"), 1,
                           GL_FALSE, projection.data());
        glCheckForErrors();
        glDrawElements(GL_TRIANGLES,
                       (GLsizei) (glFaces2.size()) /*3*numberOfFaces*/,
                       GL_UNSIGNED_INT,
                       0);
        //Clean

    currShdr->unuse();
    currShdr = NULL;
    glBindVertexArray(0);
    //////////////////////////////////////////////////
    // Render Normals
    //////////////////////////////////////////////////
    if(m_renderNormals) {
        shader.use();
        glBindVertexArray(vao_normals);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glUniform1i(glGetUniformLocation(shader.getId(), "u_isSelected"),
                    false);
        glUniform4fv(glGetUniformLocation(shader.getId(), "u_color"),
                     1,
                     m_colorNormal.data());

        Matrix4f modelMatrixNormal = computeModelMatrix();


        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_model"), 1,
                           GL_FALSE, modelMatrixNormal.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_view"), 1,
                           GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_projection"), 1,
                           GL_FALSE, projection.data());
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINES, 0, (GLsizei) m_glNormalsLinesData.size());

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
glPlyModel::loadModel(string &path) {
    if (!modelLoaded) {


        // Open File
        ifstream file(path, std::ifstream::in);
        if (file.fail()) {
            cout << "Failed to load: " << path << endl;
        }

        // Parse File
        int verticesCount = -1;
        int facesCount = -1;
        bool headerEnd = false;
        while (!file.eof()) {

            std::string line;
            std::getline(file, line);

            // Number of vertices and faces unknown
            if (!numberOfVerticesFound || !numberOfFacesFound) {

                // Look for number of vertices
                std::size_t foundVertices = line.find("element vertex");
                if (foundVertices != string::npos) {
                    stringstream lineStream(line);
                    string _;
                    lineStream >> _ >> _ >> numberOfVertices;

                    verticesCount = numberOfVertices;
                    numberOfVerticesFound = true;
                }

                // Look for number of faces
                std::size_t foundFaces = line.find("element face");
                if (foundFaces != string::npos) {
                    stringstream lineStream(line);
                    string _;
                    lineStream >> _ >> _ >> numberOfFaces;

                    facesCount = numberOfFaces;
                    numberOfFacesFound = true;
                }


            } else if (!headerEnd) {
                // Look for header end
                std::size_t found = line.find("end_header");
                if (found != string::npos) headerEnd = true;

            } else if (verticesCount > 0) {
                // Load Vertices
                stringstream lineStream(line);
                verticesCount--;
                float x, y, z;
                lineStream >> x >> y >> z;
                Vector3f vertex(x, y, z);
                m_vertices.push_back(vertex);

                glVertices2.push_back(x);
                glVertices2.push_back(y);
                glVertices2.push_back(z);

            } else if (facesCount > 0) {
                // Load Faces
                stringstream lineStream(line);
                facesCount--;
                unsigned int type, i1, i2, i3, i4;
                lineStream >> type;

                if (type == 3) {
                    //
                    // Triangle
                    //
                    lineStream >> i1 >> i2 >> i3;
                    glFaces2.push_back(i1);
                    glFaces2.push_back(i2);
                    glFaces2.push_back(i3);
                    assignNeighbors(i1, i2, i3);

                } else if (type == 4) {
                    //
                    // Quad
                    //
                    lineStream >> i1 >> i2 >> i3 >> i4;
                    glFaces2.push_back(i1);
                    glFaces2.push_back(i2);
                    glFaces2.push_back(i3);
                    assignNeighbors(i1, i2, i3);

                    glFaces2.push_back(i4);
                    glFaces2.push_back(i1);
                    glFaces2.push_back(i3);
                    assignNeighbors(i4, i1, i3);
                }


            } else break;

        }

        // Close File
        file.close();
        modelLoaded = true;
    } else {
        cout << "Model from path: " << path << " already loaded." << endl;
    }
}

//------------------------------------------------------------------------------
void
glPlyModel::approximateNormals() {
    if(m_neighbors.size() <= 0) {
        cout << "Vector m_niegbors empty\n";
        return;
    }

    if(m_normals.size() != 0 || m_glNormals.size() != 0) {
        cout << "Vector m_normals or m_glNormals not empty\n";
        return;
    }
    if(m_glNormalsLinesData.size() != 0) {
        cout << "Vector m_glNormalsLinesData not empty\n";
        return;
    }

    for(int i = 0; i < m_vertices.size(); ++i) {
        // Calculate covariance matrix
        Matrix3f cov(Matrix3f::Zero());
        const set<int>& neigh = m_neighbors[i];

        for(const auto neighbor : neigh) {
            if(i == neighbor) continue;

            Vector3f const toNeighbor = m_vertices[neighbor] - m_vertices[i];
            cov += toNeighbor * toNeighbor.transpose();
        }

        // Get normal from Eigens
        SelfAdjointEigenSolver<Matrix3f> es(cov);

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
        Matrix3f temp = es.eigenvectors();
        Vector3f normal = temp.col(smallestEigenValueId).normalized();
        // Save normal in m_ and m_gl array
        m_normals.push_back(normal);
        float x = normal(0);
        float y = normal(1);
        float z = normal(2);
        m_glNormals.push_back(x);
        m_glNormals.push_back(y);
        m_glNormals.push_back(z);
    }

    // TODO: I hope this is a temp array
    for(int i = 0; i < m_vertices.size(); ++i) {
        float ver_x = m_vertices[i](0);
        float ver_y = m_vertices[i](1);
        float ver_z = m_vertices[i](2);

        m_glNormalsLinesData.push_back(ver_x);
        m_glNormalsLinesData.push_back(ver_y);
        m_glNormalsLinesData.push_back(ver_z);

        float norm_x = ver_x + m_normals[i](0) / 100.0f;
        float norm_y = ver_y + m_normals[i](1) / 100.0f;
        float norm_z = ver_z + m_normals[i](2) / 100.0f;

        m_glNormalsLinesData.push_back(norm_x);
        m_glNormalsLinesData.push_back(norm_y);
        m_glNormalsLinesData.push_back(norm_z);
    }

}

//------------------------------------------------------------------------------
void
glPlyModel::loadPointsCloud(const Vector3f &translation,
                            const Matrix3f &rotation) {
    if (m_vertices.size()) {
        m_pointsCloud = unique_ptr<PointsCloud>(new PointsCloud(m_vertices,
                                                                translation,
                                                                rotation));
    } else cout << "No vertices loaded to cloud\n";
}

//------------------------------------------------------------------------------
void
glPlyModel::assignNeighbors(int n1, int n2, int n3) {
    // Check if vector has enough entries to store n1
    if(m_neighbors.size() <= n1) {
        for(int i = (int) m_neighbors.size(); i < n1+1; ++i) {
            set<int> temp;
            m_neighbors.push_back(temp);
        }
    }
    // Assign n1 with n2, n3
    m_neighbors[n1].insert(n2);
    m_neighbors[n1].insert(n3);

    // Check if vector has enough entries to store n2
    if(m_neighbors.size() <= n2) {
        for(int i = (int) m_neighbors.size(); i < n2+1; ++i) {
            set<int> temp;
            m_neighbors.push_back(temp);
        }
    }
    // Assign n2 with n1, n3
    m_neighbors[n2].insert(n1);
    m_neighbors[n2].insert(n3);

    // Check if vector has enough entries to store n3
    if(m_neighbors.size() <= n3) {
        for(int i = (int) m_neighbors.size(); i < n3+1; ++i) {
            set<int> temp;
            m_neighbors.push_back(temp);
        }
    }
    // Assign n3 with n1, n2
    m_neighbors[n3].insert(n1);
    m_neighbors[n3].insert(n2);
}

//------------------------------------------------------------------------------
void
glPlyModel::loadOpenGLData() {
    // Init Buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glCheckForErrors();

    // Init Vertices/Indices
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 glVertices2.size() * sizeof(GLfloat),
                 &glVertices2[0],
                 GL_STATIC_DRAW);
    glCheckForErrors();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 glFaces2.size() * sizeof(GLuint),
                 &glFaces2[0],
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
    glGenVertexArrays(1, &vaoShading);
    glGenBuffers(1, &vboshading);
    glGenBuffers(1, &eboShading);
    glCheckForErrors();

    // Init Vertices/Indices
    glBindVertexArray(vaoShading);
    glBindBuffer(GL_ARRAY_BUFFER, vboshading);
    glBufferData(GL_ARRAY_BUFFER,
                 m_glNormalsLinesData.size() * sizeof(GLfloat),
                 &m_glNormalsLinesData[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glCheckForErrors();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboShading);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 glFaces2.size() * sizeof(GLuint),
                 &glFaces2[0],
                 GL_STATIC_DRAW);
    glCheckForErrors();

    //Clean
    glBindVertexArray(0);

    glCheckForErrors();

    /////////////////////////////////////////
    // Normals
    glGenVertexArrays(1, &vao_normals);
    glGenBuffers(1, &vbo_normals);

    glBindVertexArray(vao_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);

    glBufferData(GL_ARRAY_BUFFER,
                 m_glNormalsLinesData.size() * sizeof(GLfloat),
                 &m_glNormalsLinesData[0],
                 GL_STATIC_DRAW);
    glCheckForErrors();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    //Clean
    glBindVertexArray(0);
}

//------------------------------------------------------------------------------
Matrix4f
glPlyModel::computeModelMatrix() {
    Matrix4f scaleTranslation = Matrix4f::Identity();
    Matrix4f rotation = Matrix4f::Identity();

    /* ----- Scale + Translate ----- */
    Vector3f scale = m_pointsCloud->getScale();
    scaleTranslation(0, 0) = scale(0);
    scaleTranslation(1, 1) = scale(1);
    scaleTranslation(2, 2) = scale(2);

    Vector3f t = m_pointsCloud->getTranslation();
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

    cout << "Model Path:  " << this->path << endl;
    if (numberOfVerticesFound)
        cout << "# vertices: " << this->numberOfVertices << endl;
    else
        cout << "# vertices: " << "Not Found" << endl;

    if (numberOfFacesFound)
        cout << "# faces     " << this->numberOfFaces << endl;
    else
        cout << "# faces     " << "Not Found" << endl;
}

//------------------------------------------------------------------------------
void
glPlyModel::setSelected(bool isSelected) {
    m_isSelected = (isSelected) ? 1 : 0;
}

//------------------------------------------------------------------------------
void
glPlyModel::setColor(const Vector4f &color) {
    m_color = color;
}

//------------------------------------------------------------------------------
void
glPlyModel::setWireframe(bool isWireframed) {
    m_isWireframed = isWireframed;
}

//------------------------------------------------------------------------------
void
glPlyModel::setRenderNormals(bool renderNormals) {
    m_renderNormals = renderNormals;
}

//------------------------------------------------------------------------------
bool
glPlyModel::getRenderNormals() {
    return m_renderNormals;
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
bool
glPlyModel::getWireframe() {
    return m_isWireframed;
}


//------------------------------------------------------------------------------
void
glPlyModel::introduceGaussianNoise(float mean, float stdDev) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::normal_distribution<double> distribution (mean, stdDev);

    for(int i = 0; i < m_vertices.size(); ++i) {
        // update m_vertices
        // update m_glVertice

        float x = (float) distribution(generator);
        float y = (float) distribution(generator);
        float z = (float) distribution(generator);

        m_vertices[i](0) += x;
        m_vertices[i](1) += y;
        m_vertices[i](2) += z;

        glVertices2[3 * i + 0] += x;
        glVertices2[3 * i + 1] += y;
        glVertices2[3 * i + 2] += z;
    }

    // update PointsCloud
    m_pointsCloud->setVertices(m_vertices);

    // Clear current normals
    m_normals.clear();
    m_glNormals.clear();
    m_glNormalsLinesData.clear();

    // Reload Normals
    this->approximateNormals();
    this->loadOpenGLData();
}

//------------------------------------------------------------------------------
void
glPlyModel::moveCentroidToOrigin() {
    Vector3f centroid = m_pointsCloud->getCentroidFromUpdatedVertices();
    centroid = -centroid;
    m_pointsCloud->accumulateTranslation(centroid);
}


//------------------------------------------------------------------------------
void
glPlyModel::setICPBase(bool isBase) {
    m_isICPBased = isBase;
}

//------------------------------------------------------------------------------
bool
glPlyModel::getICPBase() {
    return m_isICPBased;
}