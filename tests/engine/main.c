#include "tests/engine/main.h"

#include "minunit.h"
#include "tests/engine/aperture.h"
#include "tests/engine/lens_ideal.h"
#include "tests/engine/lens_spherical.h"
#include "tests/engine/plane.h"
#include "tests/engine/ray.h"
#include "tests/engine/sensor.h"
#include "tests/engine/sphere.h"
#include "tests/engine/vector.h"

char *
test_engine_all()
{
	// run all engine tests
	mu_run_test_group(test_aperture_all);
	mu_run_test_group(test_lens_ideal_all);
	mu_run_test_group(test_lens_spherical_all);
	mu_run_test_group(test_plane_all);
	mu_run_test_group(test_ray_all);
	mu_run_test_group(test_sensor_all);
	mu_run_test_group(test_sphere_all);
	mu_run_test_group(test_vector_all);
	return 0;
}
