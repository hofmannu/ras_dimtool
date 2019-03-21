/*

File		battery.cpp
Author	Urs Hofmann
Mail		urshofmann@gmx.net, hofmannu@student.ethz.ch
Date		20. Okt 2017

Class representing the properties of a battery. The volume-to-energy and
mass-to-energy ratios of an averaged battery are used to calculate the mass and
volume of the required battery.
*/


#include "battery.h"
#include <iostream>

using namespace std;

void battery::set_energy(const float& _energy) // [Wh]
{
	energy = _energy;
	status_energy = 1;
}

float battery::get_mass()
{
	if (status_energy == 1)
	{
		mass = energy / specific_m_energy;
		return mass; // [g]
	}
	else
	{
		cout << " E: Cannot define mass of energy w/o knowing req. energy" << endl;
		return 0;
	}
}

float battery::get_volume()
{
	if (status_energy == 1)
	{
		volume = energy / specific_v_energy;
		return volume; //[mm^3]
	}
	else
	{
		cout << " E: Cannot define volume of energy w/o knowing req. energy" << endl;
		return 0;
	}

}
