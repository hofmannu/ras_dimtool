#ifndef PNEUMATIC_TUBE_H
#define PNEUMATIC_TUBE_H

class pneumatic_tube
{
private:

  // Dimensions of tube
  float inner_diameter; //[mm]
  float outer_diameter; //[mm]
  float length; // [mm]

  // Maximum pressure
  float max_pressure; // [bar]

  // Volume
  float inner_volume; // [mm^3]
  float outer_volume; // [mm^3]

  // Mass
  float mass_per_length; // [g/mm]
  float mass; // [g]

public:

  // Set required properties
  void set_inner_diameter(const float& _inner_diameter);
  void set_outer_diameter(const float& _outer_diameter);
  void set_mass_per_length(const float& _mass_per_length);
  void set_max_pressure(const float& _max_pressure);
  void set_length(const float& _length);

  // Calculate
  void calculate_vol_and_mass();

  // Get required properties
  float get_inner_diameter() const {return inner_diameter;};
  float get_outer_diameter() const {return outer_diameter;};
  float get_length() const {return length;};
  float get_inner_volume() const {return inner_volume;};
  float get_outer_volume() const {return outer_volume;};
  float get_mass_per_length() const {return mass_per_length;};
  float get_mass() const {return mass;};

};


#endif
