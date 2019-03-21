/*
	file 		pneuamtic_valve.h
	author	Urs Hofmann
	date 		27. Okt 2017
	mail 		urshofmann@gmx.net
*/


#ifndef PNEUMATIC_VALVE_H
#define PNEUMATIC_VALVE_H

class pneumatic_valve
{
private:
	float mass;
	float volume;
	float max_pressure;
	float current; // [A]
	float voltage; // [V]
	float max_working_pressure;

public:
	float get_mass() const {return mass;};
	float get_volume() const {return volume;};
	float get_voltage() const {return voltage;};
	float get_current() const {return current;};

	float set_current(const float& _current);
	float set_voltage(const float& _voltage);
	float set_mass(const float& _volume);
	float set_volume(const float& _mass);
};

#endif
