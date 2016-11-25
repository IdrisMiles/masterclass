#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H


// GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Bullets includes
#include <bullet/btBulletDynamicsCommon.h>


#include "include/UserInterface/physicsbodyproperties.h"



class PhysicsBody
{
public:
    PhysicsBody(const unsigned int _id, std::shared_ptr<PhysicsBodyProperties> _properties = nullptr);
    ~PhysicsBody();
    void LoadMesh(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris);
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void GetSpheres(std::vector<glm::vec4> &_spheres) const;
    void GetSpheresMatrices(std::vector<glm::mat4> &_sphereMats) const;

private:

    void InitialiseSphericalRigidbodies(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris);
    void InitialiseInternalConstraints();


    unsigned int m_id;

    // Physics members
    btScalar m_mass;
    btVector3 m_inertia;
    std::vector<btDefaultMotionState*> m_motionStates;
    std::vector<btCollisionShape*> m_collisionShapes;
    std::vector<btRigidBody*> m_rigidBodies;
    std::vector<btTypedConstraint*> m_internalConstraints;
    std::vector<glm::vec4> m_spheres;
    std::vector<glm::vec3> m_initSpheres;
    std::shared_ptr<PhysicsBodyProperties> m_physicsBodyProperties;
    btDiscreteDynamicsWorld * m_dynamicWorld;
};

#endif // PHYSICSBODY_H
