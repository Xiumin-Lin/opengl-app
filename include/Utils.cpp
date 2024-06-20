#include "Utils.h"

std::vector<Mesh> Utils::load_obj(const char *filename)
{
    // std::cout << "Try loading OBJ " << filename << std::endl;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename))
    {
        std::cerr << "LoadOBJ failed: " << err << std::endl;
        return {}; // return empty vector
    }

    // std::cout << "Total Vertices count : " << attrib.vertices.size() << std::endl;
    // std::cout << "Total Normals count : " << attrib.normals.size() << std::endl;
    // std::cout << "Total Texcoords count : " << attrib.texcoords.size() << std::endl;

    // std::cout << "\nTotal Shapes count : " << shapes.size() << std::endl;
    // std::cout << "Total Materials count : " << materials.size() << std::endl;

    std::vector<Mesh> meshes;
    // Pour simplifier, nous supposons un seul shape par fichier OBJ
    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        Mesh new_mesh;
        tinyobj::shape_t shape = shapes[s];
        // std::cout << "\nShape " << shape.name << " (nb face: " << shape.mesh.num_face_vertices.size() << " & total indices: " << shape.mesh.indices.size() << ")" << std::endl;
        // Toutes les faces
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            size_t nbVertices = size_t(shape.mesh.num_face_vertices[f]);
            // std::cout << "    Face " << f << " (nb vertice: " << nbVertices << ")" << std::endl;
            // Pour chaque vertice de la face
            for (size_t v = 0; v < nbVertices; v++)
            {
                Vertex vertex;
                // std::cout << "\tIndices idx ------------------ " << index_offset + v << std::endl;
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                if (idx.vertex_index >= 0)
                {
                    tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                    // std::cout << "\tVertex idx [" << idx.vertex_index << "] : " << vx << " " << vy << " " << vz << std::endl;

                    vertex.position = vec3(vx, vy, vz);
                }
                else
                    // std::cout << "\tVertex idx [-1]" << std::endl;

                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    // std::cout << "\tNormal idx [" << idx.normal_index << "] : " << nx << " " << ny << " " << nz << std::endl;

                    vertex.normal = vec3(nx, ny, nz);
                }
                else
                    // std::cout << "\tNormal idx [-1]" << std::endl;

                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    // std::cout << "\tTexcoords idx [" << idx.texcoord_index << "] : " << tx << " " << ty << std::endl;

                    vertex.texcoords = vec2(tx, ty);
                }
                else
                    // std::cout << "\tTexcoord idx [-1]" << std::endl;

                new_mesh.vertices.push_back(vertex);
                new_mesh.vertexCount++;
                // std::cout << std::endl;
            }
            index_offset += nbVertices;
        }
        meshes.push_back(new_mesh);
    }
    // std::cout << "Success Loaded Shape count : " << meshes.size() << std::endl;
    return meshes;
}