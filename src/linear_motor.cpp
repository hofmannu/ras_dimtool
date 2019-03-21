/*
  file    linear_motor.cpp
  author  Urs Hofmann
  date    25. Dez 2017
  mail    urshofmann@gmx.net
*/

#include "linear_motor.h"

// Set functions

void linear_motor::set_mass(const float& _mass)
{
  mass = _mass; // [g]
}

void linear_motor::set_volume(const float& _volume)
{
  volume = _volume; // [mm^3]
}

void linear_motor::set_force(const float& _force)
{
  force = _force; // [N]
}

void linear_motor::set_current(const float& _current)
{
  current = _current; // [A]
}

void linear_motor::set_voltage(const float& _voltage)
{
  voltage = _voltage; // [V]
}

void linear_motor::set_speed(const float& _speed)
{
  speed = _speed; // [mm/s]
}

void linear_motor::set_stroke(const float& _stroke)
{
  stroke = _stroke; // [mm]
}

// Get functions

float linear_motor::get_power()
{
  float power = force * speed / 1000; // [N * m/s] = [W]
  return power;
}
