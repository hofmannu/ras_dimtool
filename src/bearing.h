/*
	file 		bearing.h
	author	Urs Hofmann
	date 		29. Okt 2017
	mail 		urshofmann@gmx.net, hofmannu@student.ethz.ch
  phone   +49 1573 0711 493
*/

#include <math.h>

#ifndef BEARING_H
#define BEARING_H

class bearing
{
private:
  // dimensions
  float inner_radius; // [mm]
  float outer_radius; // [mm]
  float height; // [mm]
  float volume; // [mm^3]

  // load
  float C;
  float C_0;

  // load limits
  float max_static_load; // [N]
  float max_dyn_load; // [N]

  // mass
  float mass; // [g]

public:

  // Set dimensions
  void set_inner_radius(const float& _inner_radius); // [mm]
  void set_outer_radius(const float& _outer_radius);
  void set_height(const float& _height);

  // Set load limits
  void set_max_dyn_load(const float& _max_dyn_load);
  void set_max_static_load(const float& _max_static_load);

  // Set loads
  void set_C_0(const float& _C_0);
  void set_C(const float& _C);

  // Set weight
  void set_mass(const float& _mass);

  // Get dimensions
  float get_inner_radius() const {return inner_radius;}; // [mm]
  float get_outer_radius() const {return outer_radius;}; // [mm]
  float get_height() const {return height;};
  float get_volume();

  // Get loads
  float get_C() const {return C;};
  float get_C_0() const {return C_0;}

  // Get load limits
  float get_max_static_load() const {return max_static_load;};
  float get_max_dyn_load() const {return max_dyn_load;};

  // Get mass
  float get_mass() const {return mass;};
};

#endif
