#include "Utils.h"
#include <unordered_map>
#include <tuple>

using namespace std;

// Utiliser un tuple pour combiner les indices de vertex, normal et texture
struct IndexTuple
{
    int vertexIndex;
    int normalIndex;
    int texcoordIndex;

    bool operator==(const IndexTuple &other) const
    {
        return vertexIndex == other.vertexIndex && normalIndex == other.normalIndex && texcoordIndex == other.texcoordIndex;
    }
};

// Définir un hash personnalisé pour le tuple d'indices
struct IndexHash
{
    size_t operator()(const IndexTuple &it) const
    {
        return ((hash<int>()(it.vertexIndex) ^ (hash<int>()(it.normalIndex) << 1)) >> 1) ^ (hash<int>()(it.texcoordIndex) << 1);
    }
};

/**
 * Load an OBJ file and return a vector of Mesh
 * Mesh indices are filled to allow IBO
 */
vector<unique_ptr<Mesh>> Utils::load_obj(const char *filename)
{
    // cout << "Try loading OBJ " << filename << endl;
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename))
    {
        cerr << "LoadOBJ failed: " << err << endl;
        return {}; // return empty vector
    }

    // cout << "Total Vertices count : " << attrib.vertices.size() << endl;
    // cout << "Total Normals count : " << attrib.normals.size() << endl;
    // cout << "Total Texcoords count : " << attrib.texcoords.size() << endl;

    // cout << "\nTotal Shapes count : " << shapes.size() << endl;
    // cout << "Total Materials count : " << materials.size() << endl;

    vector<unique_ptr<Mesh>> meshes;
    meshes.reserve(shapes.size());
    unordered_map<IndexTuple, int, IndexHash> index_map;

    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        tinyobj::shape_t shape = shapes[s];
        cout << "\nShape " << shape.name << " (nb face: " << shape.mesh.num_face_vertices.size() << " & total indices: " << shape.mesh.indices.size() << ")" << endl;

        int vertexCount = 0;
        vector<Vertex> temp_vertices;
        vector<uint32_t> temp_indices;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            size_t nbVertices = size_t(shape.mesh.num_face_vertices[f]);
            // cout << "    Face " << f << " (nb vertice: " << nbVertices << ")" << endl;
            for (size_t v = 0; v < nbVertices; v++)
            {
                // cout << "\tIndices idx ------------------ " << index_offset + v << endl;
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                IndexTuple tuple{idx.vertex_index, idx.normal_index, idx.texcoord_index};

                if (index_map.find(tuple) != index_map.end())
                {
                    temp_indices.push_back(index_map[tuple]);
                    continue;
                }

                Vertex vertex;
                if (idx.vertex_index >= 0)
                {
                    tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                    // cout << "\tVertex idx [" << idx.vertex_index << "] : " << vx << " " << vy << " " << vz << endl;
                    vertex.position = vec3(vx, vy, vz);
                }
                else
                {
                    cerr << "Shape " << shape.name << " - " << "Face " << f << " - ";
                    cerr << "Vertex idx [-1] " << endl;
                    exit(1);
                }

                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    // cout << "\tNormal idx [" << idx.normal_index << "] : " << nx << " " << ny << " " << nz << endl;
                    vertex.normal = vec3(nx, ny, nz);
                }
                else
                {
                    // cerr << "Shape " << shape.name << " - " << "Face " << f << " - ";
                    // cerr << "Normal idx [-1]" << endl;
                }

                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    // cout << "\tTexcoords idx [" << idx.texcoord_index << "] : " << tx << " " << ty << endl;
                    vertex.texcoords = vec2(tx, ty);
                }
                else
                {
                    // cerr << "Shape " << shape.name << " - " << "Face " << f << " - ";
                    // cerr << "Texcoord idx [-1]" << endl;
                }
                temp_vertices.push_back(vertex);
                temp_indices.push_back(vertexCount);
                index_map[tuple] = vertexCount;
                vertexCount++;
                // cout << endl;
            }
            index_offset += nbVertices;
        }
        unique_ptr<Mesh> new_mesh_pt = make_unique<Mesh>();
        new_mesh_pt->allocateVertices(temp_vertices.size());
        new_mesh_pt->allocateIndices(temp_indices.size());
        memcpy(new_mesh_pt->vertices, temp_vertices.data(), temp_vertices.size() * sizeof(Vertex));
        memcpy(new_mesh_pt->indices, temp_indices.data(), temp_indices.size() * sizeof(uint32_t));

        // cout << "vertexCount : " << vertexCount << endl;
        // cout << "new_mesh_pt->vertexCount : " << new_mesh_pt->vertexCount << endl;
        // cout << "new_mesh_pt->indexCount : " << new_mesh_pt->indexCount << endl;
        // for (size_t i = 0; i < new_mesh_pt->indexCount; i++)
        // {
        //     Vertex v = new_mesh_pt->vertices[new_mesh_pt->indices[i]];
        //     cout << i << ": new_mesh_pt->vertices[" << new_mesh_pt->indices[i] << "] = x: " << v.position.x << " y: " << v.position.y << " z: " << v.position.z << " nx: " << v.normal.x << " ny: " << v.normal.y << " nz: " << v.normal.z << " tx: " << v.texcoords.x << " ty: " << v.texcoords.y << endl;
        // }

        meshes.push_back(std::move(new_mesh_pt));
    }
    // cout << "Success Loaded Shape count : " << meshes.size() << endl;
    return meshes;
}
