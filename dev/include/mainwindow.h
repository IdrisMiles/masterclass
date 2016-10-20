#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/VolumeToSpheres.h>


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
