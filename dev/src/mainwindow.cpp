#include "include/UserInterface/mainwindow.h"
#include "include/UserInterface/simobjectpropertywidget.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGridLayout>
#include <boost/filesystem.hpp>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Initalise various widgets
    ui->setupUi(this);
    m_glScene = new OpenGLScene(this);
    m_propertiesGroupbox = new QGroupBox("Properties" ,this);
    m_propertiesTab = new QTabWidget(m_propertiesGroupbox);

    // Add OpenGLScene widget
    ui->gridLayout->addWidget(m_glScene, 0, 0, 2, 2);

    // Add Physicsbody Properties widget
    ui->gridLayout->addWidget(m_propertiesGroupbox, 0, 2, 1, 1);
    QGridLayout *propertiesLayout = new QGridLayout(m_propertiesGroupbox);
    propertiesLayout->addWidget(m_propertiesTab, 0, 0, 1, 1);
    m_propertiesGroupbox->setLayout(propertiesLayout);


    // Set up Signal and Slot connections
    connect(ui->s_LoadModel, SIGNAL(released()), this, SLOT(AddSimObject()));
    connect(ui->s_runSimBtn, SIGNAL(released()), m_glScene, SLOT(ToggleSim()));

}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_glScene;

}



void MainWindow::AddSimObject()
{
    QString file = QFileDialog::getOpenFileName(this,QString("Open File"), QString("./"), QString("3D files (*.abc *.obj)"));

    if (file.isNull())
    {
        return;
    }
    boost::filesystem::path f = file.toStdString();


    int id = m_propertiesTab->addTab(new SimObjectPropertiesWidget(m_propertiesTab), QString(f.stem().c_str()));
    SimObjectPropertiesWidget *pbWidget = dynamic_cast<SimObjectPropertiesWidget*>(m_propertiesTab->widget(id));
    m_glScene->loadSimObject(file.toStdString(), pbWidget->m_physicsProps);
    pbWidget->ConnectWithOpenGLScene(m_glScene);
    pbWidget->ConnectWithSimObject(m_glScene->GetPhysicsBody(m_glScene->NumSimObject()-1));

}
