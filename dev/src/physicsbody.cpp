#include "include/Physics/physicsbody.h"
#include "include/SpherePacking/meshspherepacker.h"
#include <float.h>

PhysicsBody::PhysicsBody()
{
    m_id = 0;
    m_meshLoaded = false;
}

PhysicsBody::PhysicsBody(const unsigned int _id, std::shared_ptr<SimObjectProperties> _properties)
{
    m_id = _id;
    m_meshLoaded = false;
    m_physicsBodyProperties = _properties;
}

PhysicsBody::~PhysicsBody()
{
    for(unsigned int i=0; i<m_motionStates.size(); ++i)
    {
        delete m_motionStates[i];
    }

    for(unsigned int i=0; i<m_collisionShapes.size(); ++i)
    {
        delete m_collisionShapes[i];
    }

    for(unsigned int i=0; i<m_rigidBodies.size(); ++i)
    {
        m_rigidBodies[i] = nullptr;
    }

    for(unsigned int i=0; i<m_internalConstraints.size(); ++i)
    {
        m_internalConstraints[i] = nullptr;
    }

    m_motionStates.clear();
    m_collisionShapes.clear();
    m_rigidBodies.clear();
    m_internalConstraints.clear();
    m_spheres.clear();

    m_dynamicWorld = 0;
    m_physicsBodyProperties = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------
// initialisationa and loading methods

void PhysicsBody::LoadMesh(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris, std::shared_ptr<SimObjectProperties> _props)
{
    if(m_meshLoaded)
    {
        // Loading a new mesh/reloading with new properties
        DeleteMesh();
        m_meshLoaded = false;
    }

    m_mesh.verts = meshVerts;
    m_mesh.tris = meshTris;


    if(_props != nullptr)
    {
        m_physicsBodyProperties = _props;
    }

    if(m_physicsBodyProperties != nullptr)
    {
        InitialiseSphericalRigidbodies(meshVerts, meshTris);
        InitialiseInternalConstraints();
    }
    else
    {
        // Properties have not been set!
        assert(m_physicsBodyProperties != nullptr);
    }

    m_meshLoaded = true;
}

void PhysicsBody::LoadMesh(const Mesh &_mesh, std::shared_ptr<SimObjectProperties> _props)
{
    if(m_meshLoaded)
    {
        // Loading a new mesh/reloading with new properties
        DeleteMesh();
        m_meshLoaded = false;
    }
    else
    {
        m_mesh = _mesh;
    }

    if(_props != nullptr)
    {
        m_physicsBodyProperties = _props;
    }

    if(m_physicsBodyProperties != nullptr)
    {
        InitialiseSphericalRigidbodies(m_mesh.verts, m_mesh.tris);
        InitialiseInternalConstraints();
    }
    else
    {
        // Properties have not been set!
        assert(m_physicsBodyProperties != nullptr);
    }

    m_meshLoaded = true;
}

void PhysicsBody::InitialiseSphericalRigidbodies(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris)
{
    MeshSpherePacker::vdb::PackMeshWithSpheres(m_spheres, meshVerts, meshTris, m_physicsBodyProperties->PhysBody.numSpheres, m_physicsBodyProperties->PhysBody.overlapSpheres, m_physicsBodyProperties->PhysBody.minSphereRad, m_physicsBodyProperties->PhysBody.maxSphereRad);

    // create rigidbody for each sphere
    int id = 0;
    for(auto sphere : m_spheres)
    {
        float x = sphere.x;
        float y = sphere.y;
        float z = sphere.z;
        float r = sphere.w;

        m_collisionShapes.push_back(new btSphereShape(r));
        //m_collisionShapes.back()->setUserPointer((void*)this);
        m_motionStates.push_back(new btDefaultMotionState(btTransform(btQuaternion::getIdentity(), btVector3(x, y, z))));
        btScalar mass = 1.0f;
        btVector3 sphereInertia = btVector3(0,0,0);
        m_collisionShapes.back()->calculateLocalInertia(mass,sphereInertia);
        btRigidBody::btRigidBodyConstructionInfo sphereRBCI(/*(4.0/3.0) * M_PI * r*r*r**/ mass, m_motionStates.back(), m_collisionShapes.back(), sphereInertia);

        m_rigidBodies.push_back(std::shared_ptr<btRigidBody>(new btRigidBody(sphereRBCI)));
        m_rigidBodies.back()->setUserIndex(id);
        m_rigidBodies.back()->setUserPointer((void*)this);


        id++;
    }
}

void PhysicsBody::InitialiseInternalConstraints()
{
    bool constraintCheck[m_rigidBodies.size()][m_rigidBodies.size()] = {false};
    int i=0;
    for(auto sphere1 : m_rigidBodies)
    {
        int j=0;
        for(auto sphere2 : m_rigidBodies)
        {
            if (sphere1 == sphere2 || constraintCheck[j][i] || constraintCheck[i][j])
            {
                j++;
                continue;
            }
            else
            {
                btTransform frameInA, frameInB;
                frameInA.setIdentity();
                frameInB.setIdentity();

                sphere1->getMotionState()->getWorldTransform(frameInA);
                sphere2->getMotionState()->getWorldTransform(frameInB);

                btVector3 pos1 = frameInA.getOrigin();
                btVector3 pos2 = frameInB.getOrigin();

                float r1 = dynamic_cast<btSphereShape*>(sphere1->getCollisionShape())->getRadius();
                float r2 = dynamic_cast<btSphereShape*>(sphere2->getCollisionShape())->getRadius();


                float dist = pos1.distance(pos2);
                if(dist <= 1.5f*(r1+r2))
                {
                    m_internalConstraints.push_back(CreateConstraint(*sphere1, *sphere2));
                }

                constraintCheck[i][j] = true;
                constraintCheck[j][i] = true;
                j++;
            }
        }
        i++;
    }

    printf("num constraints: %u\n",m_internalConstraints.size());
}

std::shared_ptr<btTypedConstraint> PhysicsBody::CreateConstraint(btRigidBody &rigidA, btRigidBody &rigidB)
{
    btTransform frameInA, frameInB;
    frameInA.setIdentity();
    frameInB.setIdentity();

    rigidA.getMotionState()->getWorldTransform(frameInA);
    rigidB.getMotionState()->getWorldTransform(frameInB);

    btTransform invCentreOfMassB = rigidB.getCenterOfMassTransform().inverse();
    btTransform globalFrameA = rigidA.getCenterOfMassTransform() * frameInA;
    frameInB = invCentreOfMassB * globalFrameA;


    if (m_physicsBodyProperties->PhysBody.constraintType == ConstraintTypes::Fixed)
    {
        std::shared_ptr<btFixedConstraint> constraint = std::shared_ptr<btFixedConstraint>(new btFixedConstraint(rigidA, rigidB, frameInA, frameInB));
        constraint->setBreakingImpulseThreshold(m_physicsBodyProperties->PhysBody.internalSpringBreakingImpulseThreshold);
        return constraint;
    }
    else if(m_physicsBodyProperties->PhysBody.constraintType == ConstraintTypes::Generic6DOFSpring)
    {
        std::shared_ptr<btGeneric6DofSpringConstraint> constraint = std::shared_ptr<btGeneric6DofSpringConstraint>(new btGeneric6DofSpringConstraint(rigidA, rigidB, frameInA, frameInB, true));
//            constraint->setStiffness(0, 1.0f);
//            constraint->setDamping(0, 1.0f);
        constraint->setBreakingImpulseThreshold(m_physicsBodyProperties->PhysBody.internalSpringBreakingImpulseThreshold);
        constraint->setLinearLowerLimit(btVector3(0,0,0));
        constraint->setLinearUpperLimit(btVector3(1, 1, 1));
        constraint->setAngularLowerLimit(btVector3( 0, 0, 0 ));
        constraint->setAngularUpperLimit(btVector3( 0, 0, 0 ));
        return constraint;
    }
    else
    {
        std::shared_ptr<btFixedConstraint> constraint = std::shared_ptr<btFixedConstraint>(new btFixedConstraint(rigidA, rigidB, frameInA, frameInB));
        constraint->setBreakingImpulseThreshold(m_physicsBodyProperties->PhysBody.internalSpringBreakingImpulseThreshold);
        return constraint;
    }

}

//-------------------------------------------------------------------------------------------------------------------------
// utility methods

void PhysicsBody::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    m_dynamicWorld = _dynamicWorld;
    AddRigidBodiesToDynamicWorld(m_dynamicWorld, _selfCollisions);
    AddConstraintsToDynamicWorld(m_dynamicWorld, _selfCollisions);
}

void PhysicsBody::RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    m_dynamicWorld = _dynamicWorld;
    RemoveRigidBodiesFromDynamicWorld(_dynamicWorld);
    RemoveConstraintsFromDynamicWorld(_dynamicWorld);
}


void PhysicsBody::AddRigidBodiesToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    if(_dynamicWorld != nullptr)
    {
        for( auto rb : m_rigidBodies)
        {
            if(_selfCollisions)
            {
                _dynamicWorld->addRigidBody(rb.get());
            }
            else
            {
                _dynamicWorld->addRigidBody(rb.get(), 1<<(m_id+1), ~(1<<(m_id+1)));
            }
        }
    }
}

void PhysicsBody::AddConstraintsToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    if(_dynamicWorld != nullptr)
    {
        for( auto ic : m_internalConstraints)
        {
            _dynamicWorld->addConstraint(ic.get());
        }
    }
}

void PhysicsBody::RemoveRigidBodiesFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    if(_dynamicWorld != nullptr)
    {
        for( auto rb : m_rigidBodies)
        {
            if(rb != nullptr)
            {
                _dynamicWorld->removeRigidBody(rb.get());
            }
        }
    }
}

void PhysicsBody::RemoveConstraintsFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    if(_dynamicWorld != nullptr)
    {
        for( auto &&ic : m_internalConstraints)
        {
            _dynamicWorld->removeConstraint(ic.get());
        }
    }
}

void PhysicsBody::DeleteMesh()
{
    m_meshLoaded = false;

    if(m_dynamicWorld != nullptr)
    {
        RemoveRigidBodiesFromDynamicWorld(m_dynamicWorld);
        RemoveConstraintsFromDynamicWorld(m_dynamicWorld);
    }

    for(unsigned int i=0; i<m_motionStates.size(); ++i)
    {
        delete m_motionStates[i];
    }

    for(unsigned int i=0; i<m_collisionShapes.size(); ++i)
    {
        delete m_collisionShapes[i];
    }

    for(unsigned int i=0; i<m_rigidBodies.size(); ++i)
    {
        m_rigidBodies[i] = nullptr;
    }

    for(unsigned int i=0; i<m_internalConstraints.size(); ++i)
    {
        m_internalConstraints[i] = nullptr;
    }

    m_motionStates.clear();
    m_collisionShapes.clear();
    m_rigidBodies.clear();
    m_internalConstraints.clear();
    m_spheres.clear();
}

void PhysicsBody::GetSpheres(std::vector<glm::vec4> &_spheres) const
{
    _spheres = m_spheres;
}

void PhysicsBody::GetSpheresMatrices(std::vector<glm::mat4> &_sphereMats) const
{
    _sphereMats.clear();
    for( auto rb : m_rigidBodies)
    {
        btTransform worldTransform;
        rb->getMotionState()->getWorldTransform(worldTransform);

        glm::mat4 mat;
        worldTransform.getOpenGLMatrix(&mat[0][0]);

        _sphereMats.push_back(mat);
    }
}

void PhysicsBody::Reset()
{
    for(unsigned int i=0; i<m_spheres.size(); i++)
    {
        m_rigidBodies[i]->clearForces();
        m_rigidBodies[i]->setLinearVelocity(btVector3(0.0f,0.0f,0.0f));
        m_rigidBodies[i]->setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
        btTransform trans(btQuaternion::getIdentity(), btVector3(m_spheres[i].x, m_spheres[i].y, m_spheres[i].z));
        m_rigidBodies[i]->setWorldTransform(trans);
    }
}


void PhysicsBody::Cache(CachedSimObject &_cachSim)
{

}


void PhysicsBody::UpdatePhysicsProps()
{
    Reset();
    LoadMesh(m_mesh, m_physicsBodyProperties);
    AddToDynamicWorld(m_dynamicWorld, m_physicsBodyProperties->PhysBody.selfCollisions);
}

void PhysicsBody::Update(){
    UpdatePlasticConstraints();
}

void PhysicsBody::UpdatePlasticConstraints()
{
    // This is probably an inefficient way to do it
    for(unsigned int i=0; i<m_internalConstraints.size(); i++)
    {
        float impulse = m_internalConstraints[i]->getAppliedImpulse();
        if(impulse >= m_physicsBodyProperties->PhysBody.yieldStrength)
        {
            auto rbA = m_rigidBodies[m_internalConstraints[i]->getRigidBodyA().getUserIndex()];
            auto rbB = m_rigidBodies[m_internalConstraints[i]->getRigidBodyB().getUserIndex()];


            // remove old constraint
            m_dynamicWorld->removeConstraint(m_internalConstraints[i].get());

            // add updated constraint
            m_internalConstraints.at(i) = CreateConstraint(*rbA, *rbB);
            m_dynamicWorld->addConstraint(m_internalConstraints[i].get());
        }
    }
}
