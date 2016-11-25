#include "include/Visualisation/rendermesh.h"

// ASSIMP includes
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// BOOST
#include <boost/filesystem.hpp>




RenderMesh::RenderMesh(QOpenGLShaderProgram *_shaderProg)
{
    if(_shaderProg)
    {
        m_shaderProg = _shaderProg;
    }
    m_meshLoaded = false;
}

RenderMesh::~RenderMesh()
{
    m_meshVAO.destroy();
    m_meshVBO.destroy();
    m_meshNBO.destroy();
    m_meshIBO.destroy();
    m_meshModelMatInstanceBO.destroy();

    m_shaderProg = 0;
}

void RenderMesh::LoadMesh(const std::string _meshFile, QOpenGLShaderProgram *_shaderProg, std::shared_ptr<PhysicsBodyProperties> _physicsBodyProperties)
{
    m_meshLoaded = true;
    if(m_physicsBodyProperties)
    {
        m_physicsBodyProperties = _physicsBodyProperties;
    }

    if(_shaderProg != 0)
    {
        m_shaderProg = _shaderProg;
    }

    m_modelMat = glm::mat4(1.0f);
    m_wireframe = false;
    m_drawMesh = true;
    m_colour = glm::vec3(0.8f,0.4f,0.4f);

    //----------------------------------------------------------------------
    // Get mesh info from file
    boost::filesystem::path file = _meshFile;

    if(file.extension() == ".abc")
    {
        LoadWithAlembic(_meshFile);
    }
    else
    {
        LoadWithASSIMP(_meshFile);

    }


    //----------------------------------------------------------------------
    // Iitialise GL VAO and buffers
    InitVAO();

}

void RenderMesh::DrawMesh()
{
    if(!m_drawMesh || !m_meshLoaded){return;}

    glUniform3fv(m_colourLoc, 1, &m_colour[0]);

    m_meshVAO.bind();
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe?GL_LINE:GL_FILL);
    glDrawElements(GL_TRIANGLES, m_meshTris.size()*3, GL_UNSIGNED_INT, &m_meshTris[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_meshVAO.release();
}

void RenderMesh::GetMeshVerts(std::vector<glm::vec3> &_outVerts) const
{
    _outVerts = m_meshVerts;
}

void RenderMesh::GetMeshTris(std::vector<glm::ivec3> &_outTris) const
{
    _outTris = m_meshTris;
}


void RenderMesh::SetShaderProg(QOpenGLShaderProgram *_shaderProg)
{

    if(_shaderProg)
    {
        m_shaderProg = _shaderProg;
    }
}

void RenderMesh::SetWireframe(const bool &_wireframe)
{
    m_wireframe = _wireframe;
}

void RenderMesh::SetDrawMesh(const bool &_drawMesh)
{
    m_drawMesh = _drawMesh;
}

void RenderMesh::SetColour(const glm::vec3 &_colour)
{
    m_colour = _colour;
}

//-----------------------------------------------------------------------------------------------------------------------

void RenderMesh::LoadWithAlembic(const std::string _meshFile)
{
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
        m_meshTris.push_back(glm::ivec3(tri1,tri2,tri3));
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

void RenderMesh::RecursiveTraverseAlembicGetPolyMesh(const IObject &_object, int _tab, int _depth, IPolyMesh &_outputMesh)
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

void RenderMesh::LoadWithASSIMP(const std::string _meshFile)
{
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
                    m_meshTris.push_back(glm::ivec3(face.mIndices[0]+indexOffset, face.mIndices[1]+indexOffset, face.mIndices[2]+indexOffset));
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

void RenderMesh::InitVAO()
{
    if(m_shaderProg->bind())
    {
        m_modelMatrixLoc = m_shaderProg->attributeLocation("modelMatrix");
        m_colourLoc = m_shaderProg->uniformLocation("colour");
        glUniform3fv(m_colourLoc, 1, &m_colour[0]);

        m_meshVAO.create();
        m_meshVAO.bind();

        m_meshIBO.create();
        m_meshIBO.bind();
        m_meshIBO.allocate(&m_meshTris[0], m_meshTris.size() * sizeof(glm::ivec3));
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

        glEnableVertexAttribArray(m_modelMatrixLoc+0);
        glEnableVertexAttribArray(m_modelMatrixLoc+1);
        glEnableVertexAttribArray(m_modelMatrixLoc+2);
        glEnableVertexAttribArray(m_modelMatrixLoc+3);
        glVertexAttribPointer(m_modelMatrixLoc+0, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), 0);
        glVertexAttribPointer(m_modelMatrixLoc+1, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(1 * sizeof(glm::vec4)));
        glVertexAttribPointer(m_modelMatrixLoc+2, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(2 * sizeof(glm::vec4)));
        glVertexAttribPointer(m_modelMatrixLoc+3, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(m_modelMatrixLoc+0, 1);
        glVertexAttribDivisor(m_modelMatrixLoc+1, 1);
        glVertexAttribDivisor(m_modelMatrixLoc+2, 1);
        glVertexAttribDivisor(m_modelMatrixLoc+3, 1);

        m_meshModelMatInstanceBO.release();


        m_meshVAO.release();

        m_shaderProg->release();
    }
}
