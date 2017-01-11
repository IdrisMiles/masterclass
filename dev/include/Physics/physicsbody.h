#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

/// @author Idris Miles
/// @version 0.1.0
/// @date 10th January 2017


// GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Bullets includes
#include <bullet/btBulletDynamicsCommon.h>

#include "include/UserInterface/simobjectproperties.h"
#include "include/Physics/cachedsim.h"
#include "include/Visualisation/mesh.h"

#include <memory>
#include <vector>

/// @class PhysicsBody class. This class handles various physics objects such as rigidbodies and constraints that make up a mesh.
class PhysicsBody
{
public:

    PhysicsBody(const unsigned int _id = 0, std::shared_ptr<SimObjectProperties> _properties = nullptr);
    ~PhysicsBody();
    void LoadMesh(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris, std::shared_ptr<SimObjectProperties> _props = nullptr);
    void LoadMesh(const Mesh &_mesh, std::shared_ptr<SimObjectProperties> _props = nullptr);
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void GetOrigSpheres(std::vector<glm::vec4> &_spheres) const;
    void GetUpdatedSpheres(std::vector<glm::vec4> &_spheres) const;
    void GetSpheresMatrices(std::vector<glm::mat4> &_sphereMats) const;
    void Reset();
    void Cache(CachedSimObject &_cachSim);
    void Update();
    void UpdatePhysicsProps();
    void SetID(const int &_id){m_id = _id;}
    std::vector<std::shared_ptr<btRigidBody>> getRigidBodies();


private:

    void InitialiseSphericalRigidbodies(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris);
    void InitialiseInternalConstraints();
    std::shared_ptr<btTypedConstraint> CreateConstraint(btRigidBody &rigidA, btRigidBody &rigidB);
    void AddRigidBodiesToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void AddConstraintsToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void RemoveRigidBodiesFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void RemoveConstraintsFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void DeleteMesh();
    void UpdatePlasticConstraints();


    unsigned int m_id;
    bool m_meshLoaded;

    // Physics members
    btScalar m_mass;
    btVector3 m_inertia;
    std::vector<btDefaultMotionState*> m_motionStates;
    std::vector<btCollisionShape*> m_collisionShapes;
    std::vector<std::shared_ptr<btRigidBody>> m_rigidBodies;
    std::vector<std::shared_ptr<btTypedConstraint>> m_internalConstraints;

    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;
    btDiscreteDynamicsWorld * m_dynamicWorld;

    Mesh m_mesh;
    std::vector<glm::vec4> m_spheres;
};

#endif // PHYSICSBODY_H
