#ifndef CACHEDPARTICLE_H
#define CACHEDPARTICLE_H

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
