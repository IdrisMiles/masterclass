#ifndef MESHSPHEREPACKER_H
#define MESHSPHEREPACKER_H

#include <vector>
#include <glm/glm.hpp>


namespace MeshSpherePacker
{
    namespace vdb
    {
        void PackMeshWithSpheres(   std::vector<glm::vec4> &spheres,
                                    const std::vector<glm::vec3> &meshVerts,
                                    const std::vector<glm::ivec3> &meshTris,
                                    const unsigned int &maxNumSpheres,
                                    const bool &overlap,
                                    const float &minSphereRad,
                                    const float &maxSphereRad   );

    }
}

#endif // MESHSPHEREPACKER_H
