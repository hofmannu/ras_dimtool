#include <math.h>
#include <iostream>

#ifndef TRACTIONWIRE_H
#define TRACTIONWIRE_H

class tractionwire
{
private:

  // Mass
  float mass; // [g]
  float specific_mass = 0.011; // [g/mm]

  // Volume
  float volume; // [mm^3]
  float outer_diameter = 1.6; // [mm]

  // Length
  float length = 0; // [mm]

  // Efficiency
  float coefficient_of_friction = 0.2; // [-]
  float bending_angle = 0; // [deg]
  float efficiency;

  void calculate_mass();
  void calculate_volume();
  void calculate_efficiency();

public:

  // Set functions
  void set_length(const float& _length);
  void set_bending_angle(const float& _bending_angle);

  // Calculations
  void calculate();

  // Return functions for a single wire (2 needed for setup)
  float get_mass() const {return mass;}; // [g]
  float get_volume() const {return volume;}; // [mm^3]
  float get_efficiency() const {return efficiency;}; // [1]

};

#endif
