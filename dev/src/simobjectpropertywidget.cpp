#include "include/UserInterface/simobjectpropertywidget.h"

#include "include/Visualisation/openglscene.h"


SimObjectPropertiesWidget::SimObjectPropertiesWidget(QWidget *parent):QGroupBox(parent)
{

    //-----------------------------------------------------------------------------------------
    // Construct all the widgets

    m_renderProps = new QGroupBox("Render Properties", this);
    m_drawMesh = new QCheckBox("Draw Mesh", m_renderProps);
    m_drawMeshWireframe = new QCheckBox("Draw Mesh Wireframe", m_renderProps);
    m_drawSpheres = new QCheckBox("Draw Spheres", m_renderProps);
    m_colour[0] = new QDoubleSpinBox(m_renderProps);
    m_colour[0]->setRange(0.0,1.0);
    m_colour[0]->setSingleStep(0.05);
    m_colour[1] = new QDoubleSpinBox(m_renderProps);
    m_colour[1]->setRange(0.0,1.0);
    m_colour[1]->setSingleStep(0.05);
    m_colour[2] = new QDoubleSpinBox(m_renderProps);
    m_colour[2]->setRange(0.0,1.0);
    m_colour[2]->setSingleStep(0.05);
    m_mass = new QDoubleSpinBox(m_renderProps);
    m_youngsModulus = new QDoubleSpinBox(m_renderProps);

    m_physProps = new QGroupBox("Physics Properties", this);
    m_numSpheresLabel = new QLabel("Num Spheres", m_physProps);
    m_numSpheres = new QDoubleSpinBox(m_physProps);
    m_numSpheres->setSingleStep(1);
    m_numSpheres->setMaximum(10000);
    m_minSphereRadLabel = new QLabel("Min Sphere Rad", m_physProps);
    m_minSphereRad = new QDoubleSpinBox(m_physProps);
    m_minSphereRad->setSingleStep(0.1);
    m_maxSphereRadLabel = new QLabel("Max Sphere Rad", m_physProps);
    m_maxSphereRad = new QDoubleSpinBox(m_physProps);
    m_maxSphereRad->setSingleStep(0.1);
    m_overlapSpheres = new QCheckBox("Overlap Spheres", m_physProps);
    m_loadPhysBody = new QPushButton("Load", m_physProps);

    //-----------------------------------------------------------------------------------------
    // Add widgets to layouts

    int row = 0;
    m_renderPropsLayout = new QGridLayout(m_renderProps);
    m_renderPropsLayout->addWidget(m_drawMesh,row++,0,1,1);
    m_renderPropsLayout->addWidget(m_drawMeshWireframe,row++,0,1,1);
    m_renderPropsLayout->addWidget(m_drawSpheres,row++,0,1,1);
    m_renderPropsLayout->addWidget(m_colour[0],row,0,1,1);
    m_renderPropsLayout->addWidget(m_colour[1],row,1,1,1);
    m_renderPropsLayout->addWidget(m_colour[2],row++,2,1,1);
    m_renderPropsLayout->addWidget(m_mass,row++,0,1,1);
    m_renderPropsLayout->addWidget(m_youngsModulus,row++,0,1,1);
    m_renderProps->setLayout(m_renderPropsLayout);


    row = 0;
    m_physPropsLayout = new QGridLayout(m_physProps);
    m_physPropsLayout->addWidget(m_numSpheresLabel, row, 0, 1, 1);
    m_physPropsLayout->addWidget(m_numSpheres, row++, 1, 1, 1);
    m_physPropsLayout->addWidget(m_minSphereRadLabel, row, 0, 1, 1);
    m_physPropsLayout->addWidget(m_minSphereRad, row++, 1, 1, 1);
    m_physPropsLayout->addWidget(m_maxSphereRadLabel, row, 0, 1, 1);
    m_physPropsLayout->addWidget(m_maxSphereRad, row++, 1, 1, 1);
    m_physPropsLayout->addWidget(m_overlapSpheres, row++, 0, 1, 1);
    m_physPropsLayout->addWidget(m_loadPhysBody, row++, 0, 1, 1);
    m_physProps->setLayout(m_physPropsLayout);


    row = 0;
    // Add widgets to layout
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->addWidget(m_physProps, row++, 0, 1, 1);
    m_gridLayout->addWidget(m_renderProps, row++, 0, 1, 1);
    setLayout(m_gridLayout);

    //-----------------------------------------------------------------------------------------

    // Initialise render properties
    m_physicsProps = std::shared_ptr<SimObjectProperties>(new SimObjectProperties());
    m_physicsProps->RenderMesh.colour = glm::vec3(0.8f,0.4f,0.4f);
    m_physicsProps->RenderMesh.position = glm::vec3(0,0,0);
    m_physicsProps->RenderMesh.rotation = glm::vec3(0,0,0);
    m_physicsProps->RenderMesh.scale = glm::vec3(0,0,0);
    m_physicsProps->RenderMesh.drawMesh = true;
    m_physicsProps->RenderMesh.drawSpheres = true;

    // Initialise physics properties
    m_physicsProps->PhysBody.numSpheres = 1000;
    m_physicsProps->PhysBody.overlapSpheres = true;
    m_physicsProps->PhysBody.minSphereRad = 1.0f;
    m_physicsProps->PhysBody.maxSphereRad = 5.0f;

    // initalise widgets to property values
    m_drawMesh->setChecked(m_physicsProps->RenderMesh.drawMesh);
    m_drawSpheres->setChecked(m_physicsProps->RenderMesh.drawSpheres);
    m_drawMeshWireframe->setChecked(m_physicsProps->RenderMesh.drawWireframe);
    m_numSpheres->setValue((double)m_physicsProps->PhysBody.numSpheres);
    m_minSphereRad->setValue(m_physicsProps->PhysBody.minSphereRad);
    m_maxSphereRad->setValue(m_physicsProps->PhysBody.maxSphereRad);
    m_overlapSpheres->setChecked(m_physicsProps->PhysBody.overlapSpheres);

    //-----------------------------------------------------------------------------------------
    // Connect widget singal and slots

    // connect widget signals and slots
    connect(m_drawMesh, SIGNAL(clicked(bool)), this, SLOT(UpdateRenderingProperties()));
    connect(m_drawMeshWireframe, SIGNAL(clicked(bool)), this, SLOT(UpdateRenderingProperties()));
    connect(m_drawSpheres, SIGNAL(clicked(bool)), this, SLOT(UpdateRenderingProperties()));
    connect(m_colour[0], SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));
    connect(m_colour[1], SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));
    connect(m_colour[2], SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));
    connect(m_mass, SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));
    connect(m_youngsModulus, SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));

    connect(m_numSpheres, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_minSphereRad, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_maxSphereRad, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_overlapSpheres, SIGNAL(clicked(bool)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_loadPhysBody, SIGNAL(clicked(bool)), this, SLOT(OnLoadPushButton()));

    // Update
    UpdatePhysicsBodyProperties();

}


SimObjectPropertiesWidget::~SimObjectPropertiesWidget()
{
    delete m_gridLayout;
    delete m_drawMesh;
    delete m_drawMeshWireframe;
    delete m_drawSpheres;
    delete m_colour[0];
    delete m_colour[1];
    delete m_colour[2];
    delete m_mass;
    delete m_youngsModulus;
}



void SimObjectPropertiesWidget::ConnectWithOpenGLScene(OpenGLScene *_glScene)
{
    connect(this, SIGNAL(PhysicsBodyPropertiesUpdated()), _glScene, SLOT(update()));

    connect(this, SIGNAL(RenderingPropertiesUpdated()), _glScene, SLOT(update()));
    //connect(this, SIGNAL(PhysicsPropertiesUpdated()), _glScene, SLOT(update()));
}

void SimObjectPropertiesWidget::ConnectWithSimObject(std::shared_ptr<SimObject> _simObject)
{
    m_simObject = _simObject;
    //connect(this, SIGNAL(PhysicsPropertiesUpdated()), _glScene, SLOT(update()));
}

void SimObjectPropertiesWidget::UpdatePhysicsBodyProperties()
{
    m_physicsProps->RenderMesh.drawMesh = m_drawMesh->isChecked();
    m_physicsProps->RenderMesh.drawWireframe = m_drawMeshWireframe->isChecked();
    m_physicsProps->RenderMesh.drawSpheres = m_drawSpheres->isChecked();
    m_physicsProps->RenderMesh.colour = glm::vec3(m_colour[0]->value(), m_colour[1]->value(), m_colour[2]->value());
    m_drawMeshWireframe->setEnabled(m_drawMesh->isChecked());

    m_physicsProps->PhysBody.numSpheres = (unsigned int)m_numSpheres->value();
    m_physicsProps->PhysBody.minSphereRad = m_minSphereRad->value();
    m_physicsProps->PhysBody.maxSphereRad = m_maxSphereRad->value();
    m_physicsProps->PhysBody.overlapSpheres = m_overlapSpheres->isChecked();

    m_physicsProps->PhysBody.reload = false;

    emit PhysicsBodyPropertiesUpdated();
    emit RenderingPropertiesUpdated();
    emit PhysicsPropertiesUpdated();
}

void SimObjectPropertiesWidget::UpdateRenderingProperties()
{
    m_physicsProps->RenderMesh.drawMesh = m_drawMesh->isChecked();
    m_physicsProps->RenderMesh.drawWireframe = m_drawMeshWireframe->isChecked();
    m_physicsProps->RenderMesh.drawSpheres = m_drawSpheres->isChecked();
    m_physicsProps->RenderMesh.colour = glm::vec3(m_colour[0]->value(), m_colour[1]->value(), m_colour[2]->value());
    m_drawMeshWireframe->setEnabled(m_drawMesh->isChecked());

    emit RenderingPropertiesUpdated();

    m_simObject->UpdateRenderProps();
}

void SimObjectPropertiesWidget::UpdatePhysicsProperties()
{
    m_physicsProps->PhysBody.numSpheres = (unsigned int)m_numSpheres->value();
    m_physicsProps->PhysBody.minSphereRad = m_minSphereRad->value();
    m_physicsProps->PhysBody.maxSphereRad = m_maxSphereRad->value();
    m_physicsProps->PhysBody.overlapSpheres = m_overlapSpheres->isChecked();

    m_physicsProps->PhysBody.reload = false;

    emit PhysicsPropertiesUpdated();

    m_simObject->UpdatePhysicsProps();
}

void SimObjectPropertiesWidget::OnLoadPushButton()
{
    m_physicsProps->PhysBody.reload = true;

    emit PhysicsBodyPropertiesUpdated();
    emit RenderingPropertiesUpdated();
    emit PhysicsPropertiesUpdated();
}
