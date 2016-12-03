#ifndef CACHEDSIM_H
#define CACHEDSIM_H

#include "include/Physics/cachedparticle.h"

class CachedSimObject
{
public:
    CachedSimObject();

    void WriteToFile();
    void ReadFromFile(std::string &_file);

    std::vector<CachedParticle> points;
};

#endif // CACHEDSIM_H
