// author: dybisz

#include <rendering/gl_utils.h>
#include "rendering/gl_ply_model.h"

//-----------------------------------------------------------------------------
glPlyModel::glPlyModel(string path) : path(path), modelLoaded(false) {
    this->loadModel(path);
    this->loadOpenGLData();

    // display vertices data gl
//    for(int i = 0; i < 3 * numberOfVertices; i+=3) {
//        cout << glVertices[i] << glVertices[i+1] << glVertices[i+2]<< endl;
//    }

    // display faces data gl
//    for(int i = 0; i < 3 * numberOfFaces; i+=3) {
//        cout << glFaces[i] << " " << glFaces[i+1] << " " <<glFaces[i+2]<< endl;
//    }
}

//-----------------------------------------------------------------------------
glPlyModel::~glPlyModel() {
    if(glVertices) delete [] glVertices;
    if(glFaces) delete [] glFaces;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

//-----------------------------------------------------------------------------
void
glPlyModel::render(glShaderProgram &shader, Matrix4f& view,
                   Matrix4f & projection) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    shader.use();

    glBindVertexArray(vao);

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(),"model"), 1,
                       GL_FALSE, model.data());
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(),"view"), 1,
                       GL_FALSE, view.data());
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(),"projection"), 1,
                       GL_FALSE, projection.data());

    glDrawElements(GL_TRIANGLES, 3*numberOfFaces, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glCheckForErrors();
    shader.unuse();
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
                    glVertices = new GLfloat[3 * numberOfVertices];
                    numberOfVerticesFound = true;
                }

                // Look for number of faces
                std::size_t foundFaces = line.find("element face");
                if (foundFaces != string::npos) {
                    stringstream lineStream(line);
                    string _;
                    lineStream >> _ >> _ >> numberOfFaces;

                    facesCount = numberOfFaces;
                    glFaces = new GLuint[3 * numberOfFaces];
                    numberOfFacesFound = true;
                }


            } else if (!headerEnd) {
                // Look for header end
                std::size_t found = line.find("end_header");
                if (found != string::npos) headerEnd = true;

            } else if (verticesCount > 0) {
                // Load Vertices
                int index = numberOfVertices - verticesCount;
                stringstream lineStream(line);
                verticesCount--;
                float x, y, z;
                lineStream >> x >> y >> z;
                Vector3f vertex(x, y, z);
                vertices.push_back(vertex);

                // Load gl data
                glVertices[3 * index] = x;
                glVertices[3 * index + 1] = y;
                glVertices[3 * index + 2] = z;

            } else if (facesCount > 0) {
                // Load Faces
                stringstream lineStream(line);
                int index = numberOfFaces - facesCount;
                facesCount--;
                unsigned int _, i1, i2, i3;
                lineStream >> _ >> i1 >> i2 >> i3;
                Vector3f face(i1, i2, i3);
                faces.push_back(face);

                // Load gl data

                glFaces[3 * index] =  i1;
                glFaces[3 * index + 1] =  i2;
                glFaces[3 * index + 2] =  i3;
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
        glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 3  *sizeof
                     (GLfloat), glVertices,
                     GL_STATIC_DRAW);
        glCheckForErrors();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfFaces * 3 * sizeof
    (GLuint), glFaces,
                     GL_STATIC_DRAW);
        glCheckForErrors();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                              (GLvoid *) 0);
        glEnableVertexAttribArray(0);
        glCheckForErrors();
    glBindVertexArray(0);

    glCheckForErrors();
}

//-----------------------------------------------------------------------------
void
glPlyModel::printInformation(bool listData) {

    cout << "Model Path:  " << this->path << endl;
    if(numberOfVerticesFound)
        cout << "# vertices: " << this->numberOfVertices << endl;
    else
        cout << "# vertices: " << "Not Found" << endl;

    if (numberOfFacesFound)
        cout << "# faces     " << this->numberOfFaces << endl;
    else
        cout << "# faces     " << "Not Found" << endl;

    if(listData) {

        if(numberOfVerticesFound) {
            cout << endl;
            cout << "[VERTICES LIST]" << endl;
            int index = 0;
            for(auto i = vertices.begin(); i != vertices.end(); i++) {
                cout << "[" << index << "]  " << " " <<
                     (*i)(0) << " " << (*i)(1) << " " << (*i)(2) << endl;
                index++;
            }
        }

        if (numberOfFacesFound) {
            cout << endl;
            cout << "[FACES LIST]" << endl;
            int index = 0;
            for(auto i = faces.begin(); i != faces.end(); i++) {
                cout << "[" << index << "]  " << " " <<
                     (*i)(0) << " " << (*i)(1) << " " << (*i)(2) << endl;
                index++;
            }
        }

    }
}
