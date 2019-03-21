/*
	file 		ras_properties.h
	author	Urs Hofmann
	date 		12. Dez 2017
	mail 		urshofmann@gmx.net
*/

#include <fstream>

using namespace std;

#ifndef RAS_PROPERTIES_H
#define RAS_PROPERTIES_H

class ras_properties
{
private:
	// Mass in [g], Volume in [cm3]
	float mass_output = 0; // [g]
	float volume_output = 0;

	float mass_transmissionsystem = 0;
	float volume_transmissionsystem = 0;

	float mass_actuationunit = 0;
	float volume_actuationunit = 0;

	float mass_overall = 0;
	float volume_overall = 0;

	void calc_overall();

public:
	// Set functions
	void set_mass_output(const float& _mass_output);
	void set_volume_output(const float& _volume_output);
	void set_mass_transmissionsystem(const float& _mass_transmissionsystem);
	void set_volume_transmissionsystem(const float& _volume_transmissionsystem);
	void set_mass_actuationunit(const float& _mass_actuationunit);
	void set_volume_actuationunit(const float& _volume_actuationunit);

	// Get functions
	float get_mass_output() const {return mass_output;};
	float get_mass_transmission_system() const {return mass_transmissionsystem;};
	float get_mass_actuation_unit() const {return mass_actuationunit;};
	float get_mass_overall();

	float get_volume_output() const {return volume_output;};
	float get_volume_transmission_system() const
		{return volume_transmissionsystem;};
	float get_volume_actuation_unit() const {return volume_actuationunit;};
	float get_volume_overall();

	void print_results(ofstream& file);
};

#endif
