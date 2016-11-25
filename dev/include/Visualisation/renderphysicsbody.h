#ifndef RENDERPHYSICSBODY_H
#define RENDERPHYSICSBODY_H

#include "include/Visualisation/rendermesh.h"

class RenderPhysicsBody : public RenderMesh
{
public:
    RenderPhysicsBody(QOpenGLShaderProgram *_shaderProg = 0);
    virtual ~RenderPhysicsBody();

    virtual void LoadMesh(const std::string _meshFile, QOpenGLShaderProgram *_shaderProg = 0);
    void LoadSpheres(const std::vector<glm::vec4> &_spheres, QOpenGLShaderProgram *_shaderProg = 0);
    virtual void DrawMesh() override;
    void UpdateSphereMats(const std::vector<glm::mat4> &_sphereMats);


protected:
    virtual void InitVAO() override;
    void AppendSphereVerts(glm::vec3 _pos = glm::vec3(0.0f,0.0f,0.0f), float _radius = 1.0f, int _stacks = 16, int _slices = 32);


    std::vector<float> m_sphereRad;
    std::vector<glm::mat4> m_sphereModelMats;
};

#endif // RENDERPHYSICSBODY_H
