#include "rack.h"

// Set parameters of rack

void rack::set_length(const float& _length)
{
  length = _length;
}

void rack::set_max_force(const float& _max_force)
{
  max_force = _max_force;
}

void rack::set_height(const float& _height)
{
  height = _height;
}

void rack::set_width(const float& _width)
{
  width = _width;
}

void rack::set_modulus(const float& _modulus)
{
  modulus = _modulus;
}

void rack::set_mass_per_length(const float& _mass_per_length)
{
  mass_per_length = _mass_per_length;
}

// Get properties
float rack::get_mass()
{
  mass = mass_per_length * length;
  return mass;
}

float rack::get_volume()
{
  volume = length * width * height;
  return volume;
}
