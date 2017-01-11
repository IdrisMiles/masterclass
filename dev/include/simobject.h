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


/// @class SimObject class. This class encapsulates all elements of a single simulated mesh, rendering of a skinned mesh and physics rigid bodies.
class SimObject
{
public:
    /// @brief Constructor.
    /// @param _id : the start id for physics bodies to enable Bullets collision masking system.
    /// @param _properties : The simulation object properties.
    SimObject(unsigned int _id = 0, std::shared_ptr<SimObjectProperties> _properties = nullptr);

    /// @brief Destructor.
    ~SimObject();

    /// @brief Method to load a simulation object from the mesh file.
    /// @param _meshFile : the file to load in.
    void LoadMesh(const std::string _meshFile);

    /// @brief Method to add all the physics objects related to this simulation object to the dynamic world so they are simulated.
    /// @param _dynamicWorld : The Bullet dynamic world we are adding our physics objects to.
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);

    /// @brief Method to remove all the physics objects related to this simulation object from the dynamic world so they are not simulated.
    /// @param _dynamicWorld : The Bullet dynamic world we are removing our physics objects from.
    void RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);

    /// @brief Method to update various components in the simulation object, to be called every simulation tick.
    void Update();

    /// @brief Method to draw the simulated object
    void Draw();

    /// @brief Method to reset the simulated object back to its start state
    void Reset();

    void Cache();

    /// @brief Method to update physics properties of the PhysicsBody
    void UpdatePhysicsProps();

    /// @briefMethod to update the rendering properties in the RenderMesh classes
    void UpdateRenderProps();

    /// @brief Method to get the number of PhysicsBody's in this SimObject. Typically 1 unless pre-fractured geometry has been loaded in.
    unsigned int NumPhysicsBodies(){return m_physBody.size();}


private:
    std::vector<RenderMesh> m_mesh;
    std::vector<RenderPhysicsBody> m_physMesh;
    std::vector<PhysicsBody> m_physBody;
//    std::vector<PhysicsConstraint*> m_externalConstraints;
    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;

    CachedSimObject m_cachedSim;

    unsigned int m_id;


};

#endif // SIMOBJECT_H
