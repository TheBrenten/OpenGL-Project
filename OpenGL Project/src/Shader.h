#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

//int getFileLength(std::string filename);

GLuint loadShaderFromFile(std::string path, GLenum shaderType);