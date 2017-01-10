#ifndef SIMOBJECT_H
#define SIMOBJECT_H

/// @author Idris Miles
/// @version 0.1.0
/// @date 10th January 2017

#include <vector>
#include "include/Physics//physicsbody.h"
#include "include/UserInterface/simobjectproperties.h"
#include "include/Physics/physicsconstraint.h"
#include "include/SpherePacking/meshspherepacker.h"
#include "include/Visualisation/rendermesh.h"
#include "include/Visualisation/renderphysicsbody.h"
#include "include/Physics/cachedsim.h"
#include "include/Physics/physicsconstraint.h"


/// SimObject class. This class encapsulates all elements of a single simulated mesh, rendering of a skinned mesh and physics rigid bodies.
class SimObject
{
public:
    /// @brief Constructor.
    /// @param _id : the start id for physics bodies to enable Bullets collision masking system.
    /// @param _properties : The simulation object properties.
    SimObject(unsigned int _id = 0, std::shared_ptr<SimObjectProperties> _properties = nullptr);

    /// @brief Destructor.
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
    unsigned int NumPhysicsBodies(){return m_physBody.size();}


private:
    std::vector<RenderMesh> m_mesh;
    std::vector<RenderPhysicsBody> m_physMesh;
    std::vector<PhysicsBody> m_physBody;
//    std::vector<PhysicsConstraint*> m_externalConstraints;
    CachedSimObject m_cachedSim;
    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;


    unsigned int m_id;


};

#endif // SIMOBJECT_H
