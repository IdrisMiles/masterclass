#ifndef PHYSICSCONSTRAINT_H
#define PHYSICSCONSTRAINT_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>

// Bullets includes
#include <bullet/btBulletDynamicsCommon.h>


class PhysicsBody;
class SimObjectProperties;

class PhysicsConstraint
{
public:
//    PhysicsConstraint(std::shared_ptr<PhysicsBody> _physicsBodyA = nullptr, std::shared_ptr<PhysicsBody> _physicsBodyB = nullptr, std::shared_ptr<SimObjectProperties> m_physicsBodyProperties = nullptr);
    PhysicsConstraint(PhysicsBody *_physicsBodyA = nullptr, PhysicsBody *_physicsBodyB = nullptr, std::shared_ptr<SimObjectProperties> _physicsBodyProperties = nullptr);

    void SetPhysicsBodyA(PhysicsBody * _physicsBodyA);
    void SetPhysicsBodyB(PhysicsBody * _physicsBodyB);
    void SetPhysicsProperty(std::shared_ptr<SimObjectProperties> _physicsBodyProperties);

    PhysicsBody * GetPhysicsBodyA();
    PhysicsBody * GetPhysicsBodyB();

    void AddConstraintsToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void RemoveConstraintsFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);

    void Update();
    void UpdatePhysicsProps();


private:
    void InitialiseConstraints();
    void DeleteConstraints();
    void UpdateBreakingConstraints();
    std::shared_ptr<btTypedConstraint> CreateConstraint(btRigidBody &rigidA, btRigidBody &rigidB);

    PhysicsBody *m_physicsBodyA;
    PhysicsBody *m_physicsBodyB;
    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;


    std::vector<std::shared_ptr<btTypedConstraint>> m_externalConstraints;
    btDiscreteDynamicsWorld * m_dynamicWorld;

};

#endif // PHYSICSCONSTRAINT_H
