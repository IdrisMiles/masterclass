#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QtOpengl include
#include "include/Visualisation/openglscene.h"

// Qt includes
#include <QMainWindow>
#include <QTabWidget>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void AddSimObject();


private:
    Ui::MainWindow *ui;
    OpenGLScene *m_glScene;
    QGroupBox *m_propertiesGroupbox;
    QTabWidget * m_propertiesTab;
    QLabel *m_simStepsLabel;
    QSlider *m_simStepsSlider;
};

#endif // MAINWINDOW_H
