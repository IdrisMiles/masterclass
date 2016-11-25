#include "include/Physics/physicsbody.h"
#include "include/SpherePacking/meshspherepacker.h"


PhysicsBody::PhysicsBody(const unsigned int _id, std::shared_ptr<PhysicsBodyProperties> _properties)
{
    m_id = _id;
    m_physicsBodyProperties = _properties;
}

PhysicsBody::~PhysicsBody()
{
    for( auto rb : m_rigidBodies)
    {
        m_dynamicWorld->removeRigidBody(rb);
    }

    for( auto ic : m_internalConstraints)
    {
        m_dynamicWorld->removeConstraint(ic);
    }
    m_dynamicWorld = 0;

    m_physicsBodyProperties = 0;
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

//-------------------------------------------------------------------------------------------------------------------------
// initialisationa and loading methods

void PhysicsBody::LoadMesh(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris)
{

    InitialiseSphericalRigidbodies(meshVerts, meshTris);

    InitialiseInternalConstraints();

}

void PhysicsBody::InitialiseSphericalRigidbodies(const std::vector<glm::vec3> &meshVerts, const std::vector<glm::ivec3> &meshTris)
{


    MeshSpherePacker::vdb::PackMeshWithSpheres(m_spheres, meshVerts, meshTris, 1000, false, 1.0f, 5.0f);


    // create rigidbody for each sphere
    for(auto sphere : m_spheres)
    {
        float x = sphere.x;
        float y = sphere.y;
        float z = sphere.z;
        float r = sphere.w;
        m_initSpheres.push_back(glm::vec3(x,y,z));

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

void PhysicsBody::InitialiseInternalConstraints()
{
    int constraintCheck[m_rigidBodies.size()][m_rigidBodies.size()];
    int i=0;
    for(auto sphere1 : m_rigidBodies)
    {
        int j=0;
        for(auto sphere2 : m_rigidBodies)
        {
            if (sphere1 == sphere2)// || constraintCheck[j][i])
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


//                m_internalConstraints.push_back(new btGeneric6DofSpringConstraint(*sphere1, *sphere2, frameInA, frameInB, true));
//                m_internalConstraints.back()->setBreakingImpulseThreshold(100000.0f);
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setLinearLowerLimit(btVector3(dist, dist, dist));
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setLinearUpperLimit(btVector3(dist, dist, dist));
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setAngularLowerLimit(btVector3( 0.0,0.0,0.0 ));
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setAngularUpperLimit(btVector3( 0.0,0.0,0.0 ));


                btFixedConstraint *constraint = new btFixedConstraint(*sphere1, *sphere2, frameInA, frameInB);
                m_internalConstraints.push_back(constraint);

                constraintCheck[i][j] = 1;

            }


            j++;
        }
        i++;
    }
}



//-------------------------------------------------------------------------------------------------------------------------
// utility methods

void PhysicsBody::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    m_dynamicWorld = _dynamicWorld;
    for( auto rb : m_rigidBodies)
    {
        _dynamicWorld->addRigidBody(rb, 1<<m_id, ~(1<<m_id));
    }

    for( auto ic : m_internalConstraints)
    {
        _dynamicWorld->addConstraint(ic);
    }
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
