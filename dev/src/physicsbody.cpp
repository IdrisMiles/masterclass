#include "physicsbody.h"



PhysicsBody::PhysicsBody()
{

}

//-------------------------------------------------------------------------------------------------------------------------
// initialisationa nd loading methods


void PhysicsBody::LoadMesh(const std::string _meshFile, QOpenGLShaderProgram *_shaderProg)
{
    m_shaderProg = _shaderProg;

    m_colour = glm::vec3(0.8f, 0.4f, 0.4f);
    m_colourLoc = m_shaderProg->uniformLocation("colour");

    //----------------------------------------------------------------------
    // Get mesh info from file

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
    for (uint32_t i=0;i<numIndices;i++)
    {
        m_meshElementIndex.push_back((int)meshSample.getFaceIndices()->get()[i]);
    }

/*
    IXform x( child, kWrapExisting );
    XformSample xs;
    x.getSchema().get( xs );
    M44d mat=xs.getMatrix();
*/

    // Get mesh vertex positions and normals
    IN3fGeomParam normals = meshSchema.getNormalsParam();
    uint32_t numPoints = meshSample.getPositions()->size();
    for(uint32_t i=0;i<numPoints;i++)
    {
        Imath::V3f p = meshSample.getPositions()->get()[i];
        N3f n = normals.getIndexedValue().getVals()->get()[i];

        m_meshVerts.push_back(glm::vec3(p.x, p.y, p.z));
        m_meshVerts.push_back(glm::vec3(n.x, n.y, n.z));

    }


    //----------------------------------------------------------------------
    // Set up VAO and VBO's for mesh
    InitialiseRenderMesh();


    //----------------------------------------------------------------------
    // Create spherical fill of mesh


    //----------------------------------------------------------------------
    // Setup Rigidbody with spherical compound collision shape
    InitialisePhysicsMesh();



    //----------------------------------------------------------------------
    // Setup VAO and VBO's for spherical representation
    InitialiseRenderSpheres();




}


void PhysicsBody::InitialiseRenderMesh()
{
    // Qt SLOTS not necessarrily in main thread, need to get graphics context
    // makeCurrent();

    m_shaderProg->bind();
    glUniform3fv(m_colourLoc, 1, &m_colour[0]);

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
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), reinterpret_cast<void *>(1 * sizeof(glm::vec3)));

    m_meshVBO.release();
    m_meshVAO.release();

    m_shaderProg->release();

    //doneCurrent();
}


void PhysicsBody::InitialisePhysicsMesh()
{
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            for(int k=0;k<5;k++)
            {
                AppendSphereVerts(glm::vec3(i*3.0f,j*3.0f,k*3.0f), 1.0f, 16, 32);
            }

        }
    }
}

void PhysicsBody::InitialiseRenderSpheres()
{

    m_shaderProg->bind();
    glUniform3fv(m_colourLoc, 1, &m_colour[0]);

    m_sphereVAO.create();
    m_sphereVAO.bind();

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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);//reinterpret_cast<void *>(1 * sizeof(glm::vec3)));

    m_sphereNBO.release();

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

    std::cout<<m_sphereVerts.size()<<"\n";

}




//-------------------------------------------------------------------------------------------------------------------------
// render methods

void PhysicsBody::DrawMesh()
{
    m_meshVAO.bind();
    //glDrawArrays(GL_TRIANGLES, 0, m_meshVerts.size()/3);
    glDrawElements(GL_TRIANGLES, m_meshElementIndex.size(), GL_UNSIGNED_INT, &m_meshElementIndex[0]);
    m_meshVAO.release();
}


void PhysicsBody::DrawSpheres()
{

    m_sphereVAO.bind();
    glDrawElements(GL_TRIANGLES, m_sphereElementIndex.size(), GL_UNSIGNED_INT, &m_sphereElementIndex[0]);
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
