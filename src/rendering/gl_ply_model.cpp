// author: dybisz

#include <rendering/gl_utils.h>
#include "rendering/gl_ply_model.h"

//-----------------------------------------------------------------------------
glPlyModel::glPlyModel(string path, const Vector3f &translation)
        : path(path),
          modelLoaded(false),
          rotation(0.0, 0.0, 0.0),
          translation(translation),
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
glPlyModel::setRotation(float roll, float yaw, float pitch) {
    this->rotation(0) = roll;
    this->rotation(1) = yaw;
    this->rotation(2) = pitch;
}

//------------------------------------------------------------------------------
void
glPlyModel::setTranslation(float x, float y, float z) {
    this->translation(0) = x;
    this->translation(1) = y;
    this->translation(2) = z;
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
    // Create Rotation Matrix
    Affine3d rotation = createRotationMatrix(this->rotation);

    // Translation
    Matrix4f model = Matrix4f::Identity();
    model(0, 3) = translation(0);
    model(1, 3) = translation(1);
    model(2, 3) = translation(2);

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

//------------------------------------------------------------------------------
MatrixXd
glPlyModel::getMatrixOfPoints() {
//    Vector3f barCoords = getBarycentricCoordinate();
//    cout << "bar lol: " << barCoords << endl;
    MatrixXd pointsMatrix(numberOfVertices, 3);
    Matrix4f modelMatrix = computeModelMatrix();
    for (int i = 0; i < numberOfVertices; ++i) {
        // Extend dimensions to homogeneous in order to include translation
        // from model matrix
        Vector4f homoVector(vertices[i](0),
                            vertices[i](1),
                            vertices[i](2),
                            1.0);
        Vector4f transformedVertex = modelMatrix * homoVector;
        pointsMatrix(i, 0) = transformedVertex(0);
        pointsMatrix(i, 1) = transformedVertex(1);
        pointsMatrix(i, 2) = transformedVertex(2);
    }
    return pointsMatrix;
}

//------------------------------------------------------------------------------
MatrixXd
glPlyModel::getMatrixOfTyldaCoordinates() {
    Vector3f barCoords = getBarycentricCoordinate();
    Matrix4f modelMatrix = computeModelMatrix();
    MatrixXd pointsMatrix(numberOfVertices, 3);

    for (int i = 0; i < numberOfVertices; ++i) {
        // Extend dimensions to homogeneous in order to include translation
        // from model matrix
        Vector4f homoVector(vertices[i](0),
                            vertices[i](1),
                            vertices[i](2),
                            1.0);
        Vector4f transformedVertex = modelMatrix * homoVector;

        pointsMatrix(i, 0) = transformedVertex(0) - barCoords(0);
        pointsMatrix(i, 1) = transformedVertex(1) - barCoords(1);
        pointsMatrix(i, 2) = transformedVertex(2) - barCoords(2);
    }
    return pointsMatrix;
}

//------------------------------------------------------------------------------
Vector3f
glPlyModel::getBarycentricCoordinate() {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    Matrix4f modelMatrix = computeModelMatrix();

    for (int i = 0; i < numberOfVertices; ++i) {
        // Extend dimensions to homogeneous in order to include translation
        // from model matrix
        Vector4f homoVector(vertices[i](0),
                            vertices[i](1),
                            vertices[i](2),
                            1.0);
        Vector4f transformedVertex = modelMatrix * homoVector;

        x += transformedVertex(0);
        y += transformedVertex(1);
        z += transformedVertex(2);

    }
    Vector3f barycentricCoordinates(x, y, z);
    barycentricCoordinates /= numberOfVertices;

    return barycentricCoordinates;
}
