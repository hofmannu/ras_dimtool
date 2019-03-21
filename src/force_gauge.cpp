/*
	file 		force_gauge.cpp
	author	Urs Hofmann
	date		27. Dez 2017
	mail		urshofmann@gmx.net
*/

#include "force_gauge.h"

void force_gauge::set_max_force(const float& _maxForce)
{
  maxForce = _maxForce;
}

void force_gauge::set_mass(const float& _mass)
{
  mass = _mass;
}

void force_gauge::set_length(const float& _length)
{
  length = _length;
}
