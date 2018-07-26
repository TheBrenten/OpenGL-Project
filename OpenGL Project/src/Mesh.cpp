#include "Mesh.h"
#include "obj_loader.h"

Mesh::Mesh(const std::string & fileName)
{
    IndexedModel model = OBJModel(fileName).ToIndexedModel();
    initMesh(model);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices) {
    IndexedModel model;
    for (unsigned int i = 0; i < numVertices; i++) {
        model.positions.push_back(*vertices[i].getPos());
        //model.texCoords.push_back(*vertices[i].GetTexCoord());
        model.normals.push_back(*vertices[i].getNormal());
    }
    for (unsigned int i = 0; i < numIndices; i++) {
        model.indices.push_back(indices[i]);
    }
    initMesh(model);
}

void Mesh::initMesh(const IndexedModel & model)
{
    m_drawCount = model.indices.size();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(NUM_BUFFERS, m_VAO_buffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_VAO_buffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
    // "tell opengl this is sequential data"
    glEnableVertexAttribArray(0);
    // tell openGL vertex position data layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VAO_buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VAO_buffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);


    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(NUM_BUFFERS, m_VAO_buffers);
    glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::draw()
{
    glBindVertexArray(m_VAO);
    // indices are unsigned integers - no offset so 0

    //glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
    glDrawElementsBaseVertex(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0, 0);
    //glDrawElements(GL_LINE_STRIP, m_drawCount, GL_UNSIGNED_INT, 0);

    //glDrawArrays(GL_TRIANGLES, 0, m_drawCount);

    //glDrawArrays(GL_TRIANGLE_STRIP, 0, m_drawCount);
    //glDrawArrays(GL_LINE_STRIP, 0, m_drawCount);

    glBindVertexArray(0);
}
