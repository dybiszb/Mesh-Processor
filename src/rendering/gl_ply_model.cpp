// author: dybisz

#include "rendering/gl_ply_model.h"

//-----------------------------------------------------------------------------
glPlyModel::glPlyModel(string path) : path(path), modelLoaded(false) {
    this->loadModel(path);
}

//-----------------------------------------------------------------------------
glPlyModel::~glPlyModel() {}

//-----------------------------------------------------------------------------
void
glPlyModel::render(glShaderProgram &shader) {

}

//-----------------------------------------------------------------------------
void
glPlyModel::loadModel(string &path) {
    if (!modelLoaded) {

        // Open File
        ifstream file(path, std::ifstream::in);
        if (file.fail()) {
            cout << "Failed to " << path << endl;
        }

        // Parse File
        int verticesCount = -1;
        int facesCount = -1;
        bool headerEnd = false;
        while (!file.eof()) {

            std::string line;
            std::getline(file, line);

            // Number of vertices and faces unknown
            int countVertices = -1;
            int countFaces = -1;
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

            } else if (facesCount > 0) {
                // Load Faces
                stringstream lineStream(line);
                facesCount--;

                int _, i1, i2, i3;
                lineStream >> _ >> i1 >> i2 >> i3;
                Vector3f face(i1, i2, i3);
                faces.push_back(face);
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
