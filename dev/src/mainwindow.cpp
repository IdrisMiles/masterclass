#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_glScene = new OpenGLScene(this);
    ui->gridLayout->addWidget(m_glScene, 0, 0, 1, 1);

    //---------------------------------------------------------------------------------------------------------------------------------
    // Boilerplate bullet initialisation
    btBroadphaseInterface *broadPhase = new btDbvtBroadphase();

    btDefaultCollisionConfiguration *collisionConfig = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *collisionDispatcher = new btCollisionDispatcher(collisionConfig);

    btGImpactCollisionAlgorithm::registerAlgorithm(collisionDispatcher);

    btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

    // initialise the dynamic world
    btDiscreteDynamicsWorld *dynamicWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadPhase, solver,collisionConfig);
    dynamicWorld->setGravity(btVector3(0.0f,-10.0f,0.0f));



    //---------------------------------------------------------------------------------------------------------------------------------
    // Setting up dynamic rigidbodies

    // collision shapes
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0.0f,1.0f,0.0f),1.0f);
    btCollisionShape *sphereShape = new btSphereShape(1.0f);

    // ground rigid body setup
    btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
    btRigidBody::btRigidBodyConstructionInfo groundRBCI(0.0f, groundMotionState, groundShape, btVector3(0,0,0));
    btRigidBody *groundRB = new btRigidBody(groundRBCI);
    dynamicWorld->addRigidBody(groundRB);

    // sphere rigid body setup
    btDefaultMotionState *sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 50.0f, 0.0f)));
    btScalar mass = 1;
    btVector3 sphereInertia = btVector3(0,0,0);
    sphereShape->calculateLocalInertia(mass,sphereInertia);
    btRigidBody::btRigidBodyConstructionInfo sphereRBCI(mass, sphereMotionState, sphereShape, sphereInertia);
    btRigidBody *sphereRB = new btRigidBody(sphereRBCI);
    dynamicWorld->addRigidBody(sphereRB);



    //---------------------------------------------------------------------------------------------------------------------------------
    // OpenVDB setup
    openvdb::initialize();

    openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();

    std::cout<<"Testing random access:\n";

    openvdb::FloatGrid::Accessor accessor = grid->getAccessor();

    openvdb::Coord coord(10000, -20000000, 3000000);

    accessor.setValue(coord, 1.0);

    std::cout<<"Grid "<< coord <<" = "<< accessor.getValue(coord)<<std::endl;




    //---------------------------------------------------------------------------------------------------------------------------------
    // Do cool stuff here

    for(int i=0; i<300; i++)
    {
        dynamicWorld->stepSimulation(1/60.0f, 10);
        btTransform sphereTrans;
        sphereRB->getMotionState()->getWorldTransform(sphereTrans);

        std::cout<<"Sphere Y position:\t"<<sphereTrans.getOrigin().y()<<"\n";
    }


    //---------------------------------------------------------------------------------------------------------------------------------

    dynamicWorld->removeRigidBody(sphereRB);
    dynamicWorld->removeRigidBody(groundRB);


    // free memory
    delete sphereRB;
    delete sphereMotionState;
    delete groundRB;
    delete groundMotionState;
    delete groundShape;
    delete sphereShape;

    delete dynamicWorld;
    delete solver;
    delete collisionDispatcher;
    delete collisionConfig;
    delete broadPhase;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_glScene;
}

