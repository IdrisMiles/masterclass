#ifndef FIXEDCONSTRAINTWIDGET_H
#define FIXEDCONSTRAINTWIDGET_H

#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QLabel>

class FixedConstraintWidget : public QWidget
{
    Q_OBJECT

public:
    FixedConstraintWidget(QWidget *parent = 0);
    virtual ~FixedConstraintWidget();

    QGridLayout *m_layout;

//    QLabel *m_glueStrengthLabel;
//    QDoubleSpinBox *m_glueStrength;
};


#endif // FIXEDCONSTRAINTWIDGET_H
