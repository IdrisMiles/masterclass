#ifndef PHYSICSCONSTRAINT_H
#define PHYSICSCONSTRAINT_H

#include <memory>
//#include "simobject.h"
class SimObject;

class PhysicsConstraint
{
public:
    PhysicsConstraint();

private:

    std::shared_ptr<SimObject> m_simObjectA;
    std::shared_ptr<SimObject> m_simObjectB;

};

#endif // PHYSICSCONSTRAINT_H
