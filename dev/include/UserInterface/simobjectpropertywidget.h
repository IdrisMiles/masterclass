#ifndef PHSYICSBODYPROPERTIESWIDGET_H
#define PHSYICSBODYPROPERTIESWIDGET_H

#include "include/UserInterface/simobjectproperties.h"
#include "include/UserInterface/fixedconstraintwidget.h"
#include "include/UserInterface/generic6dofspringconstraintwidget.h"
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QSlider>

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class OpenGLScene;
class SimObject;


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

    QLabel *m_yieldStrengthLabel;
    QDoubleSpinBox *m_yieldStrength;

    QCheckBox *m_selfCollisions;

    QLabel *m_glueStrengthLabel;
    QDoubleSpinBox *m_glueStrength;

    FixedConstraintWidget *m_fixedConstraintWidget;
    Generic6DOFSpringConstraintWidget *m_generic6DOFSpringConstraintWidget;

    QLabel *m_constraintRadiusLabel;
    QSlider *m_constraintRadius;

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
    void UpdateRenderingProperties();
    void UpdatePhysicsProperties();
    void OnLoadPushButton();

signals:
    void PhysicsBodyPropertiesUpdated();
    void RenderingPropertiesUpdated();
    void PhysicsPropertiesUpdated();


};






#endif // PHSYICSBODYPROPERTIESWIDGET_H
