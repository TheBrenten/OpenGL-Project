#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "obj_loader.h"

class Vertex {
    glm::vec3 pos;
    //glm::vec2 texCoord;
    glm::vec3 normal;
public:
    Vertex(const glm::vec3& pos,const glm::vec3& normal = glm::vec3(0,0,0)) {
        this->pos = pos;
        //this->texCoord = texCoord
        this->normal = normal;
    }

    glm::vec3* getPos() { return &pos; }
    //glm::vec2* getTexCoord() { return &texCoord; }
    glm::vec3* getNormal() { return &normal; }
};

class Mesh {
private:
    void initMesh(const IndexedModel& model);
    enum {
        POSITION_VB,
        //TEXCOORD_VB,
        NORMAL_VB,
        INDEX_VB,

        NUM_BUFFERS // fucking genius it counts the amount of the previous VB's
    };
    GLuint m_VAO;
    GLuint m_VAO_buffers[NUM_BUFFERS];
    unsigned int m_drawCount; // how many vertices we need to draw
public:
    Mesh(Vertex* vertices,unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
    Mesh(const std::string& fileName);
    virtual ~Mesh();
    void draw();
};