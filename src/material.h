//
// Created by Vimii on 2021/05/15.
//

#ifndef SPECTRAL_RAYTRACING_MATERIAL_H
#define SPECTRAL_RAYTRACING_MATERIAL_H

#include "texture.h"
#include "onb.h"
#include "pdf.h"
#include "spectrum.h"

enum material_type{
    diffuse_l,
    lamb,
    mtl,
    diel,
    isot
};

struct scatter_record{
    ray specular_ray;
    bool is_specular;
    spectrum attenuation;
    shared_ptr<pdf> pdf_ptr;
    material_type mat_type;
};



class material {
public:
    virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            scatter_record& srec
    ) const {
        return false;
    }

    virtual double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
    ) const {
        return 0;
    }

    virtual spectrum emitted(
            const ray &r_in,
            const hit_record& rec,
            double u,
            double v,
            const point3& p
    )const {
        return spectrum();
    }
};

class diffuse_light : public material  {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}

    virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            spectrum& attenuation,
            ray& scattered
    ) const {
        return false;
    }

    virtual spectrum emitted(
            const ray &r_in,
            const hit_record& rec,
            double u,
            double v,
            const point3& p
    ) const {
        if (rec.front_face)
            return emit->value(u, v, p);
        else
            return spectrum();
    }

public:
    shared_ptr<texture> emit;
};

class lambertian : public material {
public:
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            scatter_record& srec
    ) const {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u,rec.v,rec.p);
        srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
        srec.mat_type = lamb;
        return true;
    }

    double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
            ) const {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine/pi;
    }

    shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const spectrum& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.specular_ray = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = 0;
        srec.mat_type = mtl;
        return true;
    }

public:
    spectrum albedo;
    double fuzz;
};

double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

class dielectric : public material {
public:
    dielectric(double ri, double di) : ref_idx(ri), dis_idx(di) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const {
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        srec.attenuation = const_spectrum(1.0f);
        srec.mat_type = diel;

//        double ref = ref_idx + (double)(r_in.wavelength() - MTS_WAVELENGTH_MIN)
//                   / (double)(MTS_WAVELENGTH_MAX - MTS_WAVELENGTH_MIN) * (-dis_idx);

        double ref = J_SFH1_tbl[getIndex(r_in.wavelength())];

        double etai_over_etat = (rec.front_face) ? (1.0 / ref) : (ref);

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        if (etai_over_etat * sin_theta > 1.0 ) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            srec.specular_ray = ray(rec.p, reflected, r_in.time());
            return true;
        }

        double reflect_prob = schlick(cos_theta, etai_over_etat);
        if (random_double() < reflect_prob) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            srec.specular_ray = ray(rec.p, reflected, r_in.time());
            return true;
        }

        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        srec.specular_ray = ray(rec.p, refracted, r_in.time());
        return true;
    }

    double ref_BK7(double wl) const{
        return sqrt(2.251356
                    - 7.9494605 * pow(10,-3) * pow(wl,2)
                    + 1.1616810 * pow(10,-2) * pow(wl,-2)
                    + 1.5150006 * pow(10,-4) * pow(wl,-4)
                    - 3.1524929 * pow(10,-6) * pow(wl,-6)
                    + 4.8412686 * pow(10,-7) * pow(wl,-8));
    }

    static constexpr Float BK7_tbl[SAMPLE_NUM] = {
            1.5327261344431,1.53174188103097,1.53080277066309,1.52990585440618,1.52904843709247,1.52822804990742,1.52744242651672,1.52668948220893,1.52596729561628,1.5252740926464,1.52460823231646,1.52396819422893,1.52335256746855,1.522760040733,1.5221893935379,1.52163948835973,1.52110926359988,1.52059772726973,1.52010395131002,1.5196270664701,1.5191662576824,1.51872075987572,1.51828985417872,1.51787286447083,1.5174691542432,1.51707812373703,1.51669920733043,1.51633187114851,1.51597561087425,1.51562994974043,1.51529443668493,1.51496864465387,1.51465216903876,1.51434462623508,1.51404565231139,1.513754901779,1.51347204645327,1.51319677439859,1.51292878894989,1.5126678078041,1.5124135621758,1.5121657960118,1.51192426525968,1.51168873718625,1.51145898974166,1.51123481096591,1.51101599843416,1.51080235873816,1.51059370700092,1.51038986642219,1.51019066785249,1.50999594939359,1.50980555602356,1.50961933924463,1.50943715675225,1.5092588721239,1.50908435452627,1.50891347843952,1.50874612339757,1.50858217374321,1.50842151839722,1.50826405064041,1.50810966790784,1.50795827159445,1.50780976687129,1.5076640625118,1.50752107072739,1.5073807070118,1.50724288999375,1.50710754129732,1.50697458540952,1.5068439495548,1.50671556357596,1.50658935982109,1.50646527303622,1.50634324026339,1.50622320074368,1.50610509582515,1.50598886887517,1.50587446519706,1.50576183195072,1.50565091807711,1.50554167422623,1.50543405268859,1.50532800732979,1.50522349352823,1.50512046811563,1.50501888932032,1.50491871671305,1.50481991115535,1.50472243475008,1.50462625079432,1.50453132373422,1.50443761912194,1.50434510357438
    };

    static constexpr Float F2_tbl[SAMPLE_NUM] = {
            1.66881008467389,1.66631469060282,1.66395930656657,1.66173298230549,1.65962584170828,1.65762895882348,1.65573425003332,1.65393438007335,1.65222267993991,1.65059307502682,1.64904002208311,1.64755845379375,1.64614372996204,1.64479159442099,1.64349813692633,1.64225975939045,1.64107314590589,1.63993523608396,1.63884320129872,1.63779442348194,1.63678647616235,1.63581710748249,1.63488422496162,1.63398588180272,1.6331202645675,1.63228568206524,1.63148055532064,1.63070340850227,1.62995286070757,1.62922761851289,1.62852646920776,1.62784827464214,1.62719196562358,1.6265565368084,1.62594104203737,1.62534459007184,1.62476634069113,1.62420550111626,1.62366132272893,1.62313309805785,1.62262015800747,1.62212186930697,1.6216376321593,1.62116687807236,1.62070906785621,1.62026368977158,1.61983025781678,1.61940831014088,1.61899740757274,1.61859713225586,1.61820708638064,1.61782689100573,1.61745618496155,1.61709462382911,1.61674187898841,1.61639763673085,1.61606159743066,1.61573347477091,1.61541299501984,1.61509989635393,1.61479392822394,1.61449485076108,1.61420243422015,1.61391645845709,1.61363671243849,1.61336299378078,1.61309510831698,1.61283286968925,1.61257609896525,1.61232462427695,1.6120782804801,1.61183690883329,1.61160035669502,1.61136847723781,1.61114112917815,1.61091817652126,1.61069948831985,1.61048493844579,1.61027440537404,1.61006777197812,1.60986492533617,1.60966575654726,1.60947016055713,1.60927803599286,1.60908928500595,1.60890381312328,1.60872152910558,1.60854234481281,1.60836617507628,1.6081929375769,1.60802255272934,1.60785494357185,1.60769003566117,1.60752775697248,1.6073680378041
    };

    static constexpr Float J_SFH1_tbl[SAMPLE_NUM] = {
            1.99299688321653,1.97847907616544,1.96534817142415,1.95344638985265,1.94263540287461,1.93279385582475,1.92381520337884,1.91560582058028,1.90808335634927,1.90117529966671,1.89481773179165,1.88895424084613,1.88353497785331,1.87851583582853,1.87385773579388,1.86952600562192,1.86548983942366,1.86172182679527,1.85819754264491,1.85489518955353,1.85179528569855,1.84888039230618,1.8461348754106,1.84354469740338,1.84109723446648,1.83878111650952,1.83658608668766,1.83450287797023,1.83252310456922,1.83063916633023,1.82884416444118,1.82713182703212,1.82549644342826,1.82393280598055,1.82243615853897,1.82100215075509,1.81962679750552,1.81830644281876,1.81703772776674,1.81581756185069,1.81464309746999,1.81351170711423,1.81242096296329,1.81136861861906,1.81035259272639,1.80937095426996,1.80842190935997,1.80750378934137,1.80661504008148,1.80575421230757,1.80491995288121,1.80411099690928,1.80332616060285,1.80256433480575,1.80182447912287,1.80110561658668,1.80040682880694,1.79972725155474,1.79906607073749,1.79842251872606,1.79779587099943,1.79718544307611,1.79659058770461,1.7960106922883,1.79544517652257,1.79489349022441,1.7943551113366,1.79382954409057,1.79331631731347,1.79281498286662,1.79232511420351,1.79184630503703,1.79137816810624,1.79092033403424,1.79047245026936,1.79003418010254,1.78960520175471,1.78918520752823,1.78877390301727,1.78837100637233,1.78797624761452,1.78758936799575,1.78721011940121,1.78683826379076,1.78647357267649,1.7861158266334,1.785764814841,1.78542033465335,1.78508219119552,1.78475019698458,1.78442417157328,1.78410394121493,1.78378933854785,1.78348020229813,1.78317637699939
    };

    double ref_idx;
    double dis_idx;
};

class isotropic : public material{
public:
    isotropic(shared_ptr<texture> a) : albedo(a){}

    virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            spectrum& attenuation,
            ray& scattered
            ) const {
        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u,rec.v,rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};

#endif //SPECTRAL_RAYTRACING_MATERIAL_H
