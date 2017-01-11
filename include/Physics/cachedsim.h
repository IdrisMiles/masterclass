#ifndef CACHEDSIM_H
#define CACHEDSIM_H


/// @author Idris Miles
/// @version 0.1.0
/// @date 10th January 2017


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
