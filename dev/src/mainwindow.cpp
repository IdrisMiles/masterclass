#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_glScene = new OpenGLScene(this);
    ui->gridLayout->addWidget(m_glScene, 0, 0, 1, 1);

    connect(ui->s_LoadAlembic, SIGNAL(released()), m_glScene, SLOT(loadMesh()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_glScene;
}

