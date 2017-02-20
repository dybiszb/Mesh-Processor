//==============================================================================
// Following class provides means for loading and compiling shader program. As
// for now only fragment and vertex shaders are provided.
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include "GL/gl.h"

class glShaderProgram {
public:
    /**
     * Main constructor.
     *
     * @param vertexPath   Path to vertex shader file.
     * @param fragmentPath Path to fragment shader file.
     */
    glShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);

    ~glShaderProgram();

    GLuint getId();

    /**
     * Start using the program within OpenGL context;
     */
    void use();

    /**
     * Stop using the program within OpenGL context;
     */
    void unuse();

private:
    GLuint id;

    /**
     * Parse shader's code from provided file.
     *
     * @param path Path to file with shader code.
     * @param type Used only to indicate error, hence should be set either to
     *             "fragment" or "vertex" values. Empty by default.
     *
     * @return     Parsed shader code.
     */
    std::string loadShaderSourceCode(std::string path, std::string type="");

    /**
     * Compile shaders according to provided code and and type;
     *
     * @param shaderCode Parsed shader code. It is advised to use
     *                   loadShaderSourceCode procedure beforehand.
     * @param shaderType As for now it can only takes values of either :
     *                   GL_FRAGMENT_SHADER or GL_VERTEX_SHADER.
     *
     * @return           An ID of compiled shader.
     */
    GLuint compileShader(std::string shaderCode, GLenum shaderType);

    /**
     * The procedure basically introduces ease interface for calling
     * glGetProgramiv.
     *
     * @param id      ID of an objects that one wants to investigate.
     * @param status  Enum determining status one wants to check e.g.
     *                GL_COMPILE_STATUS or GL_LINK_STATUS;
     * @param idType  Additional information for error cause displaying e,g,
     *                "FRAGMENT_SHADER" or "PROGRAM".
     */
    void getIdStatusInfo(GLuint id, GLenum status, std::string idType);


};

#endif
