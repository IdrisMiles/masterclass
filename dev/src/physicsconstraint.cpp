#include "include/Physics/physicsconstraint.h"
#include "include/UserInterface/simobjectproperties.h"
#include "include/Physics/physicsbody.h"

//PhysicsConstraint::PhysicsConstraint(std::shared_ptr<PhysicsBody> _physicsBodyA, std::shared_ptr<PhysicsBody> _physicsBodyB, std::shared_ptr<SimObjectProperties> m_physicsBodyProperties)
PhysicsConstraint::PhysicsConstraint(PhysicsBody *_physicsBodyA, PhysicsBody *_physicsBodyB, std::shared_ptr<SimObjectProperties> _physicsBodyProperties)
{
    if(_physicsBodyA != nullptr)
    {
    m_physicsBodyA = _physicsBodyA;
    }

    if(_physicsBodyB != nullptr)
    {
    m_physicsBodyB = _physicsBodyB;
    }

    if(_physicsBodyProperties != nullptr)
    {
        m_physicsBodyProperties = _physicsBodyProperties;
    }

    InitialiseConstraints();
}


void PhysicsConstraint::SetPhysicsBodyA(PhysicsBody * _physicsBodyA)
{
    m_physicsBodyA = _physicsBodyA;
}

void PhysicsConstraint::SetPhysicsBodyB(PhysicsBody * _physicsBodyB)
{
    m_physicsBodyB = _physicsBodyB;
}

void PhysicsConstraint::SetPhysicsProperty(std::shared_ptr<SimObjectProperties> _physicsBodyProperties)
{
    m_physicsBodyProperties = _physicsBodyProperties;
}

PhysicsBody * PhysicsConstraint::GetPhysicsBodyA()
{
    return m_physicsBodyA;
}

PhysicsBody * PhysicsConstraint::GetPhysicsBodyB()
{
    return m_physicsBodyB;
}

void PhysicsConstraint::AddConstraintsToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    if(_dynamicWorld != nullptr)
    {
        m_dynamicWorld = _dynamicWorld;
    }

    if(m_dynamicWorld != nullptr)
    {
        for( auto ic : m_externalConstraints)
        {
            m_dynamicWorld->addConstraint(ic.get());
        }
    }
}

void PhysicsConstraint::RemoveConstraintsFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    if(_dynamicWorld != nullptr)
    {
        m_dynamicWorld = _dynamicWorld;
    }

    if(m_dynamicWorld != nullptr)
    {
        for( auto &&ic : m_externalConstraints)
        {
            m_dynamicWorld->removeConstraint(ic.get());
        }
    }
}

void PhysicsConstraint::Update()
{
    UpdateBreakingConstraints();
}

void PhysicsConstraint::UpdatePhysicsProps()
{
    DeleteConstraints();
    InitialiseConstraints();
}

void PhysicsConstraint::UpdateBreakingConstraints()
{
    // This is probably an inefficient way to do it
    for(unsigned int i=0; i<m_externalConstraints.size(); i++)
    {
        float impulse = m_externalConstraints[i]->getAppliedImpulse();
        if(impulse >= m_physicsBodyProperties->PhysBody.GlueStrength)
        {
            m_dynamicWorld->removeConstraint(m_externalConstraints[i].get());

        }
    }
}



void PhysicsConstraint::InitialiseConstraints()
{
    if(m_physicsBodyA == nullptr || m_physicsBodyB == nullptr)
    {
        return;
    }

    if(m_physicsBodyProperties == nullptr)
    {
        return;
    }

    auto rigidBodiesA = m_physicsBodyA->getRigidBodies();
    auto rigidBodiesB = m_physicsBodyB->getRigidBodies();

    // create constraints here
    if(rigidBodiesA.size() < 1 || rigidBodiesB.size() < 1)
    {
        return;
    }

    int i=0;
    for(auto &&sphereA : rigidBodiesA)
    {
        int j=0;
        for(auto &&sphereB : rigidBodiesB)
        {
            if (sphereA == sphereB)
            {
                j++;
                continue;
            }
            else
            {
                btTransform frameInA, frameInB;
                frameInA.setIdentity();
                frameInB.setIdentity();

                sphereA->getMotionState()->getWorldTransform(frameInA);
                sphereB->getMotionState()->getWorldTransform(frameInB);

                btVector3 pos1 = frameInA.getOrigin();
                btVector3 pos2 = frameInB.getOrigin();

                float r1 = dynamic_cast<btSphereShape*>(sphereA->getCollisionShape())->getRadius();
                float r2 = dynamic_cast<btSphereShape*>(sphereB->getCollisionShape())->getRadius();


                float dist = pos1.distance(pos2);
                if(dist <= m_physicsBodyProperties->PhysBody.constraintRadius*(r1+r2))
                {
                    m_externalConstraints.push_back(CreateConstraint(*sphereA, *sphereB));
                }

                j++;
            }
        }
        i++;
    }

}

void PhysicsConstraint::DeleteConstraints()
{
    if(m_dynamicWorld != nullptr)
    {
        RemoveConstraintsFromDynamicWorld(m_dynamicWorld);
    }

    m_externalConstraints.clear();
}


std::shared_ptr<btTypedConstraint> PhysicsConstraint::CreateConstraint(btRigidBody &rigidA, btRigidBody &rigidB)
{
    btTransform frameInA, frameInB;
    frameInA.setIdentity();
    frameInB.setIdentity();

    rigidA.getMotionState()->getWorldTransform(frameInA);
    rigidB.getMotionState()->getWorldTransform(frameInB);

    btTransform invCentreOfMassB = rigidB.getCenterOfMassTransform().inverse();
    btTransform globalFrameA = rigidA.getCenterOfMassTransform() * frameInA;
    frameInB = invCentreOfMassB * globalFrameA;

    std::shared_ptr<btFixedConstraint> constraint = std::shared_ptr<btFixedConstraint>(new btFixedConstraint(rigidA, rigidB, frameInA, frameInB));
    constraint->setBreakingImpulseThreshold(m_physicsBodyProperties->PhysBody.GlueStrength);
    return constraint;

}
