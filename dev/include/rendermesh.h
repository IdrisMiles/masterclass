#ifndef RENDERMESH_H
#define RENDERMESH_H

#include <GL/glew.h>

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class RenderMesh
{
public:
    RenderMesh();
    RenderMesh(const RenderMesh &_mesh);
    ~RenderMesh();

    void LoadMesh(const std::string &_file);
    void InitialiseMesh();
    void Render();


private:

    glm::mat4 m_modelMat;

    glm::vec3 m_colour;
    int m_colourLoc;

    QOpenGLVertexArrayObject m_vaos;
    QOpenGLBuffer m_vbos;
    QOpenGLBuffer m_ibos;
    std::vector<glm::vec3> m_meshVerts;
    std::vector<unsigned int> m_meshElementIndex;
};

#endif // RENDERMESH_H
