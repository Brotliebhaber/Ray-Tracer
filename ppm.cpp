#include <iostream>
#include <fstream>
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"


auto radius = 0.5;
vec3 sphere_center(0.0, 0.0, -1.0);

//for a given ray, radius and sphere_center calcualte if the ray hits the sphere by solving for h:  (h ± √h²a) / a
double hit_sphere(const ray& r, float radi, const vec3 center) {
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radi * radi;

    auto D = h*h - a*c;
    double t = (h - std::sqrt(D)) / a;
    if(t > 0){
        return t;
    }
    else {
        return -1.0;
    }
    

    //std::cout << D;
    /*if(D > 0) {
        return true;
    }
    else {
        return false;
    }*/

}


color ray_color(const ray& r, const hittable_list& object_list) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    hit_record hits;
    if(object_list.hit(r, 0.001, INFINITY, hits)){
        return(hits.normal.x()*color(1.0,0.0,0.0) + hits.normal.y()*color(0.0,1.0,0.0) + hits.normal.z()*color(0.0,0.0,1.0));
    }
    else {
        return (1.0-a)*color(1.0,1.0,1.0) + a * color(0.5, 0.7, 1.0);
    }
    

    //double t = hit_sphere(r, radius , sphere_center);

    //std::cout << t;
    /*if (t > 0){
        point3 p = r.origin() + t * r.direction();
        vec3 p_normal = (p - sphere_center)/radius;
        return p_normal.x()*color(1.0,0.0,0.0) + p_normal.y()*color(0.0,1.0,0.0) + p_normal.z()*color(0.0,0.0,1.0);
    }
    else {
        return (1.0-a)*color(1.0,1.0,1.0) + a * color(0.5, 0.7, 1.0);
    }*/
    
}

int main() {

    // Image Size
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    hittable_list scene;
    auto sphere_1 = std::make_shared<sphere>(0.3, vec3(-2.0,1.0,-2.0));
    auto sphere_2 = std::make_shared<sphere>(0.5, vec3(2.2,0.0,-3.0));
    auto sphere_3 = std::make_shared<sphere>(0.2, vec3(-0.2,0.0, -1.0));
    auto sphere_4 = std::make_shared<sphere>(1.2, vec3(1.0,4.2, -7.0));
    auto sphere_5 = std::make_shared<sphere>(0.2, vec3(0.0,0.4, -1.0));
    scene.add(sphere_1);
    scene.add(sphere_2);
    scene.add(sphere_3);
    scene.add(sphere_4);
    scene.add(sphere_5);

    // Render
    std::ofstream out("image.ppm");
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);
            //hit_record hits;
            color pixel_color = ray_color(r, scene); 
            write_color(out, pixel_color);
            

        }
    }
    std::clog << "\nDone.\n";
}