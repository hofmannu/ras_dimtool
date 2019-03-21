#include <iostream>
#include <math.h>

using namespace std;

#ifndef HYDRAULIC_HOUSING_BK
#define HYDRAULIC_HOUSING_BK

class hydraulic_housing_bk
{

private:
  float length; // [mm]
  // length is the distance between one end of the motor and the force gauge, me
  // anning: lForceGauge + lMasterCylinderExtended + tolerance

  float width; // [mm]
  // width is given as a function of the master cylinder diameter

  float mass; // [g]
  float outerVolume; // [cm^3]

  float densityScrews = 0.008; // [g/mm^3]
  // 8e3 kg / m^3 = 8e6 g / m^3 = 8e-3 g/mm^3

public:

  // Set functions
  void set_length(const float& _length);
  void set_width(const float& _width);

  void calculate();

  // Get functions
  float get_volume() const {return outerVolume;};
  float get_mass() const {return mass;};
};

#endif
