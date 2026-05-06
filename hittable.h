#ifndef HITTABLE_H
#define HITTABLE_H
#include <vector>
#include "vec3.h"
#include "ray.h"

//A note of the closest recorded hit (gets updated at each closer hit) created for every ray
struct hit_record
{
    double t;
    point3 p;
    vec3 normal;
};
//Each class hittable should have the function hit
class hittable
{
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& hits) const = 0;
};

class hittable_list : public hittable{
    public:
        std::vector<std::shared_ptr<hittable>> hlist;
        
        bool hit(const ray& r, double t_min, double t_max, hit_record& hits) const override {
            auto closest = t_max;
            bool hit_any = false;
            for(int x = 0; x < hlist.size(); x ++){
                if(hlist[x]->hit(r, t_min, closest, hits)){
                    closest = hits.t;
                    hit_any = true;
                };
            }
            return hit_any;
        }
        void add(std::shared_ptr<hittable> object){
            hlist.push_back(object);
        }
};

#endif