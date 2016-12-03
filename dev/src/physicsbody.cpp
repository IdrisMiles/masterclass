#include "include/Physics/physicsbody.h"
#include "include/SpherePacking/meshspherepacker.h"

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
    DeleteMesh();

    m_dynamicWorld = 0;
    m_physicsBodyProperties = 0;
}

//-------------------------------------------------------------------------------------------------------------------------
// initialisationa and loading methods

void PhysicsBody::LoadMesh(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris, std::shared_ptr<SimObjectProperties> _props)
{
    if(m_meshLoaded)
    {
        // Loading a new mesh/reloading with new properties
        DeleteMesh();
    }

    m_meshLoaded = true;

    if(_props != 0)
    {
        m_physicsBodyProperties = _props;
    }

    if(m_physicsBodyProperties != 0)
    {
        InitialiseSphericalRigidbodies(meshVerts, meshTris);
        InitialiseInternalConstraints();
    }
    else
    {
        // Properties have not been set!
        assert(m_physicsBodyProperties != 0);
    }
}

void PhysicsBody::LoadMesh(const Mesh _mesh, std::shared_ptr<SimObjectProperties> _props)
{
    if(m_meshLoaded)
    {
        // Loading a new mesh/reloading with new properties
        DeleteMesh();
    }

    m_meshLoaded = true;

    if(_props != 0)
    {
        m_physicsBodyProperties = _props;
    }

    if(m_physicsBodyProperties != 0)
    {
        InitialiseSphericalRigidbodies(_mesh.verts, _mesh.tris);
        InitialiseInternalConstraints();
    }
    else
    {
        // Properties have not been set!
        assert(m_physicsBodyProperties != 0);
    }
}

void PhysicsBody::InitialiseSphericalRigidbodies(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris)
{

    MeshSpherePacker::vdb::PackMeshWithSpheres(m_spheres, meshVerts, meshTris, m_physicsBodyProperties->PhysBody.numSpheres, m_physicsBodyProperties->PhysBody.overlapSpheres, m_physicsBodyProperties->PhysBody.minSphereRad, m_physicsBodyProperties->PhysBody.maxSphereRad);

    // create rigidbody for each sphere
    for(auto sphere : m_spheres)
    {
        float x = sphere.x;
        float y = sphere.y;
        float z = sphere.z;
        float r = sphere.w;

        m_collisionShapes.push_back(new btSphereShape(r));
        m_collisionShapes.back()->setUserPointer((void*)this);
        m_motionStates.push_back(new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(x, y, z))));
        btScalar mass = 1.0f;
        btVector3 sphereInertia = btVector3(0,0,0);
        m_collisionShapes.back()->calculateLocalInertia(mass,sphereInertia);
        btRigidBody::btRigidBodyConstructionInfo sphereRBCI(/*(4.0/3.0) * M_PI * r*r*r**/ mass, m_motionStates.back(), m_collisionShapes.back(), sphereInertia);


        m_rigidBodies.push_back(new btRigidBody(sphereRBCI));
    }
}

void PhysicsBody::InitialiseInternalConstraints(std::shared_ptr<SimObjectProperties> _props)
{
    int constraintCheck[m_rigidBodies.size()][m_rigidBodies.size()];
    int i=0;
    for(auto sphere1 : m_rigidBodies)
    {
        int j=0;
        for(auto sphere2 : m_rigidBodies)
        {
            if (sphere1 == sphere2 || constraintCheck[j][i])
            {
                continue;
            }


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
            if(dist < 2*(r1+r2))
            {
                btTransform invCentreOfMassB = sphere2->getCenterOfMassTransform().inverse();
                btTransform globalFrameA = sphere1->getCenterOfMassTransform() * frameInA;
                frameInB = invCentreOfMassB * globalFrameA;


//                m_internalConstraints.push_back(new btSliderConstraint(*m_rigidBodies[i], *m_rigidBodies[j], localA, localB, false));
//                m_internalConstraints.push_back(new btPoint2PointConstraint(*sphere1, *sphere2, pos1, pos2));

                /*
                btGeneric6DofSpringConstraint *constraint = new btGeneric6DofSpringConstraint(*sphere1, *sphere2, frameInA, frameInB, true);
                constraint->setEquilibriumPoint();
                constraint->setBreakingImpulseThreshold(100000.0f);
                constraint->setLinearLowerLimit(btVector3(dist, dist, dist));
                constraint->setLinearUpperLimit(btVector3(dist, dist, dist));
                constraint->setAngularLowerLimit(btVector3( 0.0,0.0,0.0 ));
                constraint->setAngularUpperLimit(btVector3( 0.0,0.0,0.0 ));
                */

                btFixedConstraint *constraint = new btFixedConstraint(*sphere1, *sphere2, frameInA, frameInB);
                constraint->setBreakingImpulseThreshold(1000000.0f);
                m_internalConstraints.push_back(constraint);

                constraintCheck[i][j] = 1;
                constraintCheck[j][i] = 1;
            }


            j++;
        }
        i++;
    }
}



//-------------------------------------------------------------------------------------------------------------------------
// utility methods

void PhysicsBody::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    m_dynamicWorld = _dynamicWorld;
    AddRigidBodiesToDynamicWorld(_dynamicWorld, _selfCollisions);
    AddConstraintsToDynamicWorld(_dynamicWorld, _selfCollisions);
}


void PhysicsBody::AddRigidBodiesToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    for( auto rb : m_rigidBodies)
    {
        if(_selfCollisions)
        {
            _dynamicWorld->addRigidBody(rb);
        }
        else
        {
            _dynamicWorld->addRigidBody(rb, 1<<(m_id+1), ~(1<<(m_id+1)));
        }
    }
}

void PhysicsBody::AddConstraintsToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    for( auto ic : m_internalConstraints)
    {
        _dynamicWorld->addConstraint(ic);
    }
}

void PhysicsBody::RemoveRigidBodiesToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    for( auto rb : m_rigidBodies)
    {
        _dynamicWorld->removeRigidBody(rb);
    }
}

void PhysicsBody::RemoveConstraintsToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    for( auto ic : m_internalConstraints)
    {
        _dynamicWorld->removeConstraint(ic);
    }
}

void PhysicsBody::DeleteMesh()
{
    RemoveRigidBodiesToDynamicWorld(m_dynamicWorld);
    RemoveConstraintsToDynamicWorld(m_dynamicWorld);

    for(unsigned int i=0; i<m_motionStates.size(); ++i)
    {
        delete m_motionStates[i];
    }
    for(unsigned int i=0; i<m_collisionShapes.size(); ++i)
    {
        delete m_rigidBodies[i];
    }
    for(unsigned int i=0; i<m_rigidBodies.size(); ++i)
    {
        delete m_motionStates[i];
    }
    for(unsigned int i=0; i<m_internalConstraints.size(); ++i)
    {
        delete m_internalConstraints[i];
    }

    m_motionStates.clear();
    m_collisionShapes.clear();
    m_rigidBodies.clear();
    m_internalConstraints.clear();
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

}
