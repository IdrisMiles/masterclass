#ifndef CACHEDPARTICLE_H
#define CACHEDPARTICLE_H

/// @author Idris Miles
/// @version 0.1.0
/// @date 10th January 2017


#include <string>
#include <vector>
#include <glm/glm.hpp>

class CachedParticle
{
public:
    CachedParticle();

    float rad;
    std::vector<glm::vec3> pos;
};

#endif // CACHEDPARTICLE_H
