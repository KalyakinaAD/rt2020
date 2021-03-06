//
// Created by alina on 06.06.2020.
//

#ifndef RT_PRIMITIVES_H
#define RT_PRIMITIVES_H

#include <vector>
#include "my_math.h"

#define EPS 1e-3
#define MAX_DIST 100

struct Ray {
    vec3f dir;
    vec3f orig;
    Ray(const vec3f &o, const vec3f &d);
};

struct Light {
    Light(const vec3f &p, float i);
    vec3f position;
    float intensity;
};

struct Material {
    vec4 diff_spec_refl_refr;
    vec3f color;
    float specular_exponent;
    float refractive_index;
    Material(const vec4 &a, const vec3f &color, float spec, float r);
    Material();
};


struct TexMaterial:Material {
    std::vector<vec3f> texture;
    int tex_w{}, tex_h{};
};

struct Hit {
    bool hit = false;
    vec3f point;
    float dist{};
    vec3f n;
    Material material;

    Hit();
    Hit(const vec3f &hit_point, float d, const vec3f& norm, const Material &mat);
    operator bool ();
};

struct Primitive {
    //Material material;
    //explicit Primitive(const Material &m):material(m) {}
    virtual Hit ray_intersect(const Ray& ray) const = 0;
    virtual float dist(const vec3f& point) const = 0;
    virtual Material get_material(const vec3f& point) const = 0;
    virtual ~Primitive() = default;
};

struct Sphere: Primitive {
    vec3f center;
    float r;
    Material material;
    Sphere(const vec3f &c, float r, const Material &m);
    Hit ray_intersect(const Ray& ray) const override;
    float dist(const vec3f& point) const override;
    Material get_material(const vec3f& point) const override;
};

struct Cone: Primitive {
    float h;
    vec2f c;
    Material material;
    vec3f pos;
    // c is the sin/cos of the angle, h is height
//        // Alternatively pass q instead of (c,h),
//        // which is the point at the base in 2D
    Cone(const vec3f &pos, float h, const vec2f &c, const Material &mat);
    Hit ray_intersect(const Ray& ray) const override;
    float dist(const vec3f& point) const override;
    Material get_material(const vec3f& point) const override ;
};


struct Box: Primitive {
    Material material;
    vec3f pos;
    vec3f b; //dims

    Box(const vec3f &pos, const vec3f &dims, const Material &material);
    Hit ray_intersect(const Ray& ray) const override;
    float dist(const vec3f& point) const override;
    Material get_material(const vec3f& point) const override ;
};

struct HorPlane : Primitive {
    float y = -4;
    Material material;
    std::vector<vec3f> texture;
    int tex_w = -1, tex_h=-1;
    HorPlane(float d, Material mat);
    HorPlane(float d, Material mat, std::vector<vec3f> tex, int tex_width, int tex_height);
    Hit ray_intersect(const Ray &ray) const override;
    float dist(const vec3f& point) const override;
    Material get_material(const vec3f& point) const override;

};


struct Fractal : Primitive {
    Material material;
    vec3f pos;
    Fractal(const vec3f &_pos, const Material &mat);
    Hit ray_intersect(const Ray &ray) const override;
    float dist(const vec3f& point) const override;
    Material get_material(const vec3f& point) const override;

};


struct Background {

    float r;
    std::vector<vec3f> texture;
    int tex_w{}, tex_h{};
    const Sphere env;

    Background(float d, std::vector<vec3f> tex, int tex_width, int tex_height);
    vec3f get_color(const Ray &ray) const;

};

#endif //RT_PRIMITIVES_H
