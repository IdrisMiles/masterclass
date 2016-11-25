#ifndef SIMOBJECT_H
#define SIMOBJECT_H

#include "include/Physics//physicsbody.h"
#include "include/UserInterface//physicsbodyproperties.h"
#include "include/Physics//physicsconstraint.h"
#include "include/SpherePacking/meshspherepacker.h"
#include "include/Visualisation/rendermesh.h"
#include "include/Visualisation/renderphysicsbody.h"

class SimObject
{
public:
    SimObject(const unsigned int _id = 0, QOpenGLShaderProgram *_shaderProg = nullptr, std::shared_ptr<PhysicsBodyProperties> _properties = nullptr);
    ~SimObject();

    void LoadMesh(const std::string _meshFile);
    void AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld);
    void Draw();


private:
    RenderMesh m_mesh;
    RenderPhysicsBody m_physMesh;
    PhysicsBody *m_physBody;
    std::shared_ptr<PhysicsBodyProperties> m_physicsBodyProperties;


    unsigned int m_id;


};

#endif // SIMOBJECT_H
