#include "include/simobject.h"
#include "include/Utils/meshloader.h"

SimObject::SimObject(const unsigned int _id, QOpenGLShaderProgram *_shaderProg, std::shared_ptr<SimObjectProperties> _properties)
{
    m_id = _id;
    m_shaderProg = _shaderProg;

    m_physicsBodyProperties = _properties;
    m_physBody.SetID(m_id);
    m_mesh.SetShaderProg(_shaderProg);
    m_physMesh.SetShaderProg(_shaderProg);
}

SimObject::~SimObject()
{
    m_physicsBodyProperties = nullptr;
}

void SimObject::LoadMesh(const std::string _meshFile)
{

    Mesh mesh = MeshLoader::LoadMesh(_meshFile);

    // Load mesh to render
    m_mesh.LoadMesh(mesh, 0, m_physicsBodyProperties);

    // Load physics bodies
    m_physBody.LoadMesh(mesh, m_physicsBodyProperties);

    // Load physics mesh to render
    m_physMesh.LoadMesh(m_physBody, 0, m_physicsBodyProperties);


    std::vector<glm::vec4> spheres;
    m_physBody.GetSpheres(spheres);
    //m_mesh.InitialiseSkinWeights(spheres);
}

void SimObject::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    m_physBody.AddToDynamicWorld(_dynamicWorld);
}

void SimObject::RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    m_physBody.RemoveFromDynamicWorld(_dynamicWorld);
}


void SimObject::Draw()
{
    // Draw the mesh
    std::vector<glm::vec4> spheres;
    m_physBody.GetSpheres(spheres);
    m_mesh.Skin(spheres);
    m_mesh.DrawMesh();

    // Draw the phsyics mesh (spheres)
    m_physMesh.UpdateMesh(m_physBody);
    m_physMesh.DrawMesh();

}


void SimObject::Reset()
{
    m_physBody.Reset();
}

void SimObject::Cache()
{
    m_physBody.Cache(m_cachedSim);
}


void SimObject::UpdatePhysicsProps()
{
    m_physBody.UpdatePhysicsProps();
    m_physMesh.UpdatePhysicsProps(m_physBody);
}

void SimObject::UpdateRenderProps()
{
    if(m_physicsBodyProperties)
    {
        m_mesh.SetDrawMesh(m_physicsBodyProperties->RenderMesh.drawMesh);
        m_mesh.SetWireframe(m_physicsBodyProperties->RenderMesh.drawWireframe);
        m_mesh.SetColour(m_physicsBodyProperties->RenderMesh.colour);

        m_physMesh.SetDrawMesh(m_physicsBodyProperties->RenderMesh.drawSpheres);
        m_physMesh.SetColour(m_physicsBodyProperties->RenderMesh.colour);
    }
}
