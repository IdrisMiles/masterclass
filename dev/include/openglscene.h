#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H


#include <GL/glew.h>

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>



// Alembic includes
#include "Alembic/AbcGeom/All.h"
#include "Alembic/AbcCoreAbstract/All.h"
#include "Alembic/AbcCoreHDF5/All.h"
#include "Alembic/Abc/ErrorHandler.h"



using namespace Alembic::Abc;
using namespace Alembic::AbcGeom;

class OpenGLScene : public QOpenGLWidget
{

    Q_OBJECT

public:
    OpenGLScene(QWidget *parent = 0);
    ~OpenGLScene();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void LoadAlembic();
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

private:
    void initializeDemoTriangle();
    void renderDemoTriangle();
    void cleanDemoTriangle();
    void initializeAlembicModelVAO();
    void RecursiveTraverseGetPolyMesh(const IObject &_object, int _tab, int _depth, IPolyMesh &_outputMesh);
    void renderAlembicModel();
    void cleanAlembicModel();

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
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;

    glm::mat4 m_projMat;
    glm::mat4 m_viewMat;
    glm::mat4 m_modelMat;


    // Demo triangle specifics
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;

    glm::vec3 m_colour;
    int m_colourLoc;

public:
    std::vector<QOpenGLVertexArrayObject*> m_vaos;
    std::vector<QOpenGLBuffer*> m_vbos;
    std::vector<QOpenGLBuffer*> m_ibos;
    std::vector<glm::vec3> m_meshVerts;
    std::vector<unsigned int> m_meshElementIndex;


};

#endif // OPENGLSCENE_H
