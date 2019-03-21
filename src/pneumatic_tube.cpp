#include "pneumatic_tube.h"
#include <math.h>

// Set required properties

void pneumatic_tube::set_inner_diameter(const float& _inner_diameter)
{
  inner_diameter = _inner_diameter;
}

void pneumatic_tube::set_outer_diameter(const float& _outer_diameter)
{
  outer_diameter = _outer_diameter;
}

void pneumatic_tube::set_mass_per_length(const float& _mass_per_length)
{
  mass_per_length = _mass_per_length;
}

void pneumatic_tube::set_max_pressure(const float& _max_pressure)
{
  max_pressure = _max_pressure;
}

void pneumatic_tube::set_length(const float& _length)
{
  length = _length;
}

// Calculate

void pneumatic_tube::calculate_vol_and_mass()
{
  mass = length * mass_per_length;
  inner_volume = pow(inner_diameter, 2) * M_PI / 4 * length;
  outer_volume = pow(outer_diameter, 2) * M_PI / 4 * length;
}
