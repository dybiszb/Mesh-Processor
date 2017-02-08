// author: dybisz

#include <rendering/gl_utils.h>
#include "rendering/gl_ply_model.h"

//-----------------------------------------------------------------------------
glPlyModel::glPlyModel(string path,
                       const Vector3f &translation,
                       const Matrix3f &rotation)
        : path(path),
          modelLoaded(false),
          m_rotation(rotation),
          m_translation(translation),
          scale(1.0) {
    this->loadModel(path);
    this->printInformation();
    this->loadOpenGLData();

}

//-----------------------------------------------------------------------------
glPlyModel::~glPlyModel() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

//------------------------------------------------------------------------------
void
glPlyModel::render(glShaderProgram &shader, Matrix4f &view,
                   Matrix4f &projection) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shader.use();

    glBindVertexArray(vao);

    Matrix4f modelMatrix = computeModelMatrix();

    glUniform1i(glGetUniformLocation(shader.getId(), "isSelected"),
                                     m_isSelected);
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1,
                       GL_FALSE, modelMatrix.data());
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "view"), 1,
                       GL_FALSE, view.data());
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1,
                       GL_FALSE, projection.data());

    glDrawElements(GL_TRIANGLES,
                   (GLsizei) (glFaces2.size()) /*3*numberOfFaces*/,
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
    glCheckForErrors();
    shader.unuse();
}

//------------------------------------------------------------------------------
void
glPlyModel::accumulateRotation(const Matrix3f& rotation) {
    Matrix3f temp = m_rotation;
    m_rotation = m_rotation * rotation;
}

//------------------------------------------------------------------------------
void
glPlyModel::accumulateTranslation(const Vector3f& translation) {
    m_translation += translation;
}

//------------------------------------------------------------------------------
void
glPlyModel::setScale(float scale) {
    this->scale = scale;
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
                vertices.push_back(vertex);

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
                    // Standard Triangle
                    //
                    lineStream >> i1 >> i2 >> i3;
                    glFaces2.push_back(i1);
                    glFaces2.push_back(i2);
                    glFaces2.push_back(i3);

                } else if (type == 4) {
                    //
                    // Quad
                    //
                    lineStream >> i1 >> i2 >> i3 >> i4;
                    glFaces2.push_back(i1);
                    glFaces2.push_back(i2);
                    glFaces2.push_back(i3);

                    glFaces2.push_back(i4);
                    glFaces2.push_back(i1);
                    glFaces2.push_back(i3);
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

//-----------------------------------------------------------------------------
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glCheckForErrors();
    glBindVertexArray(0);

    glCheckForErrors();
}

//------------------------------------------------------------------------------
Matrix4f
glPlyModel::computeModelMatrix() {
    Matrix4f model = Matrix4f::Identity();

    // Rotation
    model.block(0,0,3,3) = m_rotation;

    // Translation
    model(0, 3) = m_translation(0);
    model(1, 3) = m_translation(1);
    model(2, 3) = m_translation(2);

    return model;
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

void
glPlyModel::setSelected(bool isSelected) {
    m_isSelected = (isSelected) ? 1 : 0;
}

//------------------------------------------------------------------------------
vector<Vector3f>
glPlyModel::getVertices() {
    vector<Vector3f> transformedVertices;
    Matrix4f modelMatrix = computeModelMatrix();

    for (int i = 0; i < numberOfVertices; ++i) {
        // Extend dimensions to homogeneous in order to include translation
        // from model matrix
        Vector4f homoVector(vertices[i](0),
                            vertices[i](1),
                            vertices[i](2),
                            1.0);

        Vector4f transformedVertex = modelMatrix * homoVector;

        Vector3f outputVertex(transformedVertex(0),
                              transformedVertex(1),
                              transformedVertex(2));

        transformedVertices.push_back(outputVertex);
    }

    return transformedVertices;
}