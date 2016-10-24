#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

// OpenVDB includes
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/VolumeToSpheres.h>

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
    PhysicsBody(const unsigned int _id, QOpenGLShaderProgram *_shaderProg, const glm::vec3 _colour = glm::vec3(0.8f, 0.4f, 0.4f));
    void LoadMesh(const std::string _meshFile);
    void DrawMesh();
    void DrawSpheres();
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);

private:

    void RecursiveTraverseAlembicGetPolyMesh(const IObject &_object, int _tab, int _depth, IPolyMesh &_outputMesh);
    void InitialiseRenderMesh();
    void InitialisePhysicsMesh();
    void InitialiseRenderSpheres();
    void AppendSphereVerts(glm::vec3 _pos = glm::vec3(0.0f,0.0f,0.0f), float _radius = 1.0f, int _stacks = 16, int _slices = 32);

    unsigned int m_id;

    // common members for rendering
    int m_modelMatrixLoc;
    int m_colourLoc;
    glm::mat4 m_modelMat;
    glm::vec3 m_colour;
    QOpenGLShaderProgram *m_shaderProg;

    // members for rendering actual mesh
    QOpenGLVertexArrayObject m_meshVAO;
    QOpenGLBuffer m_meshVBO;
    QOpenGLBuffer m_meshIBO;
    QOpenGLBuffer m_meshModelMatInstanceBO;
    std::vector<glm::vec3> m_meshVerts;
    std::vector<unsigned int> m_meshElementIndex;
    std::vector<openvdb::Vec3f> m_meshVertsVDB;
    std::vector<openvdb::Vec3I> m_meshTrisElements;

    // members for rendering sphereical representation of mesh
    int m_modelMatricesLoc;
    std::vector<glm::mat4> m_sphereModelMats;
    QOpenGLVertexArrayObject m_sphereVAO;
    QOpenGLBuffer m_sphereVBO;
    QOpenGLBuffer m_sphereNBO;
    QOpenGLBuffer m_sphereIBO;
    QOpenGLBuffer m_sphereModelMatInstanceBO;
    std::vector<glm::vec3> m_sphereVerts;
    std::vector<glm::vec3> m_sphereNormals;
    std::vector<unsigned int> m_sphereElementIndex;

    // Physics members
    btScalar m_mass;
    btVector3 m_inertia;
    std::vector<btDefaultMotionState*> m_motionStates;
    std::vector<btCollisionShape*> m_collisionShapes;
    std::vector<btRigidBody*> m_rigidBodies;
};

#endif // PHYSICSBODY_H
