// author: dybisz

#include "rendering/gl_ply_model.h"

//-----------------------------------------------------------------------------
glPlyModel::glPlyModel(string path) : modelLoaded(false) {
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
        int numberOfVertices = -1;
        bool numberOfVerticesFound = false;
        bool headerEnd = false;
        while (!file.eof()) {

            std::string line;
            std::getline(file, line);

            // Number of vertices unknown
            if (!numberOfVerticesFound) {

                // Look for number of vertices
                std::size_t found = line.find("element vertex");
                if (found != string::npos) {
                    stringstream lineStream(line);
                    string _;
                    lineStream >> _ >> _ >> numberOfVertices;
                    numberOfVerticesFound = true;
                }

            } else if (!headerEnd) {

                // Look for header end
                std::size_t found = line.find("end_header");
                if (found != string::npos) headerEnd = true;

            } else if(numberOfVertices > 0){
                // Load Vertices
                stringstream lineStream(line);
                numberOfVertices--;
                float x, y, z;
                lineStream >> x >> y >> z;
                Vector3f vertex(x, y, z);
                vertices.push_back(vertex);

            } else break;

        }

        // Close File
        file.close();
        modelLoaded = true;
    } else {
        cout << "Model from path: " << path << " already loaded." << endl;
    }
}
