#include "include/simobject.h"

SimObject::SimObject(const unsigned int _id, QOpenGLShaderProgram *_shaderProg, std::shared_ptr<PhysicsBodyProperties> _properties)
{
    m_id = _id;

    m_physicsBodyProperties = _properties;
    m_physBody = new PhysicsBody(m_id, m_physicsBodyProperties);
    m_mesh.SetShaderProg(_shaderProg);
    m_physMesh.SetShaderProg(_shaderProg);
}

SimObject::~SimObject()
{
    // Clean up time
    if(m_physBody)
    {
        delete m_physBody;
    }
}

void SimObject::LoadMesh(const std::__cxx11::string _meshFile)
{

    // Load mesh to render
    m_mesh.LoadMesh(_meshFile, 0, m_physicsBodyProperties);

    // Load physics bodies
    std::vector<glm::vec3> meshVerts;
    std::vector<glm::ivec3> meshTris;
    m_mesh.GetMeshVerts(meshVerts);
    m_mesh.GetMeshTris(meshTris);
    m_physBody->LoadMesh(meshVerts, meshTris);

    // Load physics mesh to render
    std::vector<glm::vec4> spheres;
    m_physBody->GetSpheres(spheres);
    m_physMesh.LoadSpheres(spheres);
}

void SimObject::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    m_physBody->AddToDynamicWorld(_dynamicWorld);
}


void SimObject::Draw()
{
    // Draw the mesh
    if(m_physicsBodyProperties)
    {
        m_mesh.SetDrawMesh(m_physicsBodyProperties->drawMesh);
        m_mesh.SetWireframe(m_physicsBodyProperties->drawWireframe);
        m_mesh.SetColour(m_physicsBodyProperties->colour);
    }
    m_mesh.DrawMesh();

    // Draw the phsyics mesh (spheres)
    std::vector<glm::mat4> sphereMats;
    m_physBody->GetSpheresMatrices(sphereMats);
    m_physMesh.UpdateSphereMats(sphereMats);
    if(m_physicsBodyProperties)
    {
        m_physMesh.SetDrawMesh(m_physicsBodyProperties->drawSpheres);
        m_physMesh.SetColour(m_physicsBodyProperties->colour);
    }
    m_physMesh.DrawMesh();

}
