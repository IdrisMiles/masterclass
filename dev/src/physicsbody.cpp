#include "physicsbody.h"

// ASSIMP includes
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <boost/filesystem.hpp>



PhysicsBody::PhysicsBody(const unsigned int _id, QOpenGLShaderProgram *_shaderProg, PhysicsBodyProperties *_properties)
{
    m_id = _id;
    m_shaderProg = _shaderProg;
    m_colourLoc = m_shaderProg->uniformLocation("colour");
    m_modelMatricesLoc = m_shaderProg->attributeLocation("modelMatrix");

    m_wireframe = false;
    m_drawMesh = true;
    m_drawSpheres = true;
    m_physicsBodyProperties = _properties;
    m_colour = glm::vec3(0.8f,0.4f,0.4f);
    //std::cout<<m_physicsBodyProperties->colour.x<<", "<<m_physicsBodyProperties->colour.y<<", "<<m_physicsBodyProperties->colour.z<<"\n";

}

//-------------------------------------------------------------------------------------------------------------------------
// initialisationa nd loading methods


void PhysicsBody::LoadMesh(const std::string _meshFile)
{


    m_modelMat = glm::mat4(1.0f);


    //----------------------------------------------------------------------
    // Get mesh info from file

    boost::filesystem::path file = _meshFile;
    if(file.extension() == ".abc")
    {
        // Open the alembic arrchive
        IArchive iArchive(Alembic::AbcCoreHDF5::ReadArchive(), _meshFile);
        IObject topObj = iArchive.getTop();

        // Get the mesh within the alembic archive
        IPolyMesh mesh;
        RecursiveTraverseAlembicGetPolyMesh(topObj, 0, 8, mesh);

        // Get the mesh schema and samples for the mesh we found
        IPolyMeshSchema meshSchema = mesh.getSchema();
        IPolyMeshSchema::Sample meshSample;
        meshSchema.get(meshSample);

        // Get index array for mesh
        uint32_t numIndices = meshSample.getFaceIndices()->size();
        for(uint32_t i=0;i<numIndices/3;i++)
        {
            int tri1 = (int)meshSample.getFaceIndices()->get()[(3*i) + 0];
            int tri2 = (int)meshSample.getFaceIndices()->get()[(3*i) + 1];
            int tri3 = (int)meshSample.getFaceIndices()->get()[(3*i) + 2];
            m_meshElementIndex.push_back(openvdb::Vec3I(tri1,tri2,tri3));
        }

        // Get mesh vertex positions and normals
        IN3fGeomParam normals = meshSchema.getNormalsParam();
        uint32_t numPoints = meshSample.getPositions()->size();
        for(uint32_t i=0;i<numPoints;i++)
        {
            Imath::V3f p = meshSample.getPositions()->get()[i];
            N3f n = normals.getIndexedValue().getVals()->get()[i];

            m_meshVerts.push_back(glm::vec3(p.x, p.y, p.z));
            m_meshNorms.push_back(glm::vec3(n.x, n.y, n.z));
        }
    }
    else
    {

        // Load mesh with ASSIMP
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(_meshFile,
                                                 aiProcess_GenSmoothNormals |
                                                 aiProcess_Triangulate |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType);
        if(!scene)
        {
            std::cout<<"Error loading "<<_meshFile<<" with assimp\n";
        }
        else
        {
            if(scene->HasMeshes())
            {
                unsigned int indexOffset = 0;
                for(unsigned int i=0; i<scene->mNumMeshes; i++)
                {
                    unsigned int numFaces = scene->mMeshes[i]->mNumFaces;
                    for(unsigned int f=0; f<numFaces; f++)
                    {
                        auto face = scene->mMeshes[i]->mFaces[f];
                        m_meshElementIndex.push_back(openvdb::Vec3I(face.mIndices[0]+indexOffset, face.mIndices[1]+indexOffset, face.mIndices[2]+indexOffset));
                    }
                    indexOffset += 3 * numFaces;

                    unsigned int numVerts = scene->mMeshes[i]->mNumVertices;
                    for(unsigned int v=0; v<numVerts; v++)
                    {
                        auto vert = scene->mMeshes[i]->mVertices[v];
                        auto norm = scene->mMeshes[i]->mNormals[v];
                        m_meshVerts.push_back(glm::vec3(vert.x, vert.y, vert.z));
                        m_meshNorms.push_back(glm::vec3(norm.x, norm.y, norm.z));
                    }
                }
            }
        }
    }



    //----------------------------------------------------------------------
    // Set up VAO and VBO's for mesh
    InitialiseRenderMesh();



    //----------------------------------------------------------------------
    // Setup spherical Rigidbodies and physics rendering mesh
    InitialiseSphericalRigidbodies();

    InitialiseInternalConstraints();



    //----------------------------------------------------------------------
    // Setup VAO and VBO's for spherical representation
    InitialiseRenderSpheres();




}


void PhysicsBody::InitialiseRenderMesh()
{

    m_shaderProg->bind();
    glUniform3fv(m_colourLoc, 1, &m_physicsBodyProperties->colour[0]);

    m_meshVAO.create();
    m_meshVAO.bind();

    m_meshIBO.create();
    m_meshIBO.bind();
    m_meshIBO.allocate(&m_meshElementIndex[0], m_meshElementIndex.size() * sizeof(int));
    m_meshIBO.release();

    // Setup our vertex buffer object.
    m_meshVBO.create();
    m_meshVBO.bind();
    m_meshVBO.allocate(&m_meshVerts[0], m_meshVerts.size() * sizeof(glm::vec3));

    glEnableVertexAttribArray( 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
    m_meshVBO.release();


    // Setup our normals buffer object.
    m_meshNBO.create();
    m_meshNBO.bind();
    m_meshNBO.allocate(&m_meshNorms[0], m_meshNorms.size() * sizeof(glm::vec3));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
    m_meshNBO.release();


    // set up instance model matrix buffer object
    m_meshModelMatInstanceBO.create();
    m_meshModelMatInstanceBO.bind();
    m_meshModelMatInstanceBO.allocate(&m_modelMat, 1 * sizeof(glm::mat4));

    glEnableVertexAttribArray(m_modelMatricesLoc+0);
    glEnableVertexAttribArray(m_modelMatricesLoc+1);
    glEnableVertexAttribArray(m_modelMatricesLoc+2);
    glEnableVertexAttribArray(m_modelMatricesLoc+3);
    glVertexAttribPointer(m_modelMatricesLoc+0, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), 0);
    glVertexAttribPointer(m_modelMatricesLoc+1, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(1 * sizeof(glm::vec4)));
    glVertexAttribPointer(m_modelMatricesLoc+2, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(m_modelMatricesLoc+3, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(m_modelMatricesLoc+0, 1);
    glVertexAttribDivisor(m_modelMatricesLoc+1, 1);
    glVertexAttribDivisor(m_modelMatricesLoc+2, 1);
    glVertexAttribDivisor(m_modelMatricesLoc+3, 1);

    m_meshModelMatInstanceBO.release();


    m_meshVAO.release();

    m_shaderProg->release();
}


void PhysicsBody::InitialiseSphericalRigidbodies()
{
    // Convert mesh to vdb volume
    openvdb::math::Transform::Ptr xform = openvdb::math::Transform::createLinearTransform();
    openvdb::tools::QuadAndTriangleDataAdapter<glm::vec3, openvdb::Vec3I> mesh(m_meshVerts, m_meshElementIndex);
    openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();
    grid = openvdb::tools::meshToVolume<openvdb::FloatGrid>(mesh, *xform);

    // Fill volume with spheres
    openvdb::tools::fillWithSpheres<openvdb::FloatGrid>(*grid, m_spheres, 1000, false, 1.0f,5.0);

    // create rigidbody for each sphere and set sphere rendering model matrix
    AppendSphereVerts(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    for(auto sphere : m_spheres)
    {
        float x = sphere.x();
        float y = sphere.y();
        float z = sphere.z();
        float r = sphere.w();
        m_initSpheres.push_back(glm::vec3(x,y,z));

        m_collisionShapes.push_back(new btSphereShape(r));
        m_collisionShapes.back()->setUserPointer((void*)this);
        m_motionStates.push_back(new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(x, y, z))));
        btScalar mass = 1.0f;
        btVector3 sphereInertia = btVector3(0,0,0);
        m_collisionShapes.back()->calculateLocalInertia(mass,sphereInertia);
        btRigidBody::btRigidBodyConstructionInfo sphereRBCI(/*(4.0/3.0) * M_PI * r*r*r**/ mass, m_motionStates.back(), m_collisionShapes.back(), sphereInertia);


        m_rigidBodies.push_back(new btRigidBody(sphereRBCI));
        m_sphereModelMats.push_back(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)), glm::vec3(r, r, r)));
    }
}

void PhysicsBody::InitialiseInternalConstraints()
{
    int constraintCheck[m_rigidBodies.size()][m_rigidBodies.size()];
    int i=0;
    for(auto sphere1 : m_rigidBodies)
    {
        int j=0;
        for(auto sphere2 : m_rigidBodies)
        {
            if (sphere1 == sphere2)// || constraintCheck[j][i])
            {
                continue;
            }


            btTransform frameInA, frameInB;
            frameInA.setIdentity();
            frameInB.setIdentity();

            sphere1->getMotionState()->getWorldTransform(frameInA);
            sphere2->getMotionState()->getWorldTransform(frameInB);

            btVector3 pos1 = frameInA.getOrigin();
            btVector3 pos2 = frameInB.getOrigin();

            float r1 = dynamic_cast<btSphereShape*>(sphere1->getCollisionShape())->getRadius();
            float r2 = dynamic_cast<btSphereShape*>(sphere2->getCollisionShape())->getRadius();


            float dist = pos1.distance(pos2);
            if(dist < 2*(r1+r2))
            {
                btTransform invCentreOfMassB = sphere2->getCenterOfMassTransform().inverse();
                btTransform globalFrameA = sphere1->getCenterOfMassTransform() * frameInA;
                frameInB = invCentreOfMassB * globalFrameA;


//                m_internalConstraints.push_back(new btSliderConstraint(*m_rigidBodies[i], *m_rigidBodies[j], localA, localB, false));
//                m_internalConstraints.push_back(new btPoint2PointConstraint(*sphere1, *sphere2, pos1, pos2));


//                m_internalConstraints.push_back(new btGeneric6DofSpringConstraint(*sphere1, *sphere2, frameInA, frameInB, true));
//                m_internalConstraints.back()->setBreakingImpulseThreshold(100000.0f);
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setLinearLowerLimit(btVector3(dist, dist, dist));
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setLinearUpperLimit(btVector3(dist, dist, dist));
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setAngularLowerLimit(btVector3( 0.0,0.0,0.0 ));
//                dynamic_cast<btGeneric6DofSpringConstraint*>(m_internalConstraints.back())->setAngularUpperLimit(btVector3( 0.0,0.0,0.0 ));




                btFixedConstraint *constraint = new btFixedConstraint(*sphere1, *sphere2, frameInA, frameInB);
                m_internalConstraints.push_back(constraint);

                constraintCheck[i][j] = 1;

            }


            j++;
        }
        i++;
    }
}

void PhysicsBody::InitialiseRenderSpheres()
{

    m_shaderProg->bind();
    glUniform3fv(m_colourLoc, 1, &m_physicsBodyProperties->colour[0]);

    m_sphereVAO.create();
    m_sphereVAO.bind();


    // Setup element array.
    m_sphereIBO.create();
    m_sphereIBO.bind();
    m_sphereIBO.allocate(&m_sphereElementIndex[0], m_sphereElementIndex.size() * sizeof(int));
    m_sphereIBO.release();


    // Setup our vertex buffer object.
    m_sphereVBO.create();
    m_sphereVBO.bind();
    m_sphereVBO.allocate(&m_sphereVerts[0], m_sphereVerts.size() * sizeof(glm::vec3));

    glEnableVertexAttribArray( 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
    m_sphereVBO.release();


    // set up normal buffer object
    m_sphereNBO.create();
    m_sphereNBO.bind();
    m_sphereNBO.allocate(&m_sphereNormals[0], m_sphereNormals.size() * sizeof(glm::vec3));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
    m_sphereNBO.release();


    // set up instance model matrix buffer object
    m_sphereModelMatInstanceBO.create();
    m_sphereModelMatInstanceBO.bind();
    m_sphereModelMatInstanceBO.allocate(&m_sphereModelMats[0], m_sphereModelMats.size() * sizeof(glm::mat4));

    glEnableVertexAttribArray(m_modelMatricesLoc+0);
    glEnableVertexAttribArray(m_modelMatricesLoc+1);
    glEnableVertexAttribArray(m_modelMatricesLoc+2);
    glEnableVertexAttribArray(m_modelMatricesLoc+3);
    glVertexAttribPointer(m_modelMatricesLoc+0, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), 0);
    glVertexAttribPointer(m_modelMatricesLoc+1, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(1 * sizeof(glm::vec4)));
    glVertexAttribPointer(m_modelMatricesLoc+2, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(m_modelMatricesLoc+3, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(m_modelMatricesLoc+0, 1);
    glVertexAttribDivisor(m_modelMatricesLoc+1, 1);
    glVertexAttribDivisor(m_modelMatricesLoc+2, 1);
    glVertexAttribDivisor(m_modelMatricesLoc+3, 1);
    m_sphereModelMatInstanceBO.release();


    m_sphereVAO.release();

    m_shaderProg->release();


}


void PhysicsBody::AppendSphereVerts(glm::vec3 _pos, float _radius, int _stacks, int _slices)
{
    /*
     * This code is based on an answer given from the site below:
     * http://gamedev.stackexchange.com/questions/16585/how-do-you-programmatically-generate-a-sphere
     *
    */


    int indexOffset = m_sphereVerts.size();

    // Generate sphere verts/normals
    for( int t = 1 ; t < _stacks-1 ; t++ )
    {
        float theta1 = ( (float)(t)/(_stacks-1) )*glm::pi<float>();

        for( int p = 0 ; p < _slices ; p++ )
        {
            float phi1 = ( (float)(p)/(_slices-1) )*2*glm::pi<float>();

            glm::vec3 vert = glm::vec3(sin(theta1)*cos(phi1), cos(theta1), -sin(theta1)*sin(phi1));
            m_sphereVerts.push_back(_pos + (_radius*vert));
            m_sphereNormals.push_back(vert);
        }
    }
    m_sphereVerts.push_back(_pos + (_radius * glm::vec3(0.0f, 1.0f, 0.0f)));
    m_sphereVerts.push_back(_pos + (_radius * glm::vec3(0.0f, -1.0f, 0.0f)));

    m_sphereNormals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    m_sphereNormals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));



    // Generate sphere element array
    indexOffset = indexOffset < 0 ? 0 : indexOffset;
    for( int t = 0 ; t < _stacks-3 ; t++ )
    {
        for( int p = 0 ; p < _slices-1 ; p++ )
        {
            m_sphereElementIndex.push_back(indexOffset + ((t  )*_slices + p  ));
            m_sphereElementIndex.push_back(indexOffset + ((t+1)*_slices + p+1));
            m_sphereElementIndex.push_back(indexOffset + ((t  )*_slices + p+1));

            m_sphereElementIndex.push_back(indexOffset + ((t  )*_slices + p  ));
            m_sphereElementIndex.push_back(indexOffset + ((t+1)*_slices + p  ));
            m_sphereElementIndex.push_back(indexOffset + ((t+1)*_slices + p+1));
        }
    }
    // element array for top and bottom row of tri's connecting to poles
    for( int p = 0 ; p < _slices-1 ; p++ )
    {
        m_sphereElementIndex.push_back(indexOffset + ((_stacks-2)*(_slices)));
        m_sphereElementIndex.push_back(indexOffset + (p));
        m_sphereElementIndex.push_back(indexOffset + (p+1));

        m_sphereElementIndex.push_back(indexOffset + ((_stacks-2)*_slices +1));
        m_sphereElementIndex.push_back(indexOffset + ((_stacks-3)*_slices +1+p));
        m_sphereElementIndex.push_back(indexOffset + ((_stacks-3)*_slices +p));
    }


}




//-------------------------------------------------------------------------------------------------------------------------
// render methods

void PhysicsBody::DrawMesh()
{
    if(!m_physicsBodyProperties->drawMesh){return;}

    glUniform3fv(m_colourLoc, 1, &m_physicsBodyProperties->colour[0]);

    m_meshVAO.bind();

    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe?GL_LINE:GL_FILL);
    glDrawElements(GL_TRIANGLES, 3*m_meshElementIndex.size(), GL_UNSIGNED_INT, &m_meshElementIndex[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_meshVAO.release();
}


void PhysicsBody::DrawSpheres()
{
    if(!m_physicsBodyProperties->drawSpheres){return;}

    glUniform3fv(m_colourLoc, 1, &m_physicsBodyProperties->colour[0]);

    // iterate through rigid bodies and update m_sphereModelMats
    int i=0;
    for( auto rb : m_rigidBodies)
    {
        btTransform worldTransform;
        rb->getMotionState()->getWorldTransform(worldTransform);
        worldTransform.getOpenGLMatrix(&m_sphereModelMats[i][0][0]);
        float r = dynamic_cast<btSphereShape*>(m_collisionShapes[i])->getRadius();
        m_sphereModelMats[i] = m_modelMat * glm::scale(m_sphereModelMats[i], glm::vec3(r, r, r));
        i++;
    }

    m_sphereVAO.bind();
    m_sphereModelMatInstanceBO.bind();
    m_sphereModelMatInstanceBO.allocate(&m_sphereModelMats[0], m_sphereModelMats.size() * sizeof(glm::mat4));
    m_sphereModelMatInstanceBO.release();

    glDrawElementsInstanced(GL_TRIANGLES, m_sphereElementIndex.size(), GL_UNSIGNED_INT, &m_sphereElementIndex[0], m_sphereModelMats.size());
    m_sphereVAO.release();
}


//-------------------------------------------------------------------------------------------------------------------------
// utility methods

void PhysicsBody::RecursiveTraverseAlembicGetPolyMesh(const IObject &_object, int _tab, int _depth, IPolyMesh &_outputMesh)
{
    // Handle depth limit
    if (_depth < 1)
    {
        return;
    }


    unsigned int numChildren = _object.getNumChildren();
    for (unsigned int i=0; i<numChildren; i++)
    {
        const MetaData childMD = _object.getChild(i).getMetaData();
        if(IPolyMeshSchema::matches(childMD))
        {
            // This node is a mesh node
            IPolyMesh mesh(_object,_object.getChild(i).getName());
            _outputMesh = mesh;
            return;
        }
        else
        {
            // Keep searching
            RecursiveTraverseAlembicGetPolyMesh(_object.getChild(i), _tab + 1, _depth - 1, _outputMesh);
        }
    }
}


void PhysicsBody::AddToDynamicWorld(btDiscreteDynamicsWorld * _dynamicWorld)
{
    for( auto rb : m_rigidBodies)
    {
        _dynamicWorld->addRigidBody(rb, 1<<m_id, ~(1<<m_id));
    }

    for( auto ic : m_internalConstraints)
    {
        _dynamicWorld->addConstraint(ic);
    }
}


void PhysicsBody::ToggleRenderMode()
{
    m_wireframe = !m_wireframe;
}

