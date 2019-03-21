/*
	file 		battery.h
	author	Urs Hofmann
	date		20. Okt 2017
	mail		urshofmann@gmx.net
*/

#ifndef BATTERY_H
#define BATTERY_H

class battery
{
private:
	float mass; // [g]
	float volume; // [mm³]

	// Constant values
	float specific_v_energy = 0.000373; // [Wh / mm^3]
	float specific_m_energy = 0.215; // [Wh / g]

	float energy; // [Wh]

	bool status_energy = 0;

public:

	void set_energy(const float& _energy); // [Wh]

	// return functions for mass and volume of battery
	float get_mass(); // [g]
	float get_volume(); // [mm^3]
	float get_energy() const {return energy;}; // Wh
};

#endif
