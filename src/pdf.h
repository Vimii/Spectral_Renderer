//
// Created by Vimii on 2021/07/20.
//

#ifndef RAYTRACING_PDF_H
#define RAYTRACING_PDF_H

class pdf {
public:
    virtual ~pdf(){}

    virtual double value(const vec3& direction) const = 0;
    virtual vec3 generate() const = 0;
};

class cosine_pdf : public pdf{
public:
    cosine_pdf(const vec3& w) {uvw.build_from_w(w);}

    virtual double value(const vec3& direction) const{
        auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine/pi;
    }

    virtual vec3 generate() const {
        return uvw.local(random_cosine_direction());
    }

public:
    onb uvw;
};

class hittable_pdf : public pdf {
public:
    hittable_pdf(shared_ptr<hittable> p, const point3& origin)
    : ptr(p), o(origin) {}

    virtual double value(const vec3& direction) const {
        return ptr->pdf_value(o, direction);
    }

    virtual vec3 generate() const{
        return ptr->random(o);
    }

public:
    shared_ptr<hittable> ptr;
    point3 o;
};

class mixture_pdf : public pdf {
public:
    mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    virtual double value(const vec3& direction) const {
        return 0.5 * p[0]->value(direction) + 0.5 *p[1]->value(direction);
    }

    virtual vec3 generate() const {
        if (random_double() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

public:
    shared_ptr<pdf> p[2];
};

#endif //RAYTRACING_PDF_H
