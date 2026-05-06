#ifndef SPHERE_H
#define SPHER_H
#include "ray.h"
#include "vec3.h"
#include "hittable.h"

class sphere : public hittable {
    private:
        double radius;
        point3 center;
    public: 
        sphere(double radius, const point3& center) : radius(radius), center(center){}

        //check wether the ray hits the sphere and change the hit_records recordingly
        bool hit(const ray& r, double t_min, double t_max, hit_record& hits) const override{
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius * radius;
            
            auto Determinant = h*h - a*c;
            
            if(Determinant < 0){return false;}
            
            double sqrtd = std::sqrt(Determinant);
            double t = (h - sqrtd) / a;
            //check wether the ray hits and is in range of thresholds
            if (t >= t_max || t <= t_min){
                t = (h + sqrtd) / a;
                if (t >= t_max || t <= t_min){
                    return false;
                }
            }
            //update hit_records with new nearest
            hits.t = t;
            hits.p = r.at(t);
            hits.normal = (hits.p - center) / radius;
            return true;

        }


};




#endif