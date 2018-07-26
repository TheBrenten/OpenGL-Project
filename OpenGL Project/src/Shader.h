#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include "Globals.h"

//int getFileLength(std::string filename);

class Shader {
public:
    static const unsigned int NUM_SHADERS = 2;
    static const unsigned int NUM_UNIFORMS = 3;
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
public:
    Shader(const std::string& fileName);
    virtual ~Shader();
    void bind();
    //void update(const Transform& transform, const Camera& camera);
};

GLuint loadShaderFromFile(std::string path, GLenum shaderType);

