#ifndef GENERIC6DOFSPRINGCONSTRAINTWIDGET_H
#define GENERIC6DOFSPRINGCONSTRAINTWIDGET_H

#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QLabel>

class Generic6DOFSpringConstraintWidget : public QWidget
{
    Q_OBJECT

public:
    Generic6DOFSpringConstraintWidget(QWidget *parent = 0);
    virtual ~Generic6DOFSpringConstraintWidget();

    QGridLayout *m_layout;

    QLabel *m_internalSpringStiffnessLabel;
    QDoubleSpinBox *m_internalSpringStiffness;
    QLabel *m_internalSpringDampingLabel;
    QDoubleSpinBox *m_internalSpringDamping;
//    QLabel *m_glueStrengthLabel;
//    QDoubleSpinBox *m_glueStrength;

    QLabel *m_linearLowerLimitLabel;
    QDoubleSpinBox *m_linearLowerLimitXThreshold;
    QDoubleSpinBox *m_linearLowerLimitYThreshold;
    QDoubleSpinBox *m_linearLowerLimitZThreshold;

    QLabel *m_linearUpperLimitLabel;
    QDoubleSpinBox *m_linearUpperLimitXThreshold;
    QDoubleSpinBox *m_linearUpperLimitYThreshold;
    QDoubleSpinBox *m_linearUpperLimitZThreshold;

    QLabel *m_angularLowerLimitLabel;
    QDoubleSpinBox *m_angularLowerLimitXThreshold;
    QDoubleSpinBox *m_angularLowerLimitYThreshold;
    QDoubleSpinBox *m_angularLowerLimitZThreshold;

    QLabel *m_angularUpperLimitLabel;
    QDoubleSpinBox *m_angularUpperLimitXThreshold;
    QDoubleSpinBox *m_angularUpperLimitYThreshold;
    QDoubleSpinBox *m_angularUpperLimitZThreshold;
};

#endif // GENERIC6DOFSPRINGCONSTRAINTWIDGET_H
