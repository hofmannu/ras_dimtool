/*
	file 		bearing.cpp
	author	Urs Hofmann
	date 		29. Okt 2017
	mail 		urshofmann@gmx.net, hofmannu@student.ethz.ch
  phone   +49 1573 0711 493
*/

#include "bearing.h"

// Set functions

void bearing::set_inner_radius(const float& _inner_radius)
{
  inner_radius = _inner_radius;
}

void bearing::set_outer_radius(const float& _outer_radius)
{
  outer_radius = _outer_radius;
}

void bearing::set_height(const float& _height)
{
  height = _height;
}

void bearing::set_C(const float& _C)
{
  C = _C;
}

void bearing::set_C_0(const float& _C_0)
{
  C_0 = _C_0;
}

void bearing::set_max_dyn_load(const float& _max_dyn_load)
{
  max_dyn_load = _max_dyn_load;
}

void bearing::set_max_static_load(const float& _max_static_load)
{
  max_static_load = _max_static_load;
}

void bearing::set_mass(const float& _mass)
{
  mass = _mass;
}

// Calculate and return volume

float bearing::get_volume()
{
  volume = M_PI * pow(outer_radius, 2) * height;
  return volume;
}
