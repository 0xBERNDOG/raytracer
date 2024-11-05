#ifndef SENSOR_H
#define SENSOR_H

#include "engine/angle.h"
#include "engine/vector.h"

struct sensor_params {
    struct vector position;
    struct angle orientation;  // (0,0,0) should be: "up" is +z, "forward" is +x

    // pixels
    int width;
    int height;

    double pixel_spacing;  // for now, pixels are single points (not squares)
                           // with a defined spacing
};

#endif
