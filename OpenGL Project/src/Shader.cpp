#include "Shader.h"
#include <fstream>
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
