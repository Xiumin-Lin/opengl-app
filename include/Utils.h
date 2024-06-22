#ifndef UTILS_H
#define UTILS_H

// #define TINYOBJLOADER_IMPLEMENTATION is in tiny_obj_loader.cc
#include "../vendor/tiny_obj_loader.h"

#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "Mesh.h"

class Utils {
public:
    static std::vector<std::unique_ptr<Mesh>> load_obj(const std::string &filename, std::string mtl_basepath = "./");
private:
    static void LogLoadObjInfo(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials);
    static void LogMeshInfo(const std::unique_ptr<Mesh> &mesh, const std::string &name);
};

#endif