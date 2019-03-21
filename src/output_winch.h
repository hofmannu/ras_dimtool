/*
  File      output_winch.h
  Author    Urs Hofmann
  Mail      urshofmann@gmx.net, hofmannu@student.ethz.ch
  Date      01. Nov 2017
  Version   1.0

  Class representing the end-effector of the tractionwire-approach.
*/

#include "bearing.h"
#include "bearing_selector.h"
#include "rack.h"
#include "rack_selector.h"
#include <math.h>
#include <iostream>

#ifndef OUTPUT_WINCH_H
#define OUTPUT_WINCH_H

class output_winch
{
private:

  // Volume of output
  float outerVolume;

  // weight calculation
  float VMaterial; // [mm^3]
  float VHousing; // [mm^3]
  float density = 2.7 / 1000; // [g/mm^3]
  float massWinch; // [g]
  float massHousing; // [g]
  float tHousing = 1.5; // [mm]
  float tolerance = 0.5; // [mm]

  // radius of winch
  float r_winch; // [mm]
  float r_pinion = 10 * 0.5 / 2; // [mm]

  // distances between components
  float d_r1_rope;
  float d_rope_gear;
  float d_gear_r2;

  // Fixed geometries
  float h_winch = 1.5; // [mm] height of winch
  float h_cover = 1; // [mm]  height of guide putting rope on winch
  float h_buffer = 0.2; // [mm] distance between winch/gear and bearing

  // Bearings
  bearing_selector myBearingSelector;
  bearing myBearing;

  // Rack mechanism
  rack myRack;
  rack_selector myRackSelector;
  float length_rack; // [mm]

  // Speed and force acting on rope
  float v_rope;
  float F_rope;
  const float F_rope_max = 50; // [N]
  // The rope tension is limited to a maximum and its function is similar to the
  // hydraulic or pneumatic maximum pressure.

  // output force of receiving / output winch
  float F_output; // [N]
  bool status_output_force = 0; // 0: not set, 1: set

  // efficiency of transmission system
  float eff_trans_system; // [1]
  float eff_rack_and_pinion = 0.9; // Fixed value, was determined experimentally
  bool status_eff_trans_system = 0; // 0: not set, 1: set

  // Select bearings as a function of the required force
  void select_bearings();

  // Calculation
  void calculate_required_radius();
  void calculate_material_volume();
  void calculate_material_mass();

  void calculate_outer_volume();

public:
  void calculate();

  void set_output_force(const float& _F_output);
  void set_eff_trans_system(const float& _eff_trans_system);
  void set_rack_length(const float& _length_rack);

  float get_mass_housing() const {return massHousing;};
  float get_mass_winch() const {return massWinch;};
  bearing get_bearing() const {return myBearing;};
  rack get_rack() const {return myRack;};
  float get_volume() const {return outerVolume;};
  float get_radius_winch() const {return r_winch;}; // [mm]
  float get_rope_tension_at_input() const {return F_rope_max;};
  float get_radius_pinion() const {return r_pinion;}; // [mm]
};

#endif
