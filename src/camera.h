//
// Created by Vimii on 2021/05/15.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

class camera {
public:
    camera() {}

    camera(
            point3 lookfrom,
            point3 lookat,
            vec3   vup,
            double vfov, // 垂直方向の視野角 (弧度法)
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double t0 = 0,
            double t1 = 0
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
        time0 = t0;
        time1 = t1;
    }

    ray get_ray(double s, double t, int wl) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(
                origin + offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset,
                random_double(time0, time1),
                wl
        );
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;  // シャッターの開閉時間
};

#endif //RAYTRACING_CAMERA_H
