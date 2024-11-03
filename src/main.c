#include <stdio.h>
#include "sensor.h"

int main() {
    struct sensor_params params = {
        .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},

        .orientation = {.p = 0.0f, .y = 0.0f, .r = 0.0f},

        .width = 10,
        .height = 10,
        .pixel_spacing = 1.0};

    // todo:
    // define scene
    //      objects, with refraction, lenses
    // spawn + shoot rays
}
