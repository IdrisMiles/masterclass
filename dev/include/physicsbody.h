#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

// OpenVDB includes
//#include <openvdb/openvdb.h>
//#include <openvdb/tools/MeshToVolume.h>

// Open GL includes
#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Bullets includes
#include <bullet/btBulletDynamicsCommon.h>

// Alembic includes
#include "Alembic/AbcGeom/All.h"
#include "Alembic/AbcCoreAbstract/All.h"
#include "Alembic/AbcCoreHDF5/All.h"
#include "Alembic/Abc/ErrorHandler.h"

using namespace Alembic::Abc;
using namespace Alembic::AbcGeom;

class PhysicsBody
{
public:
    PhysicsBody();
    void LoadMesh(const std::string _meshFile, QOpenGLShaderProgram *_shaderProg);
    void DrawMesh();
    void DrawSpheres();

private:

    void RecursiveTraverseAlembicGetPolyMesh(const IObject &_object, int _tab, int _depth, IPolyMesh &_outputMesh);
    void InitialiseRenderMesh();
    void InitialisePhysicsMesh();
    void InitialiseRenderSpheres();
    void AppendSphereVerts(glm::vec3 _pos = glm::vec3(0.0f,0.0f,0.0f), float _radius = 1.0f, int _stacks = 16, int _slices = 32);

    // common members for rendering
    glm::mat4 m_modelMat;
    glm::vec3 m_colour;
    int m_colourLoc;
    QOpenGLShaderProgram *m_shaderProg;

    // members for rendering actual mesh
    QOpenGLVertexArrayObject m_meshVAO;
    QOpenGLBuffer m_meshVBO;
    QOpenGLBuffer m_meshIBO;
    std::vector<glm::vec3> m_meshVerts;
    std::vector<unsigned int> m_meshElementIndex;

    // members for rendering sphereical representation of mesh
    QOpenGLVertexArrayObject m_sphereVAO;
    QOpenGLBuffer m_sphereVBO;
    QOpenGLBuffer m_sphereNBO;
    QOpenGLBuffer m_sphereIBO;
    std::vector<glm::vec3> m_sphereVerts;
    std::vector<glm::vec3> m_sphereNormals;
    std::vector<unsigned int> m_sphereElementIndex;

    // Physics members
    btScalar m_mass;
    btVector3 m_inertia;
    btDefaultMotionState *m_motionState;
    btCollisionShape * m_collisionShape;
    btRigidBody *m_rigidBody;
};

#endif // PHYSICSBODY_H
