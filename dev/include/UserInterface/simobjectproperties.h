#ifndef PHSYICSBODYPROPERTIES_H
#define PHSYICSBODYPROPERTIES_H


#include <glm/glm.hpp>



class OpenGLScene;
enum ConstraintTypes {Fixed, Generic6DOFSpring};

struct SimObjectProperties
{
public:

    // Physics
    struct PHYSBODY
    {
        unsigned int numSpheres;
        float minSphereRad;
        float maxSphereRad;
        bool overlapSpheres;
        bool selfCollisions;
        bool reload;

        float mass;
        float youngsModulus;
        float yieldStrength;

        float internalSpringStiffness;
        float internalSpringDamping;
        float internalSpringBreakingImpulseThreshold;
        glm::vec3 linearLowerLimit;
        glm::vec3 linearUpperLimit;
        glm::vec3 angularLowerLimit;
        glm::vec3 angularUpperLimit;

        ConstraintTypes constraintType;
    }PhysBody;


    // Visualsation
    struct RENDERMESH
    {
        glm::vec3 colour;
        bool drawMesh;
        bool drawSpheres;
        bool drawWireframe;
    }RenderMesh;

};

#endif // PHSYICSBODYPROPERTIES_H
