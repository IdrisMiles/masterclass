#include "include/Utils/meshloader.h"
#include <iostream>
#include <boost/filesystem.hpp>


MeshLoader::MeshLoader()
{

}


Mesh MeshLoader::LoadMesh(const std::string _meshFile)
{
    Mesh mesh;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(_meshFile,
                                             aiProcess_GenSmoothNormals |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);
    if(!scene)
    {
        std::cout<<"Error loading "<<_meshFile<<" with assimp\n";
    }
    else
    {
        if(scene->HasMeshes())
        {
            unsigned int indexOffset = 0;
            for(unsigned int i=0; i<scene->mNumMeshes; i++)
            {
                unsigned int numFaces = scene->mMeshes[i]->mNumFaces;
                for(unsigned int f=0; f<numFaces; f++)
                {
                    auto face = scene->mMeshes[i]->mFaces[f];
                    mesh.tris.push_back(glm::ivec3(face.mIndices[0]+indexOffset, face.mIndices[1]+indexOffset, face.mIndices[2]+indexOffset));
                }
                indexOffset += 3 * numFaces;

                unsigned int numVerts = scene->mMeshes[i]->mNumVertices;
                for(unsigned int v=0; v<numVerts; v++)
                {
                    auto vert = scene->mMeshes[i]->mVertices[v];
                    auto norm = scene->mMeshes[i]->mNormals[v];
                    mesh.verts.push_back(glm::vec3(vert.x, vert.y, vert.z));
                    mesh.norms.push_back(glm::vec3(norm.x, norm.y, norm.z));
                }
            }
        }
    }

    return mesh;
}
