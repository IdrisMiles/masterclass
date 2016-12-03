#ifndef PHSYICSBODYPROPERTIES_H
#define PHSYICSBODYPROPERTIES_H


#include <glm/glm.hpp>



class OpenGLScene;

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
        bool reload;
    }PhysBody;


    // Visualsation
    struct RENDERMESH
    {
        glm::vec3 colour;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        bool drawMesh;
        bool drawSpheres;
        bool drawWireframe;
    }RenderMesh;

};

#endif // PHSYICSBODYPROPERTIES_H
