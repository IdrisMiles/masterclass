#ifndef SIMOBJECT_H
#define SIMOBJECT_H

#include <vector>
#include "include/Physics//physicsbody.h"
#include "include/UserInterface/simobjectproperties.h"
#include "include/Physics/physicsconstraint.h"
#include "include/SpherePacking/meshspherepacker.h"
#include "include/Visualisation/rendermesh.h"
#include "include/Visualisation/renderphysicsbody.h"
#include "include/Physics/cachedsim.h"

class SimObject
{
public:
    SimObject(unsigned int _id = 0, std::shared_ptr<SimObjectProperties> _properties = nullptr);
    ~SimObject();

    void LoadMesh(const std::string _meshFile);
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void Update();
    void Draw();
    void Reset();
    void Cache();
    void UpdatePhysicsProps();
    void UpdateRenderProps();


private:
//    RenderMesh m_mesh;
    std::vector<RenderMesh> m_mesh;
//    RenderPhysicsBody m_physMesh;
    std::vector<RenderPhysicsBody> m_physMesh;
//    PhysicsBody m_physBody;
    std::vector<PhysicsBody> m_physBody;
    CachedSimObject m_cachedSim;
    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;


    unsigned int m_id;


};

#endif // SIMOBJECT_H
