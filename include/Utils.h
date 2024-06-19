#ifndef UTILS_H
#define UTILS_H

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "../vendor/tiny_obj_loader.h"

#include <iostream>
#include <vector>
#include "Mesh.h"

class Utils {
public:
    static std::vector<Mesh> load_obj(const char* filename);
};

#endif