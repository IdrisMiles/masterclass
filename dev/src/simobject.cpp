#include "include/simobject.h"
#include "include/Utils/meshloader.h"

SimObject::SimObject(unsigned int _id, std::shared_ptr<SimObjectProperties> _properties)
{
    m_id = _id;

    m_physicsBodyProperties = _properties;
//    m_physBody.SetID(m_id);
}

SimObject::~SimObject()
{
    m_physicsBodyProperties = nullptr;
}

void SimObject::LoadMesh(const std::string _meshFile)
{

    std::vector<Mesh> mesh = MeshLoader::LoadMesh(_meshFile);
    printf("Mesh loaded \n");

    int i=0;

    if(i < mesh.size())
    {
//        // Load mesh to render
//        m_mesh.LoadMesh(mesh[i], m_physicsBodyProperties);

//        // Load physics bodies
//        m_physBody.LoadMesh(mesh[i], m_physicsBodyProperties);

//        // Load physics mesh to render
//        m_physMesh.LoadMesh(m_physBody, m_physicsBodyProperties);

//        m_mesh.InitialiseSkinWeights(m_physBody);

        m_mesh.resize(mesh.size());
        m_physMesh.resize(mesh.size());
        m_physBody.resize(mesh.size());

        for(i=0;i<mesh.size();i++)
        {
            // Load mesh to render
            m_mesh[i].LoadMesh(mesh[i], m_physicsBodyProperties);

            // Load physics bodies
            m_physBody[i].SetID(m_id++);
            m_physBody[i].LoadMesh(mesh[i], m_physicsBodyProperties);

            // Load physics mesh to render
            m_physMesh[i].LoadMesh(m_physBody[i], m_physicsBodyProperties);


            m_mesh[i].InitialiseSkinWeights(m_physBody[i]);
        }
    }
}

void SimObject::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
//    m_physBody.AddToDynamicWorld(_dynamicWorld);
    for(auto &&pb : m_physBody)
    {
        pb.AddToDynamicWorld(_dynamicWorld);
    }
}

void SimObject::RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
//    m_physBody.RemoveFromDynamicWorld(_dynamicWorld);
    for(auto &&pb : m_physBody)
    {
        pb.RemoveFromDynamicWorld(_dynamicWorld);
    }
}

void SimObject::Update()
{
//    m_physBody.Update();
    for(auto &&pb : m_physBody)
    {
        pb.Update();
    }
}

void SimObject::Draw()
{
//    // Draw the mesh
//    std::vector<glm::mat4> sphereMats;
//    m_physBody.GetSpheresMatrices(sphereMats);

//    std::vector<glm::vec4> spheres;
//    m_physBody.GetUpdatedSpheres(spheres);
//    m_mesh.Skin(spheres);
//    m_mesh.DrawMesh();

//    // Draw the phsyics mesh (spheres)
//    m_physMesh.UpdateMesh(m_physBody);
//    m_physMesh.DrawMesh();


    for(unsigned int i=0; i<m_mesh.size(); i++)
    {
        // Draw the mesh
        std::vector<glm::vec4> spheres;
        m_physBody[i].GetUpdatedSpheres(spheres);
        m_mesh[i].Skin(spheres);
        m_mesh[i].DrawMesh();

        // Draw the phsyics mesh (spheres)
        m_physMesh[i].UpdateMesh(m_physBody[i]);
        m_physMesh[i].DrawMesh();
    }

}


void SimObject::Reset()
{
//    m_physBody.Reset();
    for(auto &&pb : m_physBody)
    {
        pb.Reset();
    }
}

void SimObject::Cache()
{
//    m_physBody.Cache(m_cachedSim);
    for(auto &&pb : m_physBody)
    {
        pb.Cache(m_cachedSim);
    }
}


void SimObject::UpdatePhysicsProps()
{
//    m_physBody.UpdatePhysicsProps();
//    m_physMesh.UpdatePhysicsProps(m_physBody);

//    std::vector<glm::vec4> spheres;
//    m_physBody.GetUpdatedSpheres(spheres);
//    m_mesh.InitialiseSkinWeights(spheres);

    for (unsigned int i=0; i<m_mesh.size(); i++)
    {
        m_physBody[i].UpdatePhysicsProps();
        m_physMesh[i].UpdatePhysicsProps(m_physBody[i]);

        std::vector<glm::vec4> spheres;
        m_physBody[i].GetUpdatedSpheres(spheres);
        m_mesh[i].InitialiseSkinWeights(spheres);
    }
}

void SimObject::UpdateRenderProps()
{
    if(m_physicsBodyProperties)
    {
        for (unsigned int i=0; i<m_mesh.size(); i++)
        {
        m_mesh[i].SetDrawMesh(m_physicsBodyProperties->RenderMesh.drawMesh);
        m_mesh[i].SetWireframe(m_physicsBodyProperties->RenderMesh.drawWireframe);
        m_mesh[i].SetColour(m_physicsBodyProperties->RenderMesh.colour);

        m_physMesh[i].SetDrawMesh(m_physicsBodyProperties->RenderMesh.drawSpheres);
        m_physMesh[i].SetColour(m_physicsBodyProperties->RenderMesh.colour);
        }
    }
}
