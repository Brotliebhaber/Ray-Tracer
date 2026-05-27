#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "triangle.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "shading.h"

class cube : public hittable{
    private:
        mesh _mesh;
        std::shared_ptr<Phong_Material> material;

        void add_quad(point3 p1, point3 p2, point3 p3, point3 p4, point3 orig, double angle_x, double angle_y, double angle_z) { //Definiert die Seite eines Würfels durch Dreiecke indem es 4 point3 nimmt
            int base_index = _mesh.vertices.size();
            //std::cout << base_index;
            //Richtungsvektoren berechnen
            
            p1 = rotate(p1, angle_x, angle_y, angle_z) + orig;
            p2 = rotate(p2, angle_x, angle_y, angle_z) + orig;
            p3 = rotate(p3, angle_x, angle_y, angle_z) + orig;
            p4 = rotate(p4, angle_x, angle_y, angle_z) + orig;

            vec3 edge1 = p2 - p1;
            vec3 edge2 = p4 - p1;
            
            //Kreuzprodukt nehmen
            vec3 raw_normal = cross(edge1, edge2);
           
            //Vektor normieren 
            vec3 normal = unit_vector(raw_normal);

            //Definiere die 4 Ecken der Seite
            _mesh.vertices.push_back(vertex(p1, normal));
            _mesh.vertices.push_back(vertex(p2, normal));
            _mesh.vertices.push_back(vertex(p3, normal));
            _mesh.vertices.push_back(vertex(p4, normal));

            //Dreiecke definieren welche die Seite bilden
            _mesh.indices.push_back(triangle(base_index, base_index + 1, base_index + 2));
            _mesh.indices.push_back(triangle(base_index, base_index + 2, base_index + 3));
            //std::cout << (base_index, base_index + 1, base_index + 2);
        }

        //rotate eventuell durch rotationsmatrix ersetzen um performance zu verbessern
        point3 rotate(point3 p, double angle_x, double angle_y, double angle_z){
            double rad_x = angle_x * M_PI / 180.0;
            double rad_y = angle_y * M_PI / 180.0;
            double rad_z = angle_z * M_PI / 180.0;

            double x = p.x(), y = p.y(), z = p.z();


            //Rotation um die x-Achse
            if (angle_x != 0) {
                double current_y = y;
                y = current_y * cos(rad_x) - z * sin(rad_x);
                z = current_y * sin(rad_x) + z * cos(rad_x);
            }

            //Rotation um die y-Achse
            if (angle_y != 0) {
                double current_x = x;
                double current_z = z;
                x = current_x * cos(rad_y) + current_z * sin(rad_y);
                z = -current_x * sin(rad_y) + current_z * cos(rad_y);
            }

            //Rotation um die z-Achse
            if (angle_z != 0) {
                double current_z = z;
                double current_x = x;
                double current_y = y;
                x = current_x * cos(rad_z) - current_y * sin(rad_z);
                y = current_x * sin(rad_z) + current_y * cos(rad_z);
            }

            return(point3(x,y,z));
        }

    public:
        cube(point3 orig, float size, double angle_x, double angle_y, double angle_z, std::shared_ptr<Phong_Material> m) {

            material = m;
            _mesh.vertices.reserve(24);
            _mesh.indices.reserve(12);

            float h = size / 2.0f;
            //definieren der Extremen
            auto x0 = -h, x1 = h;
            auto y0 = -h, y1 = h;
            auto z0 = -h, z1 = h;

            //Jede Seite wird von unten links gegen den Uhrzeigersinn nach oben links definiert.
            add_quad(point3(x0, y0, z0), point3(x1, y0, z0), point3(x1, y1, z0), point3(x0, y1, z0), orig, angle_x, angle_y, angle_z); // Vorne
            add_quad(point3(x1, y0, z1), point3(x0, y0, z1), point3(x0, y1, z1), point3(x1, y1, z1), orig, angle_x, angle_y, angle_z); // Hinten
            add_quad(point3(x0, y0, z1), point3(x0, y0, z0), point3(x0, y1, z0), point3(x0, y1, z1), orig, angle_x, angle_y, angle_z); // Links
            add_quad(point3(x1, y0, z0), point3(x1, y0, z1), point3(x1, y1, z1), point3(x1, y1, z0), orig, angle_x, angle_y, angle_z); // Rechts
            add_quad(point3(x0, y1, z0), point3(x1, y1, z0), point3(x1, y1, z1), point3(x0, y1, z1), orig, angle_x, angle_y, angle_z); // Oben
            add_quad(point3(x0, y0, z1), point3(x1, y0, z1), point3(x1, y0, z0), point3(x0, y0, z0), orig, angle_x, angle_y, angle_z); // Unten

        }
        bool hit(const ray& r, double t_min, double t_max, hit_record& hits) const override{
            bool hit_anything = false;
            auto closest = t_max;
            for(int i = 0; i < _mesh.indices.size(); i++) {
                if(_mesh.indices[i].hit(r, t_min, closest, hits, _mesh)) {
                    hit_anything = true;
                    //std::cout << hit_anything;
                    closest = hits.t;
                }
            }
            if(hit_anything){
                hits.material = this->material;
            }
            return hit_anything;
        }

};





#endif