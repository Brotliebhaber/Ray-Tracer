#ifndef SHADING_H
#define SHADING_H
#include <algorithm>
#include <cmath>
#include "vec3.h"


// Eine Punkt-Lichtquelle (Position, Lichtfarbe, Lichtstärke)
class Point_Light{
    public:
        vec3 position;
        vec3 color;
        float luminosity;
        Point_Light(vec3 pos, vec3 col, float l){
            position = pos;
            color = col;
            luminosity = l;
        }
};



//Phong Material beinhaltet Material informationen zu Farbe und einen Wert von 0-1 für jede Reflektion und den shininess Exponent
class Phong_Material
    {
        private:
            vec3 color;         //Albedo
            float ambient;      //Stärke des Umgebungslichts
            float diffuse;      //Stärke des Matten Lichts
            float specular;     //Stärke des Glanzpunktes
            float shininess;    //Exponent für den Glanzpunkt
        public:
            Phong_Material(vec3 col, float amb, float dif, float spec, float shin){
            ambient = amb;
            diffuse = dif;
            specular = spec;
            color = col;
            shininess = shin;
            }
            vec3 get_color()        const {return color;}
            float get_ambient()     const {return ambient;}
            float get_diffuse()     const {return diffuse;}
            float get_specular()    const {return specular;}
            float get_shininess()   const {return shininess;}
    };

#endif