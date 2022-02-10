//
// Created by Vimii on 2022/02/10.
//

#ifndef SPECTRAL_RAYTRACING_TRIANGLE_H
#define SPECTRAL_RAYTRACING_TRIANGLE_H

#include "../Eigen/Dense"

using namespace Eigen;

struct Face{
    Vector3i vertex_id;
    Vector3i normal_id;
    Vector3i texcoord_id;
};

class triangle: public hittable{
public:
    triangle(){};

    triangle(const std::vector<Vector3f>& verticies,
             shared_ptr<material> mat
    ): m_verticies(verticies) ,
       m_mp(mat){};

    triangle(const std::vector<Vector3f>& verticies,
             const std::vector<Face>& faces,
             const std::vector<Vector3f>& normals,
             const std::vector<Vector2f>& texcoords,
             shared_ptr<material> mat
            ): m_verticies(verticies) ,
               m_faces(faces),
               m_normals(normals),
               m_texcoords(texcoords),
               m_mp(mat){};

    virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
        // AABB の辺の長さはゼロであってはならないので、
        // y 方向に少しだけ厚みを持たせる
        output_box = aabb(point3(minVertex(m_verticies)), point3(maxVertex(m_verticies)));
        return true;
    }

    virtual Vector3f maxVertex(const std::vector<Vector3f>& verticies) const {
        return Vector3f(
                max(verticies[0].x(),max(verticies[1].x(),verticies[2].x())),
                max(verticies[0].y(),max(verticies[1].y(),verticies[2].y())),
                max(verticies[0].z(),max(verticies[1].z(),verticies[2].z()))
                );
    }

    virtual Vector3f minVertex(const std::vector<Vector3f>& verticies) const {
        return Vector3f(
                min(verticies[0].x(),min(verticies[1].x(),verticies[2].x())),
                min(verticies[0].y(),min(verticies[1].y(),verticies[2].y())),
                min(verticies[0].z(),min(verticies[1].z(),verticies[2].z()))
        );
    }

protected:
    std::vector<Vector3f> m_verticies;
    std::vector<Face> m_faces;
    std::vector<Vector3f> m_normals;
    std::vector<Vector2f> m_texcoords;

    shared_ptr<material> m_mp;
};

bool triangle::hit(const ray &r, double t0, double t1, hit_record &rec) const {
    /*参考:https://github.com/mitsuba-renderer/mitsuba/blob/450a2b8a258f09ec7e0824861e2306340ccbb3f4/include/mitsuba/core/triangle.h#L109*/
    /*2辺を設定*/
    Vector3f edge1 = m_verticies[1] - m_verticies[0], edge2 = m_verticies[2] - m_verticies[0];

    /*行列式を計算*/
    Vector3f pvec = r.dir.V3f().cross(edge2);

    Float det = edge1.dot(pvec);
    if(det == 0)
        return false;
    Float inv_det = 1.0f / det;

    /*頂点[0]からr.originまでの距離*/
    Vector3f tvec = r.origin().V3f() - m_verticies[0];

    /*Uパラメータを計算しバウンドテスト*/
    float u = tvec.dot(pvec) * inv_det;
    if(u<0.0 || u > 1.0)
        return false;

    /*Vパラメータ*/
    Vector3f qvec = tvec.cross(edge1);
    float v = r.dir.V3f().dot(qvec) * inv_det;

    /*判定*/
    if(v >= 0.0 && u + v <= 1.0){
        double t = dot(edge2, qvec) * inv_det;
        if (t < t0 || t > t1)
            return false;
        rec.u = u;
        rec.v = v;
        rec.t = t;
        vec3 outward_normal = unit_vector(vec3(edge1.cross(edge2)));
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = m_mp;
        rec.p = r.at(t);
        return true;
    }
    return false;
}

#endif //SPECTRAL_RAYTRACING_TRIANGLE_H
