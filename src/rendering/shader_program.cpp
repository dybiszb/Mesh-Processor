// author: dybisz

#include "rendering/shader_program.h"

//------------------------------------------------------------------------------
ShaderProgram::ShaderProgram(const GLchar *vertexPath,
                             const GLchar *fragmentPath) {

    const GLchar *vertexCode = loadShaderSourceCode(vertexPath, "VERTEX");
    const GLchar *fragmentCode = loadShaderSourceCode(fragmentPath, "FRAGMENT");

    GLuint vertexID = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentID = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    this->id = glCreateProgram();
    glAttachShader(this->id, vertexID);
    glAttachShader(this->id, fragmentID);
    glLinkProgram(this->id);

    getIdStatusInfo(this->id, GL_LINK_STATUS, "PROGRAM");

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

//------------------------------------------------------------------------------
ShaderProgram::~ShaderProgram() {

}

//------------------------------------------------------------------------------
const GLchar *
ShaderProgram::loadShaderSourceCode(string path, string type) {
    string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::badbit);

    try {
        shaderFile.open(path);
        stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::" << type << "::SHADER::FILE_NOT_SUCCESFULLY_READ"
                  << std::endl;
    }

    return shaderCode.c_str();
}

//------------------------------------------------------------------------------
GLuint
ShaderProgram::compileShader(const GLchar *shaderCode, GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderCode, NULL);
    glCompileShader(shaderId);

    switch (shaderType) {
        case GL_VERTEX_SHADER:
            getIdStatusInfo(shaderId, GL_COMPILE_STATUS, "FRAGMENT_SHADER");
            break;
        case GL_FRAGMENT_SHADER:
            getIdStatusInfo(shaderId, GL_COMPILE_STATUS, "VERTEX_SHADER");
            break;
        default:
            break;
    }

    return shaderId;
}

//------------------------------------------------------------------------------
void
ShaderProgram::getIdStatusInfo(GLuint id, GLenum status, string idType) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        cout << "ERROR::" << idType << "\n";
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        cout << "::TYPE::" << status << "\n" << infoLog << endl;
    }
}