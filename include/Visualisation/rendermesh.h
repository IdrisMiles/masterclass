#ifndef RENDERMESH_H
#define RENDERMESH_H


/// @author Idris Miles
/// @version 0.1.0
/// @date 10th January 2017


// Open GL includes
#include <GL/glew.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "include/UserInterface/simobjectproperties.h"
#include "include/Visualisation/mesh.h"
#include "include/Physics/physicsbody.h"

#include <memory>

/// @class RenderMesh class. This class render a skinned mesh, using PhysicsBody to deform the mesh.
class RenderMesh
{
public:
    RenderMesh();
    RenderMesh(const RenderMesh &_copy);
    virtual ~RenderMesh();

    virtual void LoadMesh(const Mesh &_mesh, std::shared_ptr<SimObjectProperties> _physicsBodyProperties = nullptr);
    virtual void DrawMesh();

    void InitialiseSkinWeights(const std::vector<glm::vec4> &_spheres);
    void InitialiseSkinWeights(const PhysicsBody &_physBody);
    void Skin(const std::vector<glm::vec4> &_spheres);
    void Skin(const PhysicsBody &_physBody);

    // Setter
    void SetWireframe(const bool &_wireframe);
    void SetDrawMesh(const bool &_drawMesh);
    void SetColour(const glm::vec3 &_colour);


protected:
    void CreateShader();
    void CreateVAOs();
    void DeleteVAOs();
    void UpdateVAOs();


    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;

    bool m_wireframe;
    bool m_drawMesh;
    bool m_meshLoaded;
    bool m_vaoLoaded;
    bool m_skinWeightLoaded;

    glm::mat4 m_modelMat;
    glm::vec3 m_colour;

    // Scene shader stuff
    int m_projMatrixLoc;
    int m_viewMatrixLoc;
    int m_modelMatrixLoc;
    int m_lightPosLoc;
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

    std::vector<glm::vec3> m_skinnedMeshVerts;
    std::vector<std::vector<std::pair<unsigned int, float>>> m_skinWeights;
    std::vector<glm::vec3> m_controlPoints;
};

#endif // RENDERMESH_H
