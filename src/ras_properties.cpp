/*
	file 		ras_properties.cpp
	author 	Urs Hofmann
	date 		25. Dez 2017
	mail 		urshofmann@gmx.net
*/

#include "ras_properties.h"
#include <iostream>

using namespace std;

void ras_properties::set_mass_output(const float& _mass_output)
{
	mass_output = _mass_output; // [g]
}

void ras_properties::set_volume_output(const float& _volume_output)
{
	volume_output = _volume_output; // [cm^3]
}

void ras_properties::set_mass_transmissionsystem(const float& _mass_transmissionsystem)
{
	mass_transmissionsystem = _mass_transmissionsystem; // [g]
}

void ras_properties::set_volume_transmissionsystem(const float& _volume_transmissionsystem)
{
	volume_transmissionsystem = _volume_transmissionsystem; // [cm^3]
}

void ras_properties::set_mass_actuationunit(const float& _mass_actuationunit)
{
	mass_actuationunit = _mass_actuationunit; // [g]
}

void ras_properties::set_volume_actuationunit(const float& _volume_actuationunit)
{
	volume_actuationunit = _volume_actuationunit; // [cm^3]
}

void ras_properties::calc_overall()
{
	mass_overall = mass_transmissionsystem + mass_output + mass_actuationunit;
	volume_overall = volume_transmissionsystem + volume_output
		+ volume_actuationunit;
}

void ras_properties::print_results(ofstream& file)
{
	calc_overall();

	file << mass_output << " " << mass_transmissionsystem << " " <<
		mass_actuationunit << " " << mass_overall << " " << volume_output << " " <<
		volume_transmissionsystem << " " << volume_actuationunit << " " <<
		volume_overall << endl;
}


float ras_properties::get_mass_overall()
{
	mass_overall = mass_actuationunit + mass_transmissionsystem + mass_output;
	return mass_overall;
}

float ras_properties::get_volume_overall()
{
	volume_overall = volume_actuationunit + volume_transmissionsystem + volume_output;
	return volume_overall;
}
