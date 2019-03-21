/*
  file    hydraulic_cylinder.h
  author  Urs Hofmann (based on Gerwin Smiths work, modified a lot)
  date    24. Dez 2017
  mail    urshofmann@gmx.net
*/

#include <iostream>
#include <fstream>

#ifndef HYDRAULIC_CYLINDER_H
#define HYDRAULIC_CYLINDER_H

using namespace std;

class hydraulic_cylinder
{
public:
  // Get functions
  float get_mass() const {return massOverall;}; // [g]
  float get_outer_volume() const {return outerVolume;}; // [mm^3]
  float get_bore_diameter() const {return dBore;}; // [mm]
  float get_rod_bore_ratio();
  float get_fluid_volume(); // [mm^3]
  float get_extended_length(); // [mm]
  float get_outer_diameter(); // [mm]

  // Set functions
  void set_fluid_volume(const float& _fluidVolume);
  void set_rod_force(const float& _rodForce);
  void set_stroke(const float& _stroke);
  void set_rod_bore_ratio(const float& _rodBoreRatio);
  void calculate();

private:

  // Input variables
  float rodForce;
  bool statusRodForce = 0;
  float stroke;
  bool statusStroke = 0;
  float fluidVolume; // [mm^3]
  bool statusFluidVolume = 0;
  float rodBoreRatio;
  bool statusRodBoreRatio = 0;

  // Constants
  float maxPressure = 50; // [bar] = 10^5 N/m^2
  float densityMaterial = 0.008; // [g/mm^3];
  float densityFluid = 0.001; // [g/mm^3]
  int noConnectors = 2; // 2 for double acting, 1 for single acting
  float massConnector = 1; // [g]
  float tORing = 1; // [mm]
  float tPiston = 2; // [mm]
  float addRodLength = 5; // [mm] rod length for attachment
  float tLid = 4; // [mm]

  // Calculated variables
  float dBore; // [mm]
  float dRod; // [mm] <--
  float tWall; // [mm]
  float extendedLength; // [mm]

  float massMaterial;
  float massFluid;

  float massOverall; // [g]
  float outerVolume; // [mm^3]

  // Functions
  void calculate_bore_diameter();
  void calculate_rod_diameter();
  void calculate_stroke();

  void calculate_wall_thickness();

  void calculate_outer_volume();

  void calculate_material_mass();
  void calculate_fluid_mass();

  void calculate_mass();
};

#endif
