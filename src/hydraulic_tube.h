#ifndef HYDRAULIC_TUBE_H
#define HYDRAULIC_TUBE_H

class hydraulic_tube
{
private:

  float length; // [mm]
  float rInner; // [mm]
  float rOuter; // [mm]
  float densityWater = 0.001; // [g/mm^3]
  float densityPolymer = 0.0009; // [g/mm^3]
  float flowRate;

  float mass; // [g]
  float outerVolume; // [cm^3]

public:
  // Set function
  void set_length(const float& _length); // [mm]
  void set_flow_rate(const float& _flowRate); // [mm^3 / s]
  //void set_inner_radius(const float& _rInner); // [mm]
  //void set_outer_radius(const float& _rOuter); // [mm]

  // Calculations
  void calculate();

  // Get functions
  float get_mass() const {return mass;}; // [g]
  float get_outer_volume() const {return outerVolume;}; // [mm^3]
  float get_inner_radius() const {return rInner;}; // [mm]
  float get_outer_radius() const {return rOuter;}; // [mm]
};


#endif
