#include "include/Visualisation/renderphysicsbody.h"

#include <glm/gtx/transform.hpp>


RenderPhysicsBody::RenderPhysicsBody(QOpenGLShaderProgram *_shaderProg) : RenderMesh(_shaderProg)
{
    if(_shaderProg)
    {
        m_shaderProg = _shaderProg;
    }
    m_meshLoaded = false;
}

RenderPhysicsBody::~RenderPhysicsBody()
{
    m_meshVAO.destroy();
    m_meshVBO.destroy();
    m_meshNBO.destroy();
    m_meshIBO.destroy();
    m_meshModelMatInstanceBO.destroy();

    m_shaderProg = 0;
}

void RenderPhysicsBody::LoadMesh(const std::string _meshFile, QOpenGLShaderProgram *_shaderProg)
{

}

void RenderPhysicsBody::LoadSpheres(const std::vector<glm::vec4> &_spheres, QOpenGLShaderProgram *_shaderProg)
{
    m_meshLoaded = true;

    if(_shaderProg != 0)
    {
        m_shaderProg = _shaderProg;
    }

    m_modelMat = glm::mat4(1.0f);
    m_wireframe = false;
    m_drawMesh = true;
    m_colour = glm::vec3(0.8f,0.4f,0.4f);

    //----------------------------------------------------------------------
    // Initialise mesh
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

    //----------------------------------------------------------------------
    // Iitialise GL VAO and buffers
    InitVAO();

}

void RenderPhysicsBody::DrawMesh()
{
    if(!m_drawMesh || !m_meshLoaded){return;}

    glUniform3fv(m_colourLoc, 1, &m_colour[0]);


    m_meshVAO.bind();
    m_meshModelMatInstanceBO.bind();
    m_meshModelMatInstanceBO.allocate(&m_sphereModelMats[0], m_sphereModelMats.size() * sizeof(glm::mat4));
    m_meshModelMatInstanceBO.release();

    glDrawElementsInstanced(GL_TRIANGLES, m_meshTris.size()*3, GL_UNSIGNED_INT, &m_meshTris[0], m_sphereModelMats.size());
    m_meshVAO.release();

}


void RenderPhysicsBody::UpdateSphereMats(const std::vector<glm::mat4> &_sphereMats)
{
    for( unsigned int i=0; i<_sphereMats.size(); i++)
    {
        float r = m_sphereRad[i];
        m_sphereModelMats[i] = m_modelMat * glm::scale(_sphereMats[i], glm::vec3(r, r, r));
        i++;
    }
}


void RenderPhysicsBody::InitVAO()
{
    if(m_shaderProg->bind())
    {
        m_modelMatrixLoc = m_shaderProg->attributeLocation("modelMatrix");
        m_colourLoc = m_shaderProg->uniformLocation("colour");
        glUniform3fv(m_colourLoc, 1, &m_colour[0]);

        m_meshVAO.create();
        m_meshVAO.bind();


        // Setup element array.
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


        // set up normal buffer object
        m_meshNBO.create();
        m_meshNBO.bind();
        m_meshNBO.allocate(&m_meshNorms[0], m_meshNorms.size() * sizeof(glm::vec3));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), 0);
        m_meshNBO.release();


        // set up instance model matrix buffer object
        m_meshModelMatInstanceBO.create();
        m_meshModelMatInstanceBO.bind();
        m_meshModelMatInstanceBO.allocate(&m_sphereModelMats[0], m_sphereModelMats.size() * sizeof(glm::mat4));

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
