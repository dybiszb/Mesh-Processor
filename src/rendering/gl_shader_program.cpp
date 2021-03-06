// author: dybisz

#include "rendering/gl_shader_program.h"

//------------------------------------------------------------------------------
glShaderProgram::glShaderProgram(const GLchar *vertexPath,
                             const GLchar *fragmentPath) {
    std::string vertexCode = loadShaderSourceCode(vertexPath, "VERTEX");
    std::string fragmentCode = loadShaderSourceCode(fragmentPath, "FRAGMENT");

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
glShaderProgram::~glShaderProgram() {
    this->unuse();
    glDeleteProgram(this->id);
}

//------------------------------------------------------------------------------
GLuint
glShaderProgram::getId() {
    return id;
}

//------------------------------------------------------------------------------
std::string
glShaderProgram::loadShaderSourceCode(std::string path, std::string type) {
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::badbit);

    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::" << type << "::SHADER::FILE_NOT_SUCCESFULLY_READ"
                  << std::endl;
    }

    return shaderCode;
}

//------------------------------------------------------------------------------
GLuint
glShaderProgram::compileShader(std::string shaderCode, GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);
    const char* temp = shaderCode.c_str();
    glShaderSource(shaderId, 1, &(temp), NULL);
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
glShaderProgram::getIdStatusInfo(GLuint id, GLenum status, std::string idType) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cout << "ERROR::" << idType << "\n";
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "::TYPE::" << status << "\n" << infoLog << std::endl;
    }
}

//------------------------------------------------------------------------------
void
glShaderProgram::use() {
    glUseProgram(this->id);
}

//------------------------------------------------------------------------------
void
glShaderProgram::unuse() {
    glUseProgram(0);
}