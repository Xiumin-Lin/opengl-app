#include "Utils.h"
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
vector<unique_ptr<Mesh>> Utils::load_obj(const std::string &filename, std::string mtl_basepath)
{
    cout << "==================================================" << endl;
    // CHARGEMENT DE L'OBJ ===================================================
    cout << "Try loading OBJ " << filename << endl;
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), mtl_basepath.c_str()))
    {
        cerr << "LoadOBJ failed: " << err << endl;
        return {}; // return empty vector
    }

    Utils::LogLoadObjInfo(attrib, shapes, materials);
    
    vector<unique_ptr<Mesh>> meshes;
    meshes.reserve(shapes.size());
    unordered_map<IndexTuple, int, IndexHash> index_map;

    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        tinyobj::shape_t shape = shapes[s];
        cout << "- Shape " << shape.name << " (nb face: " << shape.mesh.num_face_vertices.size() << " & total indices: " << shape.mesh.indices.size() << ")" << endl;
        int vertexCount = 0;
        vector<Vertex> temp_vertices;
        vector<uint32_t> temp_indices;

#pragma region RECUPERATION DES VERTICES ET INDICES
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
#pragma endregion

#pragma region CREATION DU MESH
        unique_ptr<Mesh> new_mesh_pt = make_unique<Mesh>();
        new_mesh_pt->name = shape.name;

        new_mesh_pt->allocateVertices(temp_vertices.size());
        new_mesh_pt->allocateIndices(temp_indices.size());
        memcpy(new_mesh_pt->vertices, temp_vertices.data(), temp_vertices.size() * sizeof(Vertex));
        memcpy(new_mesh_pt->indices, temp_indices.data(), temp_indices.size() * sizeof(uint32_t));
#pragma endregion

#pragma region RECUPERATION DES MATERIALS
        new_mesh_pt->createMaterial();
        if (shape.mesh.material_ids.size() > 0 && shape.mesh.material_ids[0] >= 0)
        {
            int material_id = shape.mesh.material_ids[0];
            tinyobj::material_t mat = materials[material_id];
            
            Material* material = new_mesh_pt->material;
            material->asset_path = mtl_basepath;
            material->texture_filename = (mat.ambient_texname.empty() ? mat.diffuse_texname : mat.ambient_texname);
            material->specular_texture_filename = mat.specular_texname;

            material->ambient = vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
            material->diffuse = vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
            material->specular = vec3(mat.specular[0], mat.specular[1], mat.specular[2]);
            material->shininess = mat.shininess;
        }
#pragma endregion
        // DEBUG ==============================================
        // Utils::LogMeshInfo(new_mesh_pt, shape.name); // Ne pas activer s'il y a beaucoup de vertices

        // SAVE MESH ==========================================
        meshes.push_back(std::move(new_mesh_pt));
    }
    cout << "Success Loaded file - Shape count : " << meshes.size() << endl << endl;
    cout << "==================================================" << endl;
    return meshes;
}

void Utils::LogLoadObjInfo(const tinyobj::attrib_t &attrib, const vector<tinyobj::shape_t> &shapes, const vector<tinyobj::material_t> &materials)
{
    cout << "Attrib Info:" << endl;
    cout << "  Total Vertices count : " << attrib.vertices.size() << endl;
    cout << "  Total Normals count : " << attrib.normals.size() << endl;
    cout << "  Total Texcoords count : " << attrib.texcoords.size() << endl;
    cout << endl;
    cout << "Total Materials count : " << materials.size() << endl;
    for (size_t i = 0; i < materials.size(); i++)
    {
        tinyobj::material_t mat = materials[i];
        std::cout << "Material " << i << " (" << mat.name << ")" << endl;
        // AMBIENT
        string ambient_texname = mat.ambient_texname.empty() ? "None" : mat.ambient_texname;
        std::cout << "  Ambient Texture: " << mat.ambient_texname << endl;
        std::cout << "  Ambient: " << mat.ambient[0] << ", " << mat.ambient[1] << ", " << mat.ambient[2] << endl;
        // DIFFUSE
        string diffuse_texname = mat.diffuse_texname.empty() ? "None" : mat.diffuse_texname;
        std::cout << "  Diffuse Texture: " << diffuse_texname << endl;
        std::cout << "  Diffuse: " << mat.diffuse[0] << ", " << mat.diffuse[1] << ", " << mat.diffuse[2] << endl;
        // SPECULAR
        string specular_texname = mat.specular_texname.empty() ? "None" : mat.specular_texname;
        std::cout << "  Specular Texture: " << mat.specular_texname << endl;
        std::cout << "  Specular: " << mat.specular[0] << ", " << mat.specular[1] << ", " << mat.specular[2] << endl;
        std::cout << "  Shininess: " << mat.shininess << endl;
    }
    cout << endl;
    cout << "Total Shapes count : " << shapes.size() << endl;
}

void Utils::LogMeshInfo(const unique_ptr<Mesh> &mesh, const string &name)
{
    cout << ".................................................." << endl;
    cout << "LOADED Mesh Info (" << name << "):" << endl;
    cout << "  Vertices count : " << mesh->vertexCount << endl;
    cout << "  Indices count : " << mesh->indexCount << endl;
    // VERTICES
    // for (size_t i = 0; i < mesh->indexCount; i++)
    // {
    //     Vertex v = mesh->vertices[mesh->indices[i]];
    //     cout << "  " << i << ": mesh->vertices[" << mesh->indices[i] << "] = x: " << v.position.x << " y: " << v.position.y << " z: " << v.position.z;
    //     cout << " nx: " << v.normal.x << " ny: " << v.normal.y << " nz: " << v.normal.z;
    //     cout << " tx: " << v.texcoords.x << " ty: " << v.texcoords.y << endl;
    // }
    // MATERIAL
    cout << endl;
    mesh->material->logInfo();
    cout << ".................................................." << endl;
}