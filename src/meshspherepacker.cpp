#include "include/SpherePacking/meshspherepacker.h"


#ifdef foreach
#undef foreach
#endif

// OpenVDB includes
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/VolumeToSpheres.h>


void MeshSpherePacker::vdb::PackMeshWithSpheres(std::vector<glm::vec4> &spheres,
                                                const std::vector<glm::vec3> &meshVerts,
                                                const std::vector<glm::ivec3> &meshTris,
                                                const unsigned int &maxNumSpheres,
                                                const bool &overlap,
                                                const float &minSphereRad,
                                                const float &maxSphereRad)
{
    // Convert inputs into appropriate format for vdb
    std::vector<openvdb::Vec4f> vdbSpheres;
    std::vector<openvdb::Vec3I> vdbTris;
    for(auto tri : meshTris)
    {
        vdbTris.push_back(openvdb::Vec3I(tri.x, tri.y, tri.z));
    }


    // Convert mesh to vdb volume
    openvdb::math::Transform::Ptr xform = openvdb::math::Transform::createLinearTransform();
    openvdb::tools::QuadAndTriangleDataAdapter<glm::vec3, openvdb::Vec3I> vdbMesh(meshVerts, vdbTris);
    openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();
    grid = openvdb::tools::meshToVolume<openvdb::FloatGrid>(vdbMesh, *xform);

    // Fill volume with spheres
    openvdb::tools::fillWithSpheres<openvdb::FloatGrid>(*grid, vdbSpheres, maxNumSpheres, overlap, minSphereRad, maxSphereRad);


    // Convert back into default format
    for(auto sphere : vdbSpheres)
    {
        spheres.push_back(glm::vec4(sphere.x(), sphere.y(), sphere.z(), sphere.w()));
    }
}
