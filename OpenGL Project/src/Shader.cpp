#include "Shader.h"
#include <iostream>
#include <fstream>
static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string loadShader(const std::string& fileName);
static GLuint createShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
    m_program = glCreateProgram();
    m_shaders[0] = createShader(loadShader(fileName + ".vs"), GL_VERTEX_SHADER);
    m_shaders[1] = createShader(loadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
    for (unsigned int i = 0; i < NUM_SHADERS; i++)
        glAttachShader(m_program, m_shaders[i]);

    // do this before linking/validating
    // tells opengl about shader data structure
    glBindAttribLocation(m_program, 0, "position");
    glBindAttribLocation(m_program, 1, "normal");

    glLinkProgram(m_program);
    checkShaderError(m_program, GL_LINK_STATUS,true,"Error: Shader program linking failed: ");

    glValidateProgram(m_program);
    checkShaderError(m_program, GL_LINK_STATUS, true, "Error: Program is invalid: ");

    m_uniforms[0] = glGetUniformLocation(m_program, "MVP");
    m_uniforms[1] = glGetUniformLocation(m_program, "Normal");
    m_uniforms[2] = glGetUniformLocation(m_program, "lightDirection");
}

Shader::~Shader()
{
    for (unsigned int i = 0; i < NUM_SHADERS; i++) {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}

static GLuint createShader(const std::string& text, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
        std::cerr << "Error: Shader creation faled!" << std::endl;
    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];
    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLengths[0] = text.length();
    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
    glCompileShader(shader);
    checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");
    return shader;
}

void Shader::bind() {
    glUseProgram(m_program);
}


static std::string loadShader(const std::string& fileName) {
    std::ifstream file;
    file.open((fileName).c_str());
    std::string output;
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    }
    return output;

}

static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
    GLint success = 0;
    GLchar error[1024] = { 0 };
    if (isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);
    if (success == GL_FALSE) {
        if (isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        std::cerr << errorMessage << ": '" << error << "'\n";
    }
}

/*
int getFileLength(std::string filename) {
    std::ifstream file;
    file.open(filename, std::ios::in); // opens as ASCII!
    
    //if (!file) return -1;
    
    if (!file.good()) return 0;

    unsigned long pos = file.tellg();
    file.seekg(0, std::ios::end);
    unsigned long len = file.tellg();
    file.seekg(std::ios::beg);

    return len;
}
*/

GLuint loadShaderFromFile(std::string path, GLenum shaderType)
{
    //Open file
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile(path.c_str());

    //Source file loaded
    if (sourceFile)
    {
        //Get shader source
        shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
        
        // output shader to console
        //printf(shaderString.c_str());
        
        //Create shader ID
        shaderID = glCreateShader(shaderType);

        //Set shader source
        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

        //Compile shader source
        glCompileShader(shaderID);

        //Check shader for errors
        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE)
        {
            printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
            //printShaderLog(shaderID);
            glDeleteShader(shaderID);
            shaderID = 0;
        }
    }
    else
    {
        printf("Unable to open file %s\n", path.c_str());
    }

    return shaderID;
}

