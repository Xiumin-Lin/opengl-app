#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>
#include "Mesh.h"

class Object {
public:
    std::vector<std::unique_ptr<Mesh>> m_meshes;

    std::vector<std::unique_ptr<Mesh>>& getMeshes() { return m_meshes; }
    vec3 getPosition() { return m_meshes[0]->getPosition(); }
    
    void loadMeshes(const std::string& object_filename, const std::string& mtl_basepath);
    void setupMeshes(uint32_t program);
};

#endif // OBJECT_H