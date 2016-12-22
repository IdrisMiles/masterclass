#ifndef PHSYICSBODYPROPERTIESWIDGET_H
#define PHSYICSBODYPROPERTIESWIDGET_H

#include "include/UserInterface/simobjectproperties.h"
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QComboBox>

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class OpenGLScene;
class SimObject;

class FixedConstraintWidget : public QWidget
{
    Q_OBJECT

public:
    FixedConstraintWidget(QWidget *parent = 0);
    virtual ~FixedConstraintWidget();

    QGridLayout *m_layout;

    QLabel *m_internalSpringBreakingImpulseThresholdLabel;
    QDoubleSpinBox *m_internalSpringBreakingImpulseThreshold;
};

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
    QLabel *m_internalSpringBreakingImpulseThresholdLabel;
    QDoubleSpinBox *m_internalSpringBreakingImpulseThreshold;

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


class SimObjectPropertiesWidget : public QGroupBox
{

    Q_OBJECT

public:
    SimObjectPropertiesWidget(QWidget *parent = 0);
    virtual ~SimObjectPropertiesWidget();
    void ConnectWithOpenGLScene(OpenGLScene *_glScene);
    void ConnectWithSimObject(std::shared_ptr<SimObject> _simObject);

    int m_id;
    std::shared_ptr<SimObject> m_simObject;
    std::shared_ptr<SimObjectProperties> m_physicsProps;
    QGridLayout *m_gridLayout;

    QGroupBox *m_physProps;
    QGridLayout *m_physPropsLayout;
    QLabel *m_numSpheresLabel;
    QDoubleSpinBox *m_numSpheres;
    QLabel *m_minSphereRadLabel;
    QDoubleSpinBox *m_minSphereRad;
    QLabel *m_maxSphereRadLabel;
    QDoubleSpinBox *m_maxSphereRad;
    QCheckBox *m_overlapSpheres;
    QLabel *m_massLabel;
    QDoubleSpinBox *m_mass;
    QLabel *m_youngsModulusLabel;
    QDoubleSpinBox *m_youngsModulus;
    QLabel *m_yieldStrengthLabel;
    QDoubleSpinBox *m_yieldStrength;
    QCheckBox *m_selfCollisions;
    FixedConstraintWidget *m_fixedConstraintWidget;
    Generic6DOFSpringConstraintWidget *m_generic6DOFSpringConstraintWidget;

    QPushButton *m_loadPhysBody;
    QComboBox *m_constraintSelection;

    QGroupBox *m_renderProps;
    QGridLayout *m_renderPropsLayout;
    QCheckBox *m_drawMesh;
    QCheckBox *m_drawMeshWireframe;
    QCheckBox *m_drawSpheres;
    QLabel *m_colourLabel;
    QDoubleSpinBox *m_colour[3];


public slots:
    //void UpdatePhysicsBodyProperties();
    void UpdateRenderingProperties();
    void UpdatePhysicsProperties();
    void OnLoadPushButton();

signals:
    void PhysicsBodyPropertiesUpdated();
    void RenderingPropertiesUpdated();
    void PhysicsPropertiesUpdated();


};






#endif // PHSYICSBODYPROPERTIESWIDGET_H
