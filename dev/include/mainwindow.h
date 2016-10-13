#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// OpenVDB includes
#include <openvdb/openvdb.h>

// Bullets includes
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

// Alembic includes
#include "Alembic/AbcGeom/All.h"
#include "Alembic/AbcCoreAbstract/All.h"
#include "Alembic/AbcCoreHDF5/All.h"
#include "Alembic/Abc/ErrorHandler.h"

// Qt includes
#include <QMainWindow>

// QtOpengl include
#include "openglscene.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    OpenGLScene *m_glScene;
};

#endif // MAINWINDOW_H
