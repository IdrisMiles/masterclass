#ifndef PHSYICSBODYPROPERTIES_H
#define PHSYICSBODYPROPERTIES_H

#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDoubleSpinBox>

#include <glm/glm.hpp>

#include <vector>
#include <memory>


class OpenGLScene;

struct PhysicsBodyProperties
{
public:

    glm::vec3 colour;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    bool drawMesh;
    bool drawSpheres;
    bool drawWireframe;

};


class PhysicsBodyPropertiesWidget : public QGroupBox
{

    Q_OBJECT

public:
    PhysicsBodyPropertiesWidget(QWidget *parent = 0);
    virtual ~PhysicsBodyPropertiesWidget();
    void ConnectWithOpenGLScene(OpenGLScene *_glScene);


    std::shared_ptr<PhysicsBodyProperties> m_physicsProps;
    QGridLayout *m_gridLayout;
    QCheckBox *m_drawMesh;
    QCheckBox *m_drawMeshWireframe;
    QCheckBox *m_drawSpheres;
    QDoubleSpinBox *m_colour[3];
    QDoubleSpinBox *m_mass;
    QDoubleSpinBox *m_youngsModulus;


public slots:
    void UpdatePhysicsBodyProperties();

signals:
    void PhysicsBodyPropertiesUpdated();


};






#endif // PHSYICSBODYPROPERTIES_H
