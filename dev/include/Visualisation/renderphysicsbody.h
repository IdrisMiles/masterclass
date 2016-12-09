#ifndef RENDERPHYSICSBODY_H
#define RENDERPHYSICSBODY_H

#include "include/Visualisation/rendermesh.h"
#include "include/Physics/physicsbody.h"

class RenderPhysicsBody : public RenderMesh
{
public:
    RenderPhysicsBody(QOpenGLShaderProgram *_shaderProg = 0);
    virtual ~RenderPhysicsBody();

    virtual void LoadMesh(const PhysicsBody &_physBody, QOpenGLShaderProgram *_shaderProg = 0, std::shared_ptr<SimObjectProperties> _physicsBodyProperties = nullptr);
    virtual void DrawMesh() override;
    void UpdateMesh(const PhysicsBody &_physBody);
    void UpdatePhysicsProps(const PhysicsBody &_physBody);

protected:
    void LoadSpheres(const std::vector<glm::vec4> &_spheres);
    void UpdateSphereMats(const std::vector<glm::mat4> &_sphereMats);
    void AppendSphereVerts(glm::vec3 _pos = glm::vec3(0.0f,0.0f,0.0f), float _radius = 1.0f, int _stacks = 16, int _slices = 32);
    void DeleteMesh();


    std::vector<float> m_sphereRad;
    std::vector<glm::mat4> m_sphereModelMats;
};

#endif // RENDERPHYSICSBODY_H
