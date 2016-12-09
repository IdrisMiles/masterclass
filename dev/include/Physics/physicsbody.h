#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H


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


class PhysicsBody
{
public:
    PhysicsBody();
    PhysicsBody(const unsigned int _id, std::shared_ptr<SimObjectProperties> _properties = nullptr);
    ~PhysicsBody();
    void LoadMesh(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris, std::shared_ptr<SimObjectProperties> _props = nullptr);
    void LoadMesh(const Mesh &_mesh, std::shared_ptr<SimObjectProperties> _props = nullptr);
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void GetSpheres(std::vector<glm::vec4> &_spheres) const;
    void GetSpheresMatrices(std::vector<glm::mat4> &_sphereMats) const;
    void Reset();
    void Cache(CachedSimObject &_cachSim);
    void UpdatePhysicsProps();
    void SetID(const int &_id){m_id = _id;}

private:

    void InitialiseSphericalRigidbodies(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris);
    void InitialiseInternalConstraints();
    void AddConstraint(std::shared_ptr<btRigidBody> rigidA, std::shared_ptr<btRigidBody> rigidB);
    void AddRigidBodiesToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void AddConstraintsToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void RemoveRigidBodiesFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void RemoveConstraintsFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void DeleteMesh();


    unsigned int m_id;
    bool m_meshLoaded;

    // Physics members
    btScalar m_mass;
    btVector3 m_inertia;
    std::vector<btDefaultMotionState*> m_motionStates;
    std::vector<btCollisionShape*> m_collisionShapes;
    std::vector<std::shared_ptr<btRigidBody>> m_rigidBodies;
    std::vector<btTypedConstraint*> m_internalConstraints;

    Mesh m_mesh;
    std::vector<glm::vec4> m_spheres;
    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;
    btDiscreteDynamicsWorld * m_dynamicWorld;
};

#endif // PHYSICSBODY_H
