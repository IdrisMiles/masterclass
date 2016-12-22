/********************************************************************************
** Form generated from reading UI file 'generic6DofSpringWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERIC6DOFSPRINGWIDGET_H
#define UI_GENERIC6DOFSPRINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Generic6DOFSpringWidget
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *linearLowerLimitX;
    QLabel *internalSpringStiffnessLabel;
    QLabel *internalSpringBreakingImpulseThresholdLabel;
    QLabel *angularUpperLimitLabel;
    QLabel *linearLowerLimitLabel;
    QLabel *angularLowerLimitLabel;
    QLabel *linearUpperLimitLabel;
    QDoubleSpinBox *linearUpperLimitX;
    QDoubleSpinBox *linearUpperLimitY;
    QLabel *internalSpringDampeningLabel;
    QDoubleSpinBox *linearUpperLimitZ;
    QDoubleSpinBox *angularLowerLimitX;
    QDoubleSpinBox *angularUpperLimitX;
    QDoubleSpinBox *angularUpperLimitY;
    QDoubleSpinBox *angularUpperLimitZ;
    QDoubleSpinBox *angularLowerLimitY;
    QDoubleSpinBox *angularLowerLimitZ;
    QDoubleSpinBox *linearLowerLimitY;
    QDoubleSpinBox *linearLowerLimitZ;
    QDoubleSpinBox *internalSpringBreakingImpulseThreshold;
    QDoubleSpinBox *internalSpringStiffness;
    QDoubleSpinBox *internalSpringDampening;

    void setupUi(QWidget *Generic6DOFSpringWidget)
    {
        if (Generic6DOFSpringWidget->objectName().isEmpty())
            Generic6DOFSpringWidget->setObjectName(QStringLiteral("Generic6DOFSpringWidget"));
        Generic6DOFSpringWidget->resize(797, 346);
        gridLayout = new QGridLayout(Generic6DOFSpringWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        linearLowerLimitX = new QDoubleSpinBox(Generic6DOFSpringWidget);
        linearLowerLimitX->setObjectName(QStringLiteral("linearLowerLimitX"));

        gridLayout->addWidget(linearLowerLimitX, 3, 2, 1, 1);

        internalSpringStiffnessLabel = new QLabel(Generic6DOFSpringWidget);
        internalSpringStiffnessLabel->setObjectName(QStringLiteral("internalSpringStiffnessLabel"));

        gridLayout->addWidget(internalSpringStiffnessLabel, 0, 0, 1, 1);

        internalSpringBreakingImpulseThresholdLabel = new QLabel(Generic6DOFSpringWidget);
        internalSpringBreakingImpulseThresholdLabel->setObjectName(QStringLiteral("internalSpringBreakingImpulseThresholdLabel"));

        gridLayout->addWidget(internalSpringBreakingImpulseThresholdLabel, 2, 0, 1, 1);

        angularUpperLimitLabel = new QLabel(Generic6DOFSpringWidget);
        angularUpperLimitLabel->setObjectName(QStringLiteral("angularUpperLimitLabel"));

        gridLayout->addWidget(angularUpperLimitLabel, 6, 0, 1, 1);

        linearLowerLimitLabel = new QLabel(Generic6DOFSpringWidget);
        linearLowerLimitLabel->setObjectName(QStringLiteral("linearLowerLimitLabel"));

        gridLayout->addWidget(linearLowerLimitLabel, 3, 0, 1, 1);

        angularLowerLimitLabel = new QLabel(Generic6DOFSpringWidget);
        angularLowerLimitLabel->setObjectName(QStringLiteral("angularLowerLimitLabel"));

        gridLayout->addWidget(angularLowerLimitLabel, 5, 0, 1, 1);

        linearUpperLimitLabel = new QLabel(Generic6DOFSpringWidget);
        linearUpperLimitLabel->setObjectName(QStringLiteral("linearUpperLimitLabel"));

        gridLayout->addWidget(linearUpperLimitLabel, 4, 0, 1, 1);

        linearUpperLimitX = new QDoubleSpinBox(Generic6DOFSpringWidget);
        linearUpperLimitX->setObjectName(QStringLiteral("linearUpperLimitX"));

        gridLayout->addWidget(linearUpperLimitX, 4, 2, 1, 1);

        linearUpperLimitY = new QDoubleSpinBox(Generic6DOFSpringWidget);
        linearUpperLimitY->setObjectName(QStringLiteral("linearUpperLimitY"));

        gridLayout->addWidget(linearUpperLimitY, 4, 3, 1, 1);

        internalSpringDampeningLabel = new QLabel(Generic6DOFSpringWidget);
        internalSpringDampeningLabel->setObjectName(QStringLiteral("internalSpringDampeningLabel"));

        gridLayout->addWidget(internalSpringDampeningLabel, 1, 0, 1, 1);

        linearUpperLimitZ = new QDoubleSpinBox(Generic6DOFSpringWidget);
        linearUpperLimitZ->setObjectName(QStringLiteral("linearUpperLimitZ"));

        gridLayout->addWidget(linearUpperLimitZ, 4, 4, 1, 1);

        angularLowerLimitX = new QDoubleSpinBox(Generic6DOFSpringWidget);
        angularLowerLimitX->setObjectName(QStringLiteral("angularLowerLimitX"));
        angularLowerLimitX->setMaximum(1e+6);
        angularLowerLimitX->setSingleStep(10);
        angularLowerLimitX->setValue(1e+6);

        gridLayout->addWidget(angularLowerLimitX, 5, 2, 1, 1);

        angularUpperLimitX = new QDoubleSpinBox(Generic6DOFSpringWidget);
        angularUpperLimitX->setObjectName(QStringLiteral("angularUpperLimitX"));

        gridLayout->addWidget(angularUpperLimitX, 6, 2, 1, 1);

        angularUpperLimitY = new QDoubleSpinBox(Generic6DOFSpringWidget);
        angularUpperLimitY->setObjectName(QStringLiteral("angularUpperLimitY"));

        gridLayout->addWidget(angularUpperLimitY, 6, 3, 1, 1);

        angularUpperLimitZ = new QDoubleSpinBox(Generic6DOFSpringWidget);
        angularUpperLimitZ->setObjectName(QStringLiteral("angularUpperLimitZ"));

        gridLayout->addWidget(angularUpperLimitZ, 6, 4, 1, 1);

        angularLowerLimitY = new QDoubleSpinBox(Generic6DOFSpringWidget);
        angularLowerLimitY->setObjectName(QStringLiteral("angularLowerLimitY"));

        gridLayout->addWidget(angularLowerLimitY, 5, 3, 1, 1);

        angularLowerLimitZ = new QDoubleSpinBox(Generic6DOFSpringWidget);
        angularLowerLimitZ->setObjectName(QStringLiteral("angularLowerLimitZ"));

        gridLayout->addWidget(angularLowerLimitZ, 5, 4, 1, 1);

        linearLowerLimitY = new QDoubleSpinBox(Generic6DOFSpringWidget);
        linearLowerLimitY->setObjectName(QStringLiteral("linearLowerLimitY"));

        gridLayout->addWidget(linearLowerLimitY, 3, 3, 1, 1);

        linearLowerLimitZ = new QDoubleSpinBox(Generic6DOFSpringWidget);
        linearLowerLimitZ->setObjectName(QStringLiteral("linearLowerLimitZ"));

        gridLayout->addWidget(linearLowerLimitZ, 3, 4, 1, 1);

        internalSpringBreakingImpulseThreshold = new QDoubleSpinBox(Generic6DOFSpringWidget);
        internalSpringBreakingImpulseThreshold->setObjectName(QStringLiteral("internalSpringBreakingImpulseThreshold"));
        internalSpringBreakingImpulseThreshold->setMaximum(1e+6);
        internalSpringBreakingImpulseThreshold->setSingleStep(10);
        internalSpringBreakingImpulseThreshold->setValue(1e+6);

        gridLayout->addWidget(internalSpringBreakingImpulseThreshold, 2, 2, 1, 3);

        internalSpringStiffness = new QDoubleSpinBox(Generic6DOFSpringWidget);
        internalSpringStiffness->setObjectName(QStringLiteral("internalSpringStiffness"));
        internalSpringStiffness->setValue(1);

        gridLayout->addWidget(internalSpringStiffness, 0, 2, 1, 3);

        internalSpringDampening = new QDoubleSpinBox(Generic6DOFSpringWidget);
        internalSpringDampening->setObjectName(QStringLiteral("internalSpringDampening"));
        internalSpringDampening->setValue(1);

        gridLayout->addWidget(internalSpringDampening, 1, 2, 1, 3);


        retranslateUi(Generic6DOFSpringWidget);

        QMetaObject::connectSlotsByName(Generic6DOFSpringWidget);
    } // setupUi

    void retranslateUi(QWidget *Generic6DOFSpringWidget)
    {
        Generic6DOFSpringWidget->setWindowTitle(QApplication::translate("Generic6DOFSpringWidget", "Form", 0));
        internalSpringStiffnessLabel->setText(QApplication::translate("Generic6DOFSpringWidget", "Spring Stiffness", 0));
        internalSpringBreakingImpulseThresholdLabel->setText(QApplication::translate("Generic6DOFSpringWidget", "Spring Breaking Impulse Threshold", 0));
        angularUpperLimitLabel->setText(QApplication::translate("Generic6DOFSpringWidget", "Angular Upper Limit", 0));
        linearLowerLimitLabel->setText(QApplication::translate("Generic6DOFSpringWidget", "Linear Lower Limit", 0));
        angularLowerLimitLabel->setText(QApplication::translate("Generic6DOFSpringWidget", "Angular Lower Limit", 0));
        linearUpperLimitLabel->setText(QApplication::translate("Generic6DOFSpringWidget", "Linear Upper Limit", 0));
        internalSpringDampeningLabel->setText(QApplication::translate("Generic6DOFSpringWidget", "Spring Dampening", 0));
    } // retranslateUi

};

namespace Ui {
    class Generic6DOFSpringWidget: public Ui_Generic6DOFSpringWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERIC6DOFSPRINGWIDGET_H
