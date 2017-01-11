#include "include/simobject.h"
#include "include/Utils/meshloader.h"

SimObject::SimObject(unsigned int _id, std::shared_ptr<SimObjectProperties> _properties)
{
    m_id = _id;

    m_physicsBodyProperties = _properties;
}

SimObject::~SimObject()
{
    m_physicsBodyProperties = nullptr;
}

void SimObject::LoadMesh(const std::string _meshFile)
{

    std::vector<Mesh> mesh = MeshLoader::LoadMesh(_meshFile);
    printf("Mesh loaded \n");

    if(mesh.size() > 0)
    {
        m_mesh.resize(mesh.size());
        m_physMesh.resize(mesh.size());
        m_physBody.resize(mesh.size());

        for(int i=0;i<mesh.size();i++)
        {
            // Load mesh to render
            m_mesh[i].LoadMesh(mesh[i], m_physicsBodyProperties);

            // Load physics bodies
            m_physBody[i].SetID(m_id+i);
            m_physBody[i].LoadMesh(mesh[i], m_physicsBodyProperties);

            // Load physics mesh to render
            m_physMesh[i].LoadMesh(m_physBody[i], m_physicsBodyProperties);


            m_mesh[i].InitialiseSkinWeights(m_physBody[i]);
        }

        // Load glue constraints
//        for(int i=0;i<mesh.size();i++)
//        {
//            for(int j=0;j<mesh.size();j++)
//            {
//                if (i==j)
//                {
//                    continue;
//                }

//                m_externalConstraints.push_back(new PhysicsConstraint(&m_physBody[i], &m_physBody[j], m_physicsBodyProperties));
//            }
//        }
    }
}

void SimObject::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld, const bool _selfCollisions)
{
    for(auto &&pb : m_physBody)
    {
        pb.AddToDynamicWorld(_dynamicWorld);
    }
//    for(auto &&ec : m_externalConstraints)
//    {
//        ec->AddConstraintsToDynamicWorld(_dynamicWorld);
//    }
}

void SimObject::RemoveFromDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    for(auto &&pb : m_physBody)
    {
        pb.RemoveFromDynamicWorld(_dynamicWorld);
    }

//    for(auto &&ec : m_externalConstraints)
//    {
//        ec->RemoveConstraintsFromDynamicWorld(_dynamicWorld);
//    }
}

void SimObject::Update()
{
    for(auto &&pb : m_physBody)
    {
        pb.Update();
    }

//    for(auto &&ec : m_externalConstraints)
//    {
//        ec->Update();
//    }
}

void SimObject::Draw()
{
    for(unsigned int i=0; i<m_mesh.size(); i++)
    {
        // Draw the mesh
        m_mesh[i].Skin(m_physBody[i]);
        m_mesh[i].DrawMesh();

        // Draw the phsyics mesh (spheres)
        m_physMesh[i].UpdateMesh(m_physBody[i]);
        m_physMesh[i].DrawMesh();
    }


    for(unsigned int i=0; i<m_physMesh.size(); i++)
    {
        // Draw the phsyics mesh (spheres)
        m_physMesh[i].UpdateMesh(m_physBody[i]);
        m_physMesh[i].DrawMesh();
    }

}


void SimObject::Reset()
{
    for(auto &&pb : m_physBody)
    {
        pb.Reset();
    }
}

void SimObject::Cache()
{
    for(auto &&pb : m_physBody)
    {
        pb.Cache(m_cachedSim);
    }
}


void SimObject::UpdatePhysicsProps()
{
    for (unsigned int i=0; i<m_mesh.size(); i++)
    {
        m_physBody[i].UpdatePhysicsProps();
        m_physMesh[i].UpdatePhysicsProps(m_physBody[i]);

        std::vector<glm::vec4> spheres;
        m_physBody[i].GetUpdatedSpheres(spheres);
        m_mesh[i].InitialiseSkinWeights(spheres);
    }

//    for(auto &&ec : m_externalConstraints)
//    {
//        ec->UpdatePhysicsProps();
//    }
}

void SimObject::UpdateRenderProps()
{
    if(m_physicsBodyProperties != nullptr)
    {
        for (unsigned int i=0; i<m_mesh.size(); i++)
        {
        m_mesh[i].SetDrawMesh(m_physicsBodyProperties->RenderMesh.drawMesh);
        m_mesh[i].SetWireframe(m_physicsBodyProperties->RenderMesh.drawWireframe);
        m_mesh[i].SetColour(m_physicsBodyProperties->RenderMesh.colour);
        }

        for (unsigned int i=0; i<m_physMesh.size(); i++)
        {
        m_physMesh[i].SetDrawMesh(m_physicsBodyProperties->RenderMesh.drawSpheres);
        m_physMesh[i].SetColour(m_physicsBodyProperties->RenderMesh.colour);
        }
    }
}
