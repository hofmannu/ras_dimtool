/*
	file 		force_gauge.h
	author	Urs Hofmann
	date		27. Dez 2017
	mail		urshofmann@gmx.net
*/

#ifndef FORCE_GAUGE_H
#define FORCE_GAUGE_H

class force_gauge
{
private:
  float maxForce; // [N]
  float mass; // [g]
  float length; // [mm]

public:
  // Set functions
  void set_max_force(const float& _maxForce);
  void set_mass(const float& _mass);
  void set_length(const float& _length);

  // Get functions
  float get_mass() const {return mass;};
  float get_length() const {return length;};
  float get_max_force() const {return maxForce;};
};

#endif
