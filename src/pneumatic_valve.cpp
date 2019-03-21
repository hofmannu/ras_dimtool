#include "pneumatic_valve.h"

float pneumatic_valve::set_current(const float& _current)
{
	current = _current; // [A]
}

float pneumatic_valve::set_voltage(const float& _voltage)
{
	voltage = _voltage; // [V]
}

float pneumatic_valve::set_mass(const float& _mass)
{
	mass = _mass;
}

float pneumatic_valve::set_volume(const float& _volume)
{
	volume = _volume;
}