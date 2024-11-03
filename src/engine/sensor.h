#ifndef SENSOR_H
#define SENSOR_H

#include "angle.h"
#include "vector.h"

struct sensor_params {
    vector position;
    struct angle orientation;  // (0,0,0) should be: "up" is +z, "forward" is +x

    // pixels
    int width;
    int height;

    float pixel_spacing;  // for now, pixels are single points (not squares)
                          // with a defined spacing
};

#endif
