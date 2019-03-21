/*
  file    tractionwire_housing_bk.h
  author  Urs Hofmann
  date    20. Dez 2017
  mail    urshofmann@gmx.net
*/

#ifndef TRACTIONWIRE_HOUSING_BK_H
#define TRACTIONWIRE_HOUSING_BK_H

class tractionwire_housing_bk
{

private:
  float materialVolume;
  float materialMass;
  float materialDensity = 2.7 / 1000; // [g/mm^3]
  float wallThickness = 1.5; // [mm]

  float outerVolume; // [mm^3]
  float mass; // [g]

  float winchDiameter; // [mm]

  float tolerance = 3; // [mm]

  float outerSideLength; // [mm]

  float fixedLengthSEEs = 50; // [mm]
  float fixedMassSEEs = 10; // [g]
  float fixedHeight = 25; // [mm]

  void calculate_outer_volume();
  void calculate_mass();

public:

  float get_mass() const {return mass;}; // [g]
  float get_volume() const {return outerVolume;}; // [mm^3]
  void set_winch_diameter(const float& _winchDiameter);
  void calculate();
};

#endif
