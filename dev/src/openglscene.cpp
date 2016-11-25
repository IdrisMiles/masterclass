#include "include/Visualisation/openglscene.h"

#include <iostream>

#include <QOpenGLContext>
#include <QMouseEvent>
#include <QFileDialog>
#include <QKeyEvent>


OpenGLScene::OpenGLScene(QWidget *parent) : QOpenGLWidget(parent),
    m_xRot(0),
    m_yRot(0),
    m_zRot(0),
    m_zDis(200),
    m_shaderProg(0)
{
    QSurfaceFormat format;
    format.setVersion(4, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);

    setFormat(format);
    setFocus();
    setFocusPolicy(Qt::StrongFocus);

    m_runSim = false;
    m_simObjects.clear();
    initializePhysicsWorld();

    m_dt = 0.016f;
    m_physicsTimer = new QTimer(this);
    connect(m_physicsTimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));
}


OpenGLScene::~OpenGLScene()
{
    cleanup();
}

//--------------------------------------------------------------------------------------------------

QSize OpenGLScene::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize OpenGLScene::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void OpenGLScene::setXTranslation(int x)
{
    qNormalizeAngle(x);
    if (x != m_xDis) {
        m_xDis = x;
        update();
    }
}

void OpenGLScene::setYTranslation(int y)
{
    qNormalizeAngle(y);
    if (y != m_yDis) {
        m_yDis = y;
        update();
    }
}

void OpenGLScene::setZTranslation(int z)
{
    qNormalizeAngle(z);
    if (z != m_zDis) {
        m_zDis= z;
        update();
    }
}

void OpenGLScene::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        update();
    }
}

void OpenGLScene::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        update();
    }
}

void OpenGLScene::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        update();
    }
}


//--------------------------------------------------------------------------------------------------
// Cleanup methods

void OpenGLScene::cleanup()
{
    makeCurrent();
    cleanPhysicsWorld();
    m_groundVBO.destroy();
    m_groundVAO.destroy();
    delete m_shaderProg;
    m_shaderProg = 0;
    doneCurrent();
}


void OpenGLScene::cleanPhysicsWorld()
{
    // empty physics world
    m_dynamicWorld->removeRigidBody(m_groundRB);

    // clean physics bodies
    for(auto&& simObj : m_simObjects)
    {
        delete simObj;
    }
    m_simObjects.clear();


    // free physics memory
    delete m_groundRB;
    delete m_groundMotionState;
    delete m_groundShape;

    delete m_dynamicWorld;
    delete m_solver;
    delete m_collisionDispatcher;
    delete m_collisionConfig;
    delete m_broadPhase;
}


//--------------------------------------------------------------------------------------------------
// Initialization and loading methods

void OpenGLScene::loadSimObject(const std::string &_file, std::shared_ptr<PhysicsBodyProperties> _properties)
{
    makeCurrent();
    m_simObjects.push_back(new SimObject(m_simObjects.size(), m_shaderProg, _properties));
    m_simObjects.back()->LoadMesh(_file);
    m_simObjects.back()->AddToDynamicWorld(m_dynamicWorld);
    doneCurrent();
    update();
}

void OpenGLScene::ToggleSim()
{
    m_runSim = !m_runSim;
    if(m_runSim)
    {

    }
    else
    {

    }
    update();
}

void OpenGLScene::initializePhysicsWorld()
{
    //----------------------------------
    // Boilerplate bullet initialisation
    m_broadPhase = new btDbvtBroadphase();

    m_collisionConfig = new btDefaultCollisionConfiguration();
    m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfig);

    btGImpactCollisionAlgorithm::registerAlgorithm(m_collisionDispatcher);

    m_solver = new btSequentialImpulseConstraintSolver();

    // initialise the dynamic world
    m_dynamicWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadPhase, m_solver, m_collisionConfig);
    m_dynamicWorld->setGravity(btVector3(0.0f,-10.0f,0.0f));


    //-------------------------------
    // Setting up dynamic rigidbodies

    // collision shapes
    m_groundShape = new btStaticPlaneShape(btVector3(0.0f,1.0f,0.0f),1.0f);

    // ground rigid body setup
    m_groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
    btRigidBody::btRigidBodyConstructionInfo groundRBCI(0.0f, m_groundMotionState, m_groundShape, btVector3(0,0,0));
    m_groundRB = new btRigidBody(groundRBCI);
    m_dynamicWorld->addRigidBody(m_groundRB);
}

void OpenGLScene::initializeGroundPlane()
{
    m_groundColour = glm::vec3(0.7f, 0.7f, 0.7f);
    m_colourLoc = m_shaderProg->uniformLocation("colour");
    glUniform3fv(m_colourLoc, 1, &m_groundColour[0]);

    static glm::mat4 modelMat(1.0);
    GLuint modelMatricesLoc = m_shaderProg->attributeLocation("modelMatrix");

    static GLfloat const planeVertices[] = {
        100.0f, 0.0f, 100.0f,   // front right
        0.0f, 1.0f, 0.0f,
        100.0f, 0.0f, -100.0f,  // back right
        0.0f, 1.0f, 0.0f,
        -100.0f, 0.0f, -100.0f, // back left
        0.0f, 1.0f, 0.0f,
        -100.0f, 0.0f, 100.0f,  // front left
        0.0f, 1.0f, 0.0f,
        100.0f, 0.0f, 100.0f,   // fonrt right
        0.0f, 1.0f, 0.0f,
        -100.0f, 0.0f, -100.0f,  // back left
        0.0f, 1.0f, 0.0f
    };

    m_groundVAO.create();
    m_groundVAO.bind();

    // Setup our vertex buffer object.
    m_groundVBO.create();
    m_groundVBO.bind();
    m_groundVBO.allocate(planeVertices, 12 * 3 * sizeof(GLfloat));
    glEnableVertexAttribArray( 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_groundVBO.release();

    // set up instance model matrix buffer object
    m_groundMBO.create();
    m_groundMBO.bind();
    m_groundMBO.allocate(&modelMat, 1 * sizeof(glm::mat4));
    glEnableVertexAttribArray(modelMatricesLoc+0);
    glEnableVertexAttribArray(modelMatricesLoc+1);
    glEnableVertexAttribArray(modelMatricesLoc+2);
    glEnableVertexAttribArray(modelMatricesLoc+3);
    glVertexAttribPointer(modelMatricesLoc+0, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), 0);
    glVertexAttribPointer(modelMatricesLoc+1, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(1 * sizeof(glm::vec4)));
    glVertexAttribPointer(modelMatricesLoc+2, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(modelMatricesLoc+3, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::mat4), reinterpret_cast<void *>(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(modelMatricesLoc+0, 1);
    glVertexAttribDivisor(modelMatricesLoc+1, 1);
    glVertexAttribDivisor(modelMatricesLoc+2, 1);
    glVertexAttribDivisor(modelMatricesLoc+3, 1);
    m_groundMBO.release();

    m_groundVAO.release();
}

void OpenGLScene::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLScene::cleanup);
    glewInit();


    glClearColor(0.4, 0.4, 0.4, 1);

    // setup shaders
    m_shaderProg = new QOpenGLShaderProgram;
    m_shaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, "../shader/instanceVert.glsl");
    m_shaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, "../shader/instanceFrag.glsl");
    m_shaderProg->bindAttributeLocation("vertex", 0);
    m_shaderProg->bindAttributeLocation("normal", 1);
    m_shaderProg->link();

    m_shaderProg->bind();
    m_projMatrixLoc = m_shaderProg->uniformLocation("projMatrix");
    m_viewMatrixLoc = m_shaderProg->uniformLocation("viewMatrix");
    m_lightPosLoc = m_shaderProg->uniformLocation("lightPos");


    // initialise view and projection matrices
    m_viewMat = glm::mat4(1);
    m_viewMat = glm::lookAt(glm::vec3(0,0,0),glm::vec3(0,0,-1),glm::vec3(0,1,0));
    m_projMat = glm::perspective(45.0f, GLfloat(width()) / height(), 0.01f, 1000.0f);

    // Light position is fixed.
    m_lightPos = glm::vec3(0, 20, 70);
    glUniform3fv(m_lightPosLoc, 1, &m_lightPos[0]);

    initializeGroundPlane();

    m_shaderProg->release();

    m_physicsTimer->start((int)(m_dt * 1000));
}


//--------------------------------------------------------------------------------------------------
// Render methods

void OpenGLScene::drawGroundPlane()
{
    glUniform3fv(m_colourLoc, 1, &m_groundColour[0]);
    m_groundVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_groundVAO.release();
}


void OpenGLScene::paintGL()
{
    // clean gl window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // update model matrix
    m_modelMat = glm::mat4(1);
    m_modelMat = glm::translate(m_modelMat, glm::vec3(0,0, -0.1f*m_zDis));// m_zDis));
    m_modelMat = glm::rotate(m_modelMat, glm::radians(m_xRot/16.0f), glm::vec3(1,0,0));
    m_modelMat = glm::rotate(m_modelMat, glm::radians(m_yRot/16.0f), glm::vec3(0,1,0));


    // Set shader params
    m_shaderProg->bind();

    glUniformMatrix4fv(m_projMatrixLoc, 1, false, &m_projMat[0][0]);
    glUniformMatrix4fv(m_viewMatrixLoc, 1, false, &(m_viewMat*m_modelMat)[0][0]);


    //---------------------------------------------------------------------------------------
    // Draw code
    drawGroundPlane();

    for(auto&& simObj : m_simObjects)
    {
        simObj->Draw();
    }
    //---------------------------------------------------------------------------------------


    m_shaderProg->release();

}

void OpenGLScene::updateSimulation()
{
    if(m_runSim)
    {
        m_dynamicWorld->stepSimulation(m_dt, 10);
        update();
    }
}

//--------------------------------------------------------------------------------------------------
// Qt methods

void OpenGLScene::resizeGL(int w, int h)
{
    m_projMat = glm::perspective(45.0f, GLfloat(w) / h, 0.01f, 1000.0f);
}

void OpenGLScene::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void OpenGLScene::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setZTranslation(m_zDis + dy);
    }
    m_lastPos = event->pos();
}

void OpenGLScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_P)
    {
        m_runSim = !m_runSim;
    }

    update();

}
