#include "include/UserInterface/generic6dofspringconstraintwidget.h"

Generic6DOFSpringConstraintWidget::Generic6DOFSpringConstraintWidget(QWidget *parent) : QWidget(parent)
{
    m_internalSpringStiffnessLabel = new QLabel("Internal Spring Stiffness", this);
    m_internalSpringStiffness = new QDoubleSpinBox(this);
    m_internalSpringStiffness->setSingleStep(1);
    m_internalSpringStiffness->setMaximum(1000000);
    m_internalSpringDampingLabel = new QLabel("Internal Spring Damping", this);
    m_internalSpringDamping = new QDoubleSpinBox(this);
    m_internalSpringDamping->setSingleStep(1);
    m_internalSpringDamping->setMaximum(1000000);
//    m_glueStrengthLabel = new QLabel("Internal Spring Breaking Threshold", this);
//    m_glueStrength = new QDoubleSpinBox(this);
//    m_glueStrength->setSingleStep(10);
//    m_glueStrength->setMaximum(1000000);

    m_linearLowerLimitLabel = new QLabel("Linear Lower Limit", this);
    m_linearLowerLimitXThreshold = new QDoubleSpinBox(this);
    m_linearLowerLimitYThreshold = new QDoubleSpinBox(this);
    m_linearLowerLimitZThreshold = new QDoubleSpinBox(this);

    m_linearUpperLimitLabel = new QLabel("Linear Upper Limit", this);
    m_linearUpperLimitXThreshold = new QDoubleSpinBox(this);
    m_linearUpperLimitYThreshold = new QDoubleSpinBox(this);
    m_linearUpperLimitZThreshold = new QDoubleSpinBox(this);

    m_angularLowerLimitLabel = new QLabel("Angular Lower Limit", this);
    m_angularLowerLimitXThreshold = new QDoubleSpinBox(this);
    m_angularLowerLimitYThreshold = new QDoubleSpinBox(this);
    m_angularLowerLimitZThreshold = new QDoubleSpinBox(this);

    m_angularUpperLimitLabel = new QLabel("Angular Upper Limit", this);
    m_angularUpperLimitXThreshold = new QDoubleSpinBox(this);
    m_angularUpperLimitYThreshold = new QDoubleSpinBox(this);
    m_angularUpperLimitZThreshold = new QDoubleSpinBox(this);


    int row = 0;
    m_layout = new QGridLayout(this);

    m_layout->addWidget(m_internalSpringStiffnessLabel,row,0,1,1);
    m_layout->addWidget(m_internalSpringStiffness,row++,1,1,1);
    m_layout->addWidget(m_internalSpringDampingLabel,row,0,1,1);
    m_layout->addWidget(m_internalSpringDamping,row++,1,1,1);
//    m_layout->addWidget(m_glueStrengthLabel,row,0,1,1);
//    m_layout->addWidget(m_glueStrength,row++,1,1,1);

    m_layout->addWidget(m_linearUpperLimitLabel,row,0,1,1);
    m_layout->addWidget(m_linearUpperLimitXThreshold,row,1,1,1);
    m_layout->addWidget(m_linearUpperLimitYThreshold,row,2,1,1);
    m_layout->addWidget(m_linearUpperLimitZThreshold,row++,3,1,1);

    m_layout->addWidget(m_linearLowerLimitLabel,row,0,1,1);
    m_layout->addWidget(m_linearLowerLimitXThreshold,row,1,1,1);
    m_layout->addWidget(m_linearLowerLimitYThreshold,row,2,1,1);
    m_layout->addWidget(m_linearLowerLimitZThreshold,row++,3,1,1);

    m_layout->addWidget(m_angularUpperLimitLabel,row,0,1,1);
    m_layout->addWidget(m_angularUpperLimitXThreshold,row,1,1,1);
    m_layout->addWidget(m_angularUpperLimitYThreshold,row,2,1,1);
    m_layout->addWidget(m_angularUpperLimitZThreshold,row++,3,1,1);

    m_layout->addWidget(m_angularLowerLimitLabel,row,0,1,1);
    m_layout->addWidget(m_angularLowerLimitXThreshold,row,1,1,1);
    m_layout->addWidget(m_angularLowerLimitYThreshold,row,2,1,1);
    m_layout->addWidget(m_angularLowerLimitZThreshold,row++,3,1,1);

    setLayout(m_layout);
}

Generic6DOFSpringConstraintWidget::~Generic6DOFSpringConstraintWidget()
{

}
