#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H


#include "include/simobject.h"

// Open GL and Qt includes
#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QTimer>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Bullets includes
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>


class OpenGLScene : public QOpenGLWidget
{

    Q_OBJECT

public:
    OpenGLScene(QWidget *parent = 0);
    ~OpenGLScene();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void loadSimObject(const std::string &_file, std::shared_ptr<SimObjectProperties> _properties = nullptr);
    std::shared_ptr<SimObject> GetPhysicsBody(const unsigned &_i = 0) {return  (_i<m_simObjects.size())?m_simObjects[_i]:NULL;}
    unsigned int NumPhysicsBodies()const;
    unsigned int NumSimObject()const {return m_simObjects.size();}

    static glm::mat4 getProjMat(){return m_projMat;}
    static glm::mat4 getViewMat(){return m_viewMat;}
    static glm::mat4 getModelMat(){return m_modelMat;}
    static glm::vec3 getLightPos(){return m_lightPos;}

public slots:
    void ToggleSim();
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setXTranslation(int x);
    void setYTranslation(int y);
    void setZTranslation(int z);
    void cleanup();
    void updateSimulation();
    void SetSimulationSteps(int _simSteps);


protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void initializePhysicsWorld();
    void cleanPhysicsWorld();
    void initializeGroundPlane();
    void drawGroundPlane();
    void CacheSim();

    int m_xRot;
    int m_yRot;
    int m_zRot;
    int m_xDis;
    int m_yDis;
    int m_zDis;
    QPoint m_lastPos;

    // Scene shader stuff
    int m_projMatrixLoc;
    int m_viewMatrixLoc;
    int m_modelMatrixLoc;
    int m_lightPosLoc;

    static glm::mat4 m_projMat;
    static glm::mat4 m_viewMat;
    static glm::mat4 m_modelMat;
    static glm::vec3 m_lightPos;
    QOpenGLShaderProgram *m_shaderProg;

    // Ground plane stuff
    QOpenGLVertexArrayObject m_groundVAO;
    QOpenGLBuffer m_groundVBO;
    QOpenGLBuffer m_groundMBO;
    glm::vec3 m_groundColour;
    GLuint m_colourLoc;

    // Bullet physics stuff
    bool m_runSim;
    btBroadphaseInterface *m_broadPhase;
    btDefaultCollisionConfiguration *m_collisionConfig;
    btCollisionDispatcher *m_collisionDispatcher;
    btSequentialImpulseConstraintSolver *m_solver;
    btDiscreteDynamicsWorld *m_dynamicWorld;
    btCollisionShape *m_groundShape;
    btDefaultMotionState *m_groundMotionState;
    btRigidBody *m_groundRB;
    std::vector<std::shared_ptr<SimObject>> m_simObjects;

    bool m_glInit;
    QTimer *m_physicsTimer;
    float m_dt;
    int m_simSubSteps;

    bool m_cache;


public:


};

#endif // OPENGLSCENE_H
