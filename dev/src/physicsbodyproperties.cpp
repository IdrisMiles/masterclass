#include "physicsbodyproperties.h"

#include "physicsbody.h"
#include "openglscene.h"


PhysicsBodyPropertiesWidget::PhysicsBodyPropertiesWidget(QWidget *parent):QGroupBox(parent)
{

    // Construct all the widgets
    m_drawMesh = new QCheckBox("Draw Mesh", this);
    m_drawSpheres = new QCheckBox("Draw Spheres", this);
    m_colour[0] = new QDoubleSpinBox(this);
    m_colour[0]->setRange(0.0,1.0);
    m_colour[0]->setSingleStep(0.05);
    m_colour[1] = new QDoubleSpinBox(this);
    m_colour[1]->setRange(0.0,1.0);
    m_colour[1]->setSingleStep(0.05);
    m_colour[2] = new QDoubleSpinBox(this);
    m_colour[2]->setRange(0.0,1.0);
    m_colour[2]->setSingleStep(0.05);
    m_mass = new QDoubleSpinBox(this);
    m_youngsModulus = new QDoubleSpinBox(this);

    // Add widgets to layout
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->addWidget(m_drawMesh,0,0,1,1);
    m_gridLayout->addWidget(m_drawSpheres,1,0,1,1);
    m_gridLayout->addWidget(m_colour[0],2,0,1,1);
    m_gridLayout->addWidget(m_colour[1],2,1,1,1);
    m_gridLayout->addWidget(m_colour[2],2,2,1,1);
    m_gridLayout->addWidget(m_mass,3,0,1,1);
    m_gridLayout->addWidget(m_youngsModulus,4,0,1,1);

    // Initialise physics properties
    m_physicsProps = new PhysicsBodyProperties();
    m_physicsProps->colour = glm::vec3(0.8f,0.4f,0.4f);
    m_physicsProps->position = glm::vec3(0,0,0);
    m_physicsProps->rotation = glm::vec3(0,0,0);
    m_physicsProps->scale = glm::vec3(0,0,0);
    m_physicsProps->drawMesh = true;
    m_physicsProps->drawSpheres = true;

    // initalise widgets to property values
    m_drawMesh->setChecked(m_physicsProps->drawMesh);
    m_drawSpheres->setChecked(m_physicsProps->drawSpheres);

    // connect widget signals and slots
    connect(m_drawMesh, SIGNAL(clicked(bool)), this, SLOT(UpdatePhysicsBodyProperties()));
    connect(m_drawSpheres, SIGNAL(clicked(bool)), this, SLOT(UpdatePhysicsBodyProperties()));
    connect(m_colour[0], SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsBodyProperties()));
    connect(m_colour[1], SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsBodyProperties()));
    connect(m_colour[2], SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsBodyProperties()));
    connect(m_mass, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsBodyProperties()));
    connect(m_youngsModulus, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsBodyProperties()));

    // Update
    UpdatePhysicsBodyProperties();
}


PhysicsBodyPropertiesWidget::~PhysicsBodyPropertiesWidget()
{
    delete m_drawMesh;
    delete m_gridLayout;
}



void PhysicsBodyPropertiesWidget::ConnectWithOpenGLScene(OpenGLScene *_glScene)
{
    connect(this, SIGNAL(PhysicsBodyPropertiesUpdated()), _glScene, SLOT(update()));
}


void PhysicsBodyPropertiesWidget::UpdatePhysicsBodyProperties()
{
    m_physicsProps->drawMesh = m_drawMesh->isChecked();
    m_physicsProps->drawSpheres = m_drawSpheres->isChecked();
    m_physicsProps->colour = glm::vec3(m_colour[0]->value(), m_colour[1]->value(), m_colour[2]->value());
    emit PhysicsBodyPropertiesUpdated();

}
