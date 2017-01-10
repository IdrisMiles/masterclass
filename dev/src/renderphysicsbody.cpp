#include "include/Visualisation/renderphysicsbody.h"
#include <glm/gtx/transform.hpp>
#include "Visualisation/openglscene.h"

RenderPhysicsBody::RenderPhysicsBody() : RenderMesh()
{
    m_meshLoaded = false;
    m_vaoLoaded = false;
}


RenderPhysicsBody::RenderPhysicsBody(const RenderPhysicsBody &_copy)
{
    m_meshLoaded = false;
    m_vaoLoaded = false;
}

RenderPhysicsBody::~RenderPhysicsBody()
{
    m_meshVAO.destroy();
    m_meshVBO.destroy();
    m_meshNBO.destroy();
    m_meshIBO.destroy();
    m_meshModelMatInstanceBO.destroy();

    delete m_shaderProg;
    m_shaderProg = 0;
    m_physicsBodyProperties = nullptr;
}

void RenderPhysicsBody::LoadMesh(const PhysicsBody &_physBody, std::shared_ptr<SimObjectProperties> _physicsBodyProperties)
{
    m_meshLoaded = true;
    if(_physicsBodyProperties != nullptr)
    {
        m_physicsBodyProperties = _physicsBodyProperties;
    }

    m_modelMat = glm::mat4(1.0f);

    if(m_physicsBodyProperties != nullptr)
    {
        m_wireframe = false;
        m_drawMesh = m_physicsBodyProperties->RenderMesh.drawSpheres;
        m_colour = m_physicsBodyProperties->RenderMesh.colour;
    }
    else
    {
        m_wireframe = false;
        m_drawMesh = true;
        m_colour = glm::vec3(0.6f,0.6f,0.6f);
    }


    //----------------------------------------------------------------------
    // Initialise mesh
    std::vector<glm::vec4> spheres;
    _physBody.GetOrigSpheres(spheres);
    LoadSpheres(spheres);


    //----------------------------------------------------------------------
    // Iitialise GL VAO and buffers
    if(!m_vaoLoaded)
    {
        CreateShader();
        CreateVAOs();
        UpdateVAOs();
    }
    else
    {
        UpdateVAOs();
    }
}

void RenderPhysicsBody::LoadSpheres(const std::vector<glm::vec4> &_spheres)
{
    m_sphereRad.clear();
    m_sphereModelMats.clear();
    m_meshVerts.clear();
    AppendSphereVerts(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    for(auto sphere : _spheres)
    {
        float x = sphere.x;
        float y = sphere.y;
        float z = sphere.z;
        float r = sphere.w;

        m_sphereRad.push_back(r);
        m_sphereModelMats.push_back(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)), glm::vec3(r, r, r)));
    }
}

void RenderPhysicsBody::DrawMesh()
{
    if(!m_drawMesh || !m_meshLoaded){return;}
    m_shaderProg->bind();

    glUniformMatrix4fv(m_projMatrixLoc, 1, false, &OpenGLScene::getProjMat()[0][0]);
    glUniformMatrix4fv(m_viewMatrixLoc, 1, false, &(OpenGLScene::getViewMat()*OpenGLScene::getModelMat())[0][0]);

    glUniform3fv(m_colourLoc, 1, &m_colour[0]);


    m_meshVAO.bind();
    m_meshModelMatInstanceBO.bind();
    m_meshModelMatInstanceBO.allocate(&m_sphereModelMats[0], m_sphereModelMats.size() * sizeof(glm::mat4));
    m_meshModelMatInstanceBO.release();

    glDrawElementsInstanced(GL_TRIANGLES, m_meshTris.size()*3, GL_UNSIGNED_INT, &m_meshTris[0], m_sphereModelMats.size());
    m_meshVAO.release();
    m_shaderProg->release();

}

void RenderPhysicsBody::UpdateMesh(const PhysicsBody &_physBody)
{
    std::vector<glm::mat4> sphereMats;
    _physBody.GetSpheresMatrices(sphereMats);
    UpdateSphereMats(sphereMats);
}

void RenderPhysicsBody::UpdateSphereMats(const std::vector<glm::mat4> &_sphereMats)
{
    for( unsigned int i=0; i<_sphereMats.size(); i++)
    {
        float r = m_sphereRad[i];
        m_sphereModelMats[i] = m_modelMat * glm::scale(_sphereMats[i], glm::vec3(r, r, r));
    }
}


void RenderPhysicsBody::AppendSphereVerts(glm::vec3 _pos, float _radius, int _stacks, int _slices)
{
    /*
     * This code is based on an answer given from the site below:
     * http://gamedev.stackexchange.com/questions/16585/how-do-you-programmatically-generate-a-sphere
     *
    */


    int indexOffset = m_meshVerts.size();

    // Generate sphere verts/normals
    for( int t = 1 ; t < _stacks-1 ; t++ )
    {
        float theta1 = ( (float)(t)/(_stacks-1) )*glm::pi<float>();

        for( int p = 0 ; p < _slices ; p++ )
        {
            float phi1 = ( (float)(p)/(_slices-1) )*2*glm::pi<float>();

            glm::vec3 vert = glm::vec3(sin(theta1)*cos(phi1), cos(theta1), -sin(theta1)*sin(phi1));
            m_meshVerts.push_back(_pos + (_radius*vert));
            m_meshNorms.push_back(vert);
        }
    }
    m_meshVerts.push_back(_pos + (_radius * glm::vec3(0.0f, 1.0f, 0.0f)));
    m_meshVerts.push_back(_pos + (_radius * glm::vec3(0.0f, -1.0f, 0.0f)));

    m_meshNorms.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    m_meshNorms.push_back(glm::vec3(0.0f, -1.0f, 0.0f));



    // Generate sphere element array
    indexOffset = indexOffset < 0 ? 0 : indexOffset;
    for( int t = 0 ; t < _stacks-3 ; t++ )
    {
        for( int p = 0 ; p < _slices-1 ; p++ )
        {
            glm::vec3 tri1(indexOffset + ((t  )*_slices + p  ),
                           indexOffset + ((t+1)*_slices + p+1),
                           indexOffset + ((t  )*_slices + p+1));
            m_meshTris.push_back(tri1);

            glm::vec3 tri2(indexOffset + ((t  )*_slices + p  ),
                           indexOffset + ((t+1)*_slices + p  ),
                           indexOffset + ((t+1)*_slices + p+1));
            m_meshTris.push_back(tri2);
        }
    }
    // element array for top and bottom row of tri's connecting to poles
    for( int p = 0 ; p < _slices-1 ; p++ )
    {
        glm::vec3 tri1(indexOffset + ((_stacks-2)*(_slices)),
                       indexOffset + (p),
                       indexOffset + (p+1));
        m_meshTris.push_back(tri1);

        glm::vec3 tri2(indexOffset + ((_stacks-2)*_slices +1),
                       indexOffset + ((_stacks-3)*_slices +1+p),
                       indexOffset + ((_stacks-3)*_slices +p));
        m_meshTris.push_back(tri2);
    }

}


void RenderPhysicsBody::DeleteMesh()
{
    m_meshLoaded = false;

    m_sphereRad.clear();
    m_sphereModelMats.clear();


}

void RenderPhysicsBody::UpdatePhysicsProps(const PhysicsBody &_physBody)
{
    DeleteMesh();
    LoadMesh(_physBody);
}
