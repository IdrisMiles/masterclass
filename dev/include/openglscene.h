#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H


#include "physicsbody.h"

// Open GL and Qt includes
#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

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

public slots:
    void loadMesh();
    void ToggleSim();
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setXTranslation(int x);
    void setYTranslation(int y);
    void setZTranslation(int z);
    void cleanup();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void xTranslationChanged(int x);
    void yTranslationChanged(int y);
    void zTranslationChanged(int z);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void initializeDemoTriangle();
    void renderDemoTriangle();
    void cleanDemoTriangle();

    void initializePhysicsWorld();
    void cleanPhysicsWorld();



    int m_xRot;
    int m_yRot;
    int m_zRot;
    int m_xDis;
    int m_yDis;
    int m_zDis;
    QPoint m_lastPos;
    glm::vec3 m_lightPos;

    QOpenGLShaderProgram *m_shaderProg;

    int m_projMatrixLoc;
    int m_viewMatrixLoc;
    int m_modelMatrixLoc;
    int m_lightPosLoc;

    glm::mat4 m_projMat;
    glm::mat4 m_viewMat;
    glm::mat4 m_modelMat;


    // Demo triangle specifics
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;

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
    std::vector<PhysicsBody*> m_physicsBodies;

    glm::vec3 m_colour;
    int m_colourLoc;

public:


};

#endif // OPENGLSCENE_H
