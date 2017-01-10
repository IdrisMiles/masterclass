#include "include/UserInterface/fixedconstraintwidget.h"

FixedConstraintWidget::FixedConstraintWidget(QWidget *parent) : QWidget(parent)
{
//    m_glueStrengthLabel = new QLabel("Internal Spring Breaking Threshold", this);
//    m_glueStrength = new QDoubleSpinBox(this);
//    m_glueStrength->setSingleStep(1);
//    m_glueStrength->setMaximum(1000000);


    int row = 0;
    m_layout = new QGridLayout(this);

//    m_layout->addWidget(m_glueStrengthLabel,row,0,1,1);
//    m_layout->addWidget(m_glueStrength,row++,1,1,1);

    setLayout(m_layout);
}

FixedConstraintWidget::~FixedConstraintWidget()
{

}
