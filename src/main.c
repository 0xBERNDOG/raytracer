#include <stdio.h>

#include "engine/object.h"
#include "engine/ray.h"
#include "engine/sensor.h"
#include "engine/sphere.h"
#include "version.h"

int main() {
    // todo: populate the world
    struct object world[10];

    struct sensor_params params = {
        .position = {.x = 0.0, .y = 0.0, .z = 0.0},

        .orientation = {.p = 0.0, .y = 0.0, .r = 0.0},

        .width = 10,
        .height = 10,
        .pixel_spacing = 1.0};

    struct sphere sphere = {.position = {.x = 5.0, .y = 5.0, .z = 5.0},
                            .radius = 1};
    struct object sphere_obj = {
        .object = (void*)&sphere,
        .func_ray_intersection = sphere_ray_intersection};
    world[0] = sphere_obj;

    struct ray ray = {.position = {.x = 0.0, .y = 0.0, .z = 0.0},
                      .direction = {.x = 1.0, .y = 0.0, .z = 0.0}};
    for (int x = 0; x < 100; x++) {
        for (int y = 0; y < 100; y++) {
            ray.position.y = x / 10.0;
            ray.position.z = y / 10.0;

            optional_vector hit_pos =
                ray_get_nearest_hit_position(&ray, world, 1);

            if (hit_pos.present) {
                printf(" X");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    // todo:
    // define scene
    //      objects, with refraction, lenses
    // spawn + shoot rays
}
