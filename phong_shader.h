#ifndef PHONGSHADER_H
#define PHONGSHADER_H
#include <algorithm>
#include <cmath>
#include "vec3.h"
#include "hittable.h"
#include "shading.h"


class Phong_Shader{
    public:
        static vec3 compute_light(const hit_record& hits, const vec3& camera_pos, const Point_Light& light)
        {
            const Phong_Material& mat = *(hits.material);

            vec3 N = hits.normal;
            vec3 L = unit_vector(light.position - hits.p);
            vec3 V = unit_vector(camera_pos - hits.p);

            vec3 R = unit_vector(2.0f * dot(N,L) * N - L);

            //Ambient Light
            vec3 ambient_part = mat.get_ambient() * mat.get_color();

            //Diffuse Light
            float cos_theta = dot(N,L);
            float diffuse_factor = std::max(0.0f, cos_theta);
            vec3 diffuse_part = mat.get_diffuse() * diffuse_factor * mat.get_color();

            //Specular Light
            float cos_phi = dot(R, V);
            float specular_factor = std::max(0.0f, cos_phi);

            float shiny_power = std::pow(specular_factor, mat.get_shininess());
            vec3 specular_part = mat.get_specular() * shiny_power * light.color;

            //add up all Light and return color
            return ambient_part + (diffuse_part + specular_part) * light.color;
        };
};

#endif