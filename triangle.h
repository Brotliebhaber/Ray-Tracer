#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <vector>
#include "vec3.h"
#include "ray.h"
#include "hittable.h"

struct triangle;

struct vertex {
    vec3 position; // Position
    vec3 normal; // Vertex normal

    vertex() : position(point3(0,0,0)), normal(vec3(0,0,0)) {}
    vertex(vec3 pos, vec3 norm) : position(pos), normal(norm) {}
};  

struct mesh {
    std::vector<vertex> vertices;
    std::vector<triangle> indices;
};



class triangle{
    private:    
        int v1, v2, v3; //indices in the vertex array
    public:
        //triangle(int v1, int v2, int v3) : v1(v1), v2(v2), v3(v3){}
        //triangle() : v1(0), v2(0), v3(0) {}
        triangle(int i1, int i2, int i3){
            v1 = i1;
            v2 = i2;
            v3 = i3;
        }

        bool hit(const ray& r, double t_min, double t_max, hit_record& hits, const mesh& _mesh) const
        {
            // Edges of the Triangle //
            vec3 edge1 = _mesh.vertices[v2].position - _mesh.vertices[v1].position;
            vec3 edge2 = _mesh.vertices[v3].position - _mesh.vertices[v1].position;

            //std::cout << "first edge: " << _mesh.vertices[v2].position << "   second edge: " << _mesh.vertices[v1].position;            
            //std::cout << "    v1:  " << v1 << "    v2:   " << v2 << "   v3:   " << v3;
            vec3 pvec = cross(r.direction(), edge2);

            //std::cout << pvec;
            
            auto det = dot(edge1, pvec);
            //std::cout << "Determinante" << det << "t_min: " << t_min << "\n";
            if (std::abs(det) < t_min){
                //std::cout << "ich"; 
                return false;
            }
            auto invDet = 1.0f/det;
            
            vec3 tvec = r.origin() - _mesh.vertices[v1].position;

            // Calculate U-Paramater and test
            auto u = dot(pvec, tvec) * invDet;
            if (u < 0.0f || u > 1.0f) return false;

            //Calculate V-Parameter and test
            vec3 qvec = cross(tvec, edge1);
            auto v = dot(r.direction(), qvec) * invDet;
            if(v < 0.0f || u + v > 1.0f)return false;

            // Calculate T-Parameter
            auto t = dot(edge2, qvec) * invDet;
            //std::cout << t;
            //std::cout << "Det: " << det << " | U: " << u << " | V: " << v << " | T: " << t << "\n";
            if (t < t_max && t > t_min){
                hits.t = t;
                hits.p = r.at(t);
                //hits.normal = unit_vector(cross(edge2, edge1));
                //hits.normal = _mesh.vertices[v1].normal;
                auto w = 1.0 - u - v;
                /*
                vec3 interpolated_normal = w * _mesh.vertices[v1].normal
                                         + u * _mesh.vertices[v2].normal
                                         + v * _mesh.vertices[v3].normal;
                */
                hits.normal = unit_vector(cross(edge2,edge1));
                //std::cout << hits.normal;
                                
                return true;
            }
            return false;
        }
};

#endif