#ifndef SIMOBJECT_H
#define SIMOBJECT_H

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
    SimObject(const unsigned int _id = 0, QOpenGLShaderProgram *_shaderProg = nullptr, std::shared_ptr<SimObjectProperties> _properties = nullptr);
    ~SimObject();

    void LoadMesh(const std::string _meshFile);
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions = false);
    void RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void Draw();
    void Reset();
    void Cache();
    void UpdatePhysicsProps();
    void UpdateRenderProps();


private:
    RenderMesh m_mesh;
    std::vector<RenderMesh*> m_meshes;
    RenderPhysicsBody m_physMesh;
    PhysicsBody m_physBody;
    CachedSimObject m_cachedSim;
    std::shared_ptr<SimObjectProperties> m_physicsBodyProperties;


    unsigned int m_id;
    QOpenGLShaderProgram* m_shaderProg;


};

#endif // SIMOBJECT_H
