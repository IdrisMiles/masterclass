#ifndef RENDERMESH_H
#define RENDERMESH_H

// Open GL includes
#include <GL/glew.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <glm/glm.hpp>

// Alembic includes
#include "Alembic/AbcGeom/All.h"
#include "Alembic/AbcCoreAbstract/All.h"
#include "Alembic/AbcCoreHDF5/All.h"
#include "Alembic/Abc/ErrorHandler.h"

#include "include/UserInterface//physicsbodyproperties.h"

using namespace Alembic::Abc;
using namespace Alembic::AbcGeom;

class RenderMesh
{
public:
    RenderMesh(QOpenGLShaderProgram *_shaderProg = 0);
    virtual ~RenderMesh();

    virtual void LoadMesh(const std::string _meshFile, QOpenGLShaderProgram *_shaderProg = 0, std::shared_ptr<PhysicsBodyProperties> _physicsBodyProperties = nullptr);
    virtual void DrawMesh();

    // Getters
    virtual void GetMeshVerts(std::vector<glm::vec3> &_outVerts) const;
    virtual void GetMeshTris(std::vector<glm::ivec3> &_outTris) const;

    // Setter
    void SetShaderProg(QOpenGLShaderProgram *_shaderProg);
    void SetWireframe(const bool &_wireframe);
    void SetDrawMesh(const bool &_drawMesh);
    void SetColour(const glm::vec3 &_colour);


protected:
    void LoadWithAlembic(const std::string _meshFile);
    void RecursiveTraverseAlembicGetPolyMesh(const IObject &_object, int _tab, int _depth, IPolyMesh &_outputMesh);
    void LoadWithASSIMP(const std::string _meshFile);
    virtual void InitVAO();


    std::shared_ptr<PhysicsBodyProperties> m_physicsBodyProperties;

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

};

#endif // RENDERMESH_H
