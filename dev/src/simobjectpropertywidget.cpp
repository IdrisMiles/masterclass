#include "include/UserInterface/simobjectpropertywidget.h"

#include "include/Visualisation/openglscene.h"
#include <iostream>


SimObjectPropertiesWidget::SimObjectPropertiesWidget(QWidget *parent):QGroupBox(parent)
{

    //-----------------------------------------------------------------------------------------
    // Construct all the widgets

    m_renderProps = new QGroupBox("Render Properties", this);
    m_drawMesh = new QCheckBox("Draw Mesh", m_renderProps);
    m_drawMeshWireframe = new QCheckBox("Draw Mesh Wireframe", m_renderProps);
    m_drawSpheres = new QCheckBox("Draw Spheres", m_renderProps);
    m_colourLabel = new QLabel("Colour", m_renderProps);
    m_colour[0] = new QDoubleSpinBox(m_renderProps);
    m_colour[0]->setRange(0.0,1.0);
    m_colour[0]->setSingleStep(0.05);
    m_colour[1] = new QDoubleSpinBox(m_renderProps);
    m_colour[1]->setRange(0.0,1.0);
    m_colour[1]->setSingleStep(0.05);
    m_colour[2] = new QDoubleSpinBox(m_renderProps);
    m_colour[2]->setRange(0.0,1.0);
    m_colour[2]->setSingleStep(0.05);


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
    m_massLabel = new QLabel("Mass", m_physProps);
    m_mass = new QDoubleSpinBox(m_physProps);
    m_yieldStrengthLabel = new QLabel("Yield Strength", m_physProps);
    m_yieldStrength = new QDoubleSpinBox(m_physProps);
    m_selfCollisions = new QCheckBox("Self Collision", m_physProps);
    m_glueStrengthLabel = new QLabel("Glue Strength", m_physProps);
    m_glueStrength = new QDoubleSpinBox(m_physProps);
    m_constraintRadiusLabel = new QLabel("Constraint Radius", m_physProps);
    m_constraintRadius = new QSlider(Qt::Horizontal, m_physProps);
    m_loadPhysBody = new QPushButton("Load", m_physProps);

    m_fixedConstraintWidget = new FixedConstraintWidget(m_physProps);
    m_generic6DOFSpringConstraintWidget = new Generic6DOFSpringConstraintWidget(m_physProps);

    m_constraintSelection = new QComboBox(m_physProps);
    m_constraintSelection->addItem("Fixed");
    m_constraintSelection->addItem("Generic6DOFSpring");



    //-----------------------------------------------------------------------------------------
    // Add widgets to layouts

    int row = 0;
    m_renderPropsLayout = new QGridLayout(m_renderProps);
    m_renderPropsLayout->addWidget(m_drawMesh,row++,0,1,1);
    m_renderPropsLayout->addWidget(m_drawMeshWireframe,row++,0,1,1);
    m_renderPropsLayout->addWidget(m_drawSpheres,row++,0,1,1);
    m_renderPropsLayout->addWidget(m_colourLabel, row, 0, 1, 1);
    m_renderPropsLayout->addWidget(m_colour[0],row,1,1,1);
    m_renderPropsLayout->addWidget(m_colour[1],row,2,1,1);
    m_renderPropsLayout->addWidget(m_colour[2],row++,3,1,1);
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
    m_physPropsLayout->addWidget(m_massLabel,row,0,1,1);
    m_physPropsLayout->addWidget(m_mass,row++,1,1,1);
    m_physPropsLayout->addWidget(m_yieldStrengthLabel,row,0,1,1);
    m_physPropsLayout->addWidget(m_yieldStrength,row++,1,1,1);
    m_physPropsLayout->addWidget(m_glueStrengthLabel,row,0,1,1);
    m_physPropsLayout->addWidget(m_glueStrength,row++,1,1,1);
    m_physPropsLayout->addWidget(m_selfCollisions, row++, 0, 1, 1);
    m_physPropsLayout->addWidget(m_constraintRadiusLabel, row, 0, 1, 1);
    m_physPropsLayout->addWidget(m_constraintRadius, row++, 1, 1, 1);
    m_physPropsLayout->addWidget(m_loadPhysBody, row++, 0, 1, 1);
    m_physPropsLayout->addWidget(m_constraintSelection, row++, 0, 1, 1);
    m_physPropsLayout->addWidget(m_fixedConstraintWidget, row++, 0, 1, 2);
    m_physPropsLayout->addWidget(m_generic6DOFSpringConstraintWidget, row++, 0, 1, 2);
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
    m_physicsProps->RenderMesh.colour = glm::vec3(0.6f,0.6f,0.6f);
    m_physicsProps->RenderMesh.drawMesh = true;
    m_physicsProps->RenderMesh.drawSpheres = true;

    // Initialise physics properties
    m_physicsProps->PhysBody.numSpheres = 100;
    m_physicsProps->PhysBody.overlapSpheres = true;
    m_physicsProps->PhysBody.minSphereRad = 1.0f;
    m_physicsProps->PhysBody.maxSphereRad = 5.0f;
    m_physicsProps->PhysBody.mass = 1.0f;
    m_physicsProps->PhysBody.constraintType = ConstraintTypes::Fixed;
    m_physicsProps->PhysBody.internalSpringStiffness = 0.0f;
    m_physicsProps->PhysBody.internalSpringDamping = 1.0f;
    m_physicsProps->PhysBody.GlueStrength = 10.0f;
    m_physicsProps->PhysBody.constraintRadius = 1.0f;
    m_physicsProps->PhysBody.selfCollisions = false;
    m_physicsProps->PhysBody.yieldStrength = 1.0f;

    // initalise widgets to property values
    m_drawMesh->setChecked(m_physicsProps->RenderMesh.drawMesh);
    m_drawSpheres->setChecked(m_physicsProps->RenderMesh.drawSpheres);
    m_drawMeshWireframe->setChecked(m_physicsProps->RenderMesh.drawWireframe);
    m_numSpheres->setValue((double)m_physicsProps->PhysBody.numSpheres);
    m_minSphereRad->setValue(m_physicsProps->PhysBody.minSphereRad);
    m_maxSphereRad->setValue(m_physicsProps->PhysBody.maxSphereRad);
    m_overlapSpheres->setChecked(m_physicsProps->PhysBody.overlapSpheres);
    m_selfCollisions->setChecked(m_physicsProps->PhysBody.selfCollisions);
    m_mass->setValue(m_physicsProps->PhysBody.mass);
    m_yieldStrength->setMaximum(1000);
    m_yieldStrength->setValue(m_physicsProps->PhysBody.yieldStrength);
    m_constraintRadius->setMinimum(100);
    m_constraintRadius->setMaximum(300);
    m_constraintRadius->setSliderPosition(100);
    m_glueStrength->setMaximum(1000000);
    m_glueStrength->setValue(m_physicsProps->PhysBody.GlueStrength);
    m_colour[0]->setValue(m_physicsProps->RenderMesh.colour.x);
    m_colour[1]->setValue(m_physicsProps->RenderMesh.colour.y);
    m_colour[2]->setValue(m_physicsProps->RenderMesh.colour.z);
    m_generic6DOFSpringConstraintWidget->m_internalSpringDamping->setValue(m_physicsProps->PhysBody.internalSpringDamping);
    m_generic6DOFSpringConstraintWidget->m_internalSpringStiffness->setValue(m_physicsProps->PhysBody.internalSpringStiffness);

    m_fixedConstraintWidget->hide();
    m_generic6DOFSpringConstraintWidget->hide();
    if(m_constraintSelection->currentText().toStdString() == "Fixed")
    {
        m_physicsProps->PhysBody.constraintType = ConstraintTypes::Fixed;
        m_fixedConstraintWidget->show();
    }
    else if(m_constraintSelection->currentText().toStdString() == "Generic6DOFSpring")
    {
        m_physicsProps->PhysBody.constraintType = ConstraintTypes::Generic6DOFSpring;
        m_generic6DOFSpringConstraintWidget->show();
    }
    else
    {
        m_physicsProps->PhysBody.constraintType = ConstraintTypes::Fixed;
        m_fixedConstraintWidget->show();
    }

    //-----------------------------------------------------------------------------------------
    // Connect widget singal and slots

    // connect widget signals and slots
    connect(m_drawMesh, SIGNAL(clicked(bool)), this, SLOT(UpdateRenderingProperties()));
    connect(m_drawMeshWireframe, SIGNAL(clicked(bool)), this, SLOT(UpdateRenderingProperties()));
    connect(m_drawSpheres, SIGNAL(clicked(bool)), this, SLOT(UpdateRenderingProperties()));
    connect(m_colour[0], SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));
    connect(m_colour[1], SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));
    connect(m_colour[2], SIGNAL(valueChanged(double)), this, SLOT(UpdateRenderingProperties()));

    connect(m_numSpheres, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_minSphereRad, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_maxSphereRad, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_overlapSpheres, SIGNAL(clicked(bool)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_mass, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_yieldStrength, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_glueStrength, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_constraintRadius, SIGNAL(sliderMoved(int)), this, SLOT(UpdatePhysicsProperties()));


    connect(m_generic6DOFSpringConstraintWidget->m_internalSpringStiffness, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_internalSpringDamping, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_linearLowerLimitXThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_linearLowerLimitYThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_linearLowerLimitZThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_linearUpperLimitXThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_linearUpperLimitYThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_linearUpperLimitZThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_angularLowerLimitXThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_angularLowerLimitYThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_angularLowerLimitZThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_angularUpperLimitXThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_angularUpperLimitYThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_generic6DOFSpringConstraintWidget->m_angularUpperLimitZThreshold, SIGNAL(valueChanged(double)), this, SLOT(UpdatePhysicsProperties()));


    connect(m_constraintSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_selfCollisions, SIGNAL(clicked(bool)), this, SLOT(UpdatePhysicsProperties()));
    connect(m_loadPhysBody, SIGNAL(clicked(bool)), this, SLOT(OnLoadPushButton()));

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
}



void SimObjectPropertiesWidget::ConnectWithOpenGLScene(OpenGLScene *_glScene)
{
    connect(this, SIGNAL(RenderingPropertiesUpdated()), _glScene, SLOT(update()));
}

void SimObjectPropertiesWidget::ConnectWithSimObject(std::shared_ptr<SimObject> _simObject)
{
    m_simObject = _simObject;
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
    m_physicsProps->PhysBody.selfCollisions = m_selfCollisions->isChecked();
    m_physicsProps->PhysBody.yieldStrength = m_yieldStrength->value();
    m_physicsProps->PhysBody.constraintRadius = (float)m_constraintRadius->value()/100.0f;


    m_fixedConstraintWidget->hide();
    m_generic6DOFSpringConstraintWidget->hide();
    if(m_constraintSelection->currentText().toStdString() == "Fixed")
    {
        m_physicsProps->PhysBody.constraintType = ConstraintTypes::Fixed;
        m_fixedConstraintWidget->show();
    }
    else if(m_constraintSelection->currentText().toStdString() == "Generic6DOFSpring")
    {
        m_physicsProps->PhysBody.constraintType = ConstraintTypes::Generic6DOFSpring;
        m_physicsProps->PhysBody.linearUpperLimit.x = m_generic6DOFSpringConstraintWidget->m_linearUpperLimitXThreshold->value();
        m_physicsProps->PhysBody.linearUpperLimit.y = m_generic6DOFSpringConstraintWidget->m_linearUpperLimitYThreshold->value();
        m_physicsProps->PhysBody.linearUpperLimit.z = m_generic6DOFSpringConstraintWidget->m_linearUpperLimitZThreshold->value();

        m_physicsProps->PhysBody.linearLowerLimit.x = -m_generic6DOFSpringConstraintWidget->m_linearLowerLimitXThreshold->value();
        m_physicsProps->PhysBody.linearLowerLimit.y = -m_generic6DOFSpringConstraintWidget->m_linearLowerLimitYThreshold->value();
        m_physicsProps->PhysBody.linearLowerLimit.z = -m_generic6DOFSpringConstraintWidget->m_linearLowerLimitZThreshold->value();

        m_physicsProps->PhysBody.angularUpperLimit.x = m_generic6DOFSpringConstraintWidget->m_angularUpperLimitXThreshold->value();
        m_physicsProps->PhysBody.angularUpperLimit.y = m_generic6DOFSpringConstraintWidget->m_angularUpperLimitYThreshold->value();
        m_physicsProps->PhysBody.angularUpperLimit.z = m_generic6DOFSpringConstraintWidget->m_angularUpperLimitZThreshold->value();

        m_physicsProps->PhysBody.angularLowerLimit.x = -m_generic6DOFSpringConstraintWidget->m_angularLowerLimitXThreshold->value();
        m_physicsProps->PhysBody.angularLowerLimit.y = -m_generic6DOFSpringConstraintWidget->m_angularLowerLimitYThreshold->value();
        m_physicsProps->PhysBody.angularLowerLimit.z = -m_generic6DOFSpringConstraintWidget->m_angularLowerLimitZThreshold->value();

        m_physicsProps->PhysBody.internalSpringDamping = m_generic6DOFSpringConstraintWidget->m_internalSpringDamping->value();
        m_physicsProps->PhysBody.internalSpringStiffness = m_generic6DOFSpringConstraintWidget->m_internalSpringStiffness->value();

        m_generic6DOFSpringConstraintWidget->show();
    }
    else
    {
        m_physicsProps->PhysBody.constraintType = ConstraintTypes::Fixed;
        m_fixedConstraintWidget->show();
    }


    m_physicsProps->PhysBody.reload = false;


    emit PhysicsPropertiesUpdated();
}

void SimObjectPropertiesWidget::OnLoadPushButton()
{
    m_physicsProps->PhysBody.reload = true;

    emit RenderingPropertiesUpdated();
    emit PhysicsPropertiesUpdated();

    m_simObject->UpdatePhysicsProps();
}
