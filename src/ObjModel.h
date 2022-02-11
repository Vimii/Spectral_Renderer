//
// Created by Vimii on 2022/02/11.
//

#ifndef SPECTRAL_RAYTRACING_OBJMODEL_H
#define SPECTRAL_RAYTRACING_OBJMODEL_H

#include "loadObj.h"

class ObjModel: public hittable {
public:
    ObjModel(){}
    ObjModel(
            std::string filepath,
            shared_ptr<material> mp
            );

    virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
        output_box = aabb(box_min, box_max);
        return true;
    }


protected:
    std::string m_filepath;
    std::vector<Vector3f> m_vertices;
    std::vector<Face> m_faces;
    std::vector<Vector3f> m_normals;
    std::vector<Vector2f> m_texcoords;
    Vector3f box_min;
    Vector3f box_max;
    hittable_list sides;
};

ObjModel::ObjModel(std::string filepath,
                   shared_ptr<material> mp)
                   :m_filepath(filepath){

    //オブジェクト読み込み
    loadObj(filepath,m_vertices,m_faces,m_normals,m_texcoords);

    //bbox範囲を設定
    box_min = box_max = m_vertices[0];
    for (auto v: m_vertices) {
        box_min = Vector3f(min(v[0],box_min[0]),min(v[1],box_min[1]),min(v[2],box_min[2]));
        box_max = Vector3f(max(v[0],box_max[0]),max(v[1],box_max[1]),max(v[2],box_max[2]));
    }

    //三角ポリゴンを生成
    for (auto f: m_faces) {
        std::vector<Vector3f> v;
        v.push_back(m_vertices[f.vertex_id[0]]);
        v.push_back(m_vertices[f.vertex_id[1]]);
        v.push_back(m_vertices[f.vertex_id[2]]);

        std::vector<Vector3f> n;
        n.push_back(m_normals[f.normal_id[0]]);
        n.push_back(m_normals[f.normal_id[1]]);
        n.push_back(m_normals[f.normal_id[2]]);

        std::vector<Vector2f> t;
        if(m_texcoords.size() > 0) {
            t.push_back(m_texcoords[f.texcoord_id[0]]);
            t.push_back(m_texcoords[f.texcoord_id[1]]);
            t.push_back(m_texcoords[f.texcoord_id[2]]);
        }else {
            //適当です。
            t.push_back(Vector2f(0.f, 0.f));
            t.push_back(Vector2f(1.f, 0.f));
            t.push_back(Vector2f(0.f, 1.f));
        }
        sides.add(make_shared<triangle>(v,f,n,t,mp));
    }

}

bool ObjModel::hit(const ray& r, double t0, double t1, hit_record& rec) const {
    return sides.hit(r,t0,t1,rec);
}

#endif //SPECTRAL_RAYTRACING_OBJMODEL_H
