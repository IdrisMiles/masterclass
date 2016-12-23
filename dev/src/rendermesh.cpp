#include "include/Visualisation/rendermesh.h"
#include <iostream>
#include <algorithm>

RenderMesh::RenderMesh(QOpenGLShaderProgram *_shaderProg)
{
    if(_shaderProg)
    {
        m_shaderProg = _shaderProg;
    }
    m_meshLoaded = false;
    m_vaoLoaded = false;
    m_skinWeightLoaded = false;
}

RenderMesh::~RenderMesh()
{
    m_meshVAO.destroy();
    m_meshVBO.destroy();
    m_meshNBO.destroy();
    m_meshIBO.destroy();
    m_meshModelMatInstanceBO.destroy();

    m_shaderProg = 0;
    m_physicsBodyProperties = nullptr;
}


void RenderMesh::LoadMesh(const Mesh _mesh, QOpenGLShaderProgram *_shaderProg, std::shared_ptr<SimObjectProperties> _physicsBodyProperties)
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

    m_meshVerts = _mesh.verts;
    m_meshNorms = _mesh.norms;
    m_meshTris = _mesh.tris;
    m_skinnedMeshVerts = m_meshVerts;

    //----------------------------------------------------------------------
    // Iitialise GL VAO and buffers
    if(!m_vaoLoaded)
    {
        CreateVAOs();
        UpdateVAOs();
    }
    else
    {
        UpdateVAOs();
    }

}

void RenderMesh::DrawMesh()
{
    if(!m_drawMesh || !m_meshLoaded){return;}

    glUniform3fv(m_colourLoc, 1, &m_colour[0]);

    m_meshVAO.bind();

    m_meshVBO.bind();
    m_meshVBO.allocate(&m_skinnedMeshVerts[0], m_skinnedMeshVerts.size() * sizeof(glm::vec3));
    m_meshVBO.release();

    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe?GL_LINE:GL_FILL);
    glDrawElements(GL_TRIANGLES, m_meshTris.size()*3, GL_UNSIGNED_INT, &m_meshTris[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_meshVAO.release();
}

void RenderMesh::InitialiseSkinWeights(const std::vector<glm::vec4> &_spheres)
{
    // Get control points
    for(unsigned int i=0; i<_spheres.size(); i++)
    {
        m_controlPoints.push_back(glm::vec3(_spheres[i].x, _spheres[i].y, _spheres[i].z));
    }


    // initialise vert control point weights
    m_skinWeights.resize(m_meshVerts.size());
    for(unsigned int i=0; i<m_meshVerts.size(); i++)
    {
        int maxNumNeighs = 4;
        std::vector<std::pair<int, float>> neighs(maxNumNeighs, std::make_pair(-1, FLT_MAX));
        float totalDist = 0.0f;
        float newtotalWeight = 0.0f;


        // Find closest neighbours
        m_skinWeights[i].resize(m_controlPoints.size());
        for(unsigned int j=0; j<m_controlPoints.size(); j++)
        {
            float dist = glm::distance(m_meshVerts[i], m_controlPoints[j]);
            if(dist < neighs.back().second)
            {
                neighs.pop_back();
                neighs.push_back(std::make_pair(j, dist));
                std::sort(neighs.begin(), neighs.end(), [](std::pair<int, float> a, std::pair<int, float> b){return a.second < b.second;});
            }

            m_skinWeights[i][j] = -1.0f;
        }

        // Get total distance
        for(unsigned int k=0; k<neighs.size(); k++)
        {
            totalDist += neighs[k].second;
        }

        // normalize the weights for this vert
        for(unsigned int k=0; k<neighs.size(); k++)
        {
            int controlPointId = neighs[k].first;
            float dist = neighs[k].second;

            m_skinWeights[i][controlPointId] = (1.0f - (dist / totalDist))/(maxNumNeighs-1);
            newtotalWeight += m_skinWeights[i][controlPointId];
        }

    }

    m_skinWeightLoaded = true;
}

void RenderMesh::Skin(const std::vector<glm::vec4> &_spheres)
{
    if(!m_skinWeightLoaded)
    {
        printf("skin weights have not been loaded - abort skinning\n");
        return;
    }

    if(_spheres.size() != m_controlPoints.size())
    {
        printf("Different number of control points now - abort skinning\n");
        return;
    }

    if(m_skinnedMeshVerts.size() != m_meshVerts.size())
    {
        printf("Different number of mesh verts now - abort skinning\n");
        return;
    }


    // Get our delta displacement
    std::vector<glm::vec3> deltaControlPoints;
    for(unsigned int i=0; i<_spheres.size(); i++)
    {
        deltaControlPoints.push_back(glm::vec3(_spheres[i].x, _spheres[i].y, _spheres[i].z) - m_controlPoints[i]);
    }

    for(unsigned int i=0; i<m_skinnedMeshVerts.size(); i++)
    {
        glm::vec3 delta = glm::vec3(0.0f, 0.0f, 0.0f);

        for(unsigned int j=0; j<deltaControlPoints.size(); j++)
        {
            if ( m_skinWeights[i][j] > 0.0f )
            {
                delta += m_skinWeights[i][j] * deltaControlPoints[j];
            }
        }

        m_skinnedMeshVerts[i] = m_meshVerts[i] + delta;
    }
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

void RenderMesh::CreateVAOs()
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
        m_meshIBO.release();

        // Setup our vertex buffer object.
        m_meshVBO.create();
        m_meshVBO.bind();
        glEnableVertexAttribArray( 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
        m_meshVBO.release();


        // Setup our normals buffer object.
        m_meshNBO.create();
        m_meshNBO.bind();
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
        m_meshNBO.release();


        // set up instance model matrix buffer object
        m_meshModelMatInstanceBO.create();
        m_meshModelMatInstanceBO.bind();
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

        m_vaoLoaded = true;
    }
}

void RenderMesh::DeleteVAOs()
{
    m_meshVAO.destroy();
    m_meshVBO.destroy();
    m_meshNBO.destroy();
    m_meshIBO.destroy();

    m_vaoLoaded = false;
}

void RenderMesh::UpdateVAOs()
{
    if(m_shaderProg->bind())
    {
        m_modelMatrixLoc = m_shaderProg->attributeLocation("modelMatrix");
        m_colourLoc = m_shaderProg->uniformLocation("colour");
        glUniform3fv(m_colourLoc, 1, &m_colour[0]);

        m_meshVAO.bind();

        m_meshIBO.bind();
        m_meshIBO.allocate(&m_meshTris[0], m_meshTris.size() * sizeof(glm::ivec3));
        m_meshIBO.release();

        // Setup our vertex buffer object.
        m_meshVBO.bind();
        m_meshVBO.allocate(&m_meshVerts[0], m_meshVerts.size() * sizeof(glm::vec3));
        glEnableVertexAttribArray( 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
        m_meshVBO.release();


        // Setup our normals buffer object.
        m_meshNBO.bind();
        m_meshNBO.allocate(&m_meshNorms[0], m_meshNorms.size() * sizeof(glm::vec3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
        m_meshNBO.release();


        // set up instance model matrix buffer object
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
