#ifndef RENDERMESH_H
#define RENDERMESH_H

// Open GL includes
#include <GL/glew.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "include/UserInterface/simobjectproperties.h"
#include "include/Visualisation/mesh.h"

#include <memory>


class RenderMesh
{
public:
    RenderMesh(QOpenGLShaderProgram *_shaderProg = 0);
    virtual ~RenderMesh();

    virtual void LoadMesh(const Mesh _mesh, QOpenGLShaderProgram *_shaderProg = 0, std::shared_ptr<SimObjectProperties> _physicsBodyProperties = nullptr);
    virtual void DrawMesh();

    void Skin(const std::vector<glm::vec4> &_spheres);

    // Getters
    virtual void GetMeshVerts(std::vector<glm::vec3> &_outVerts) const;
    virtual void GetMeshTris(std::vector<glm::ivec3> &_outTris) const;

    // Setter
    void SetShaderProg(QOpenGLShaderProgram *_shaderProg);
    void SetWireframe(const bool &_wireframe);
    void SetDrawMesh(const bool &_drawMesh);
    void SetColour(const glm::vec3 &_colour);


protected:
    void LoadWithASSIMP(const std::string _meshFile);
    virtual void InitVAO();


    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;

    bool m_wireframe;
    bool m_drawMesh;
    bool m_meshLoaded;

    glm::mat4 m_modelMat;
    glm::vec3 m_colour;

    int m_modelMatrixLoc;
    int m_colourLoc;

    QOpenGLShaderProgram *m_shaderProg;
    QOpenGLVertexArrayObject m_meshVAO;
    QOpenGLBuffer m_meshVBO;
    QOpenGLBuffer m_meshNBO;
    QOpenGLBuffer m_meshIBO;
    QOpenGLBuffer m_meshModelMatInstanceBO;

    std::vector<glm::vec3> m_meshVerts;
    std::vector<glm::vec3> m_meshNorms;
    std::vector<glm::ivec3> m_meshTris;

    std::vector<std::vector<float>> m_skinWeights;
};

#endif // RENDERMESH_H
