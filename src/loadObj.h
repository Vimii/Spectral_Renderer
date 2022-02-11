//
// Created by Vimii on 2022/02/11.
//

#ifndef SPECTRAL_RAYTRACING_LOADOBJ_H
#define SPECTRAL_RAYTRACING_LOADOBJ_H

#ifndef TINEOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include "../tinyobjloader-master/tiny_obj_loader.h"
#include "triangle.h"
#include <algorithm>

void loadObj(
        std::string filepath,
        std::vector<Vector3f>& vertices,
        std::vector<Face>& faces,
        std::vector<Vector3f>& normals,
        std::vector<Vector2f>& texcoords
        )
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    tinyobj::ObjReader reader;

    if(!reader.ParseFromFile(filepath,reader_config))
    {
        if(reader.Error().empty())
            std::cerr << "TinyObjReader: " << reader.Error() << std::endl;
    }

    if(!reader.Warning().empty())
        std::cerr << "[WARNING]TinyObjReader: " << reader.Warning() << std::endl;

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    vertices.resize(attrib.vertices.size() / 3);
    normals.resize(attrib.normals.size() / 3);
    texcoords.resize(attrib.texcoords.size() / 2);

    for(size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            Face face{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
            for (size_t v = 0; v < 3; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                //setByIndex
                ((int*)&face.vertex_id)[v] = idx.vertex_index;
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                vertices[idx.vertex_index] = Vector3f(vx,vy,vz);

                //Normals if exists
                if(idx.normal_index >= 0)
                {
                    //setByIndex
                    ((int*)&face.normal_id)[v] = idx.normal_index;
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    normals[idx.normal_index] = Vector3f(nx,ny,nz);
                }

                //Texcoords if exists
                if(idx.texcoord_index >= 0) {
                    //setByIndex
                    ((int*)&face.texcoord_id)[v] = idx.texcoord_index;
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    texcoords[idx.texcoord_index] = Vector2f(tx, ty);
                }
            }
            faces.push_back(face);
            index_offset += 3;
        }

    }

}

#endif //SPECTRAL_RAYTRACING_LOADOBJ_H
