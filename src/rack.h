#ifndef RACK_H
#define RACK_H

class rack
{
private:
  float length; // [mm]
  float max_force; // [N]
  float height; // [mm]
  float width; // [mm]
  float modulus; // [1]
  float mass_per_length; // [g/mm]
  float mass; // [g]
  float volume; // [mm^3]

public:

  // Set properties of rack
  void set_length(const float& _length);
  void set_max_force(const float& _max_force);
  void set_height(const float& _height);
  void set_width(const float& _width);
  void set_modulus(const float& modulus);
  void set_mass_per_length(const float& _weight_per_length);

  // Get values
  float get_mass();
  float get_volume();
  float get_height() const {return height;}; // [mm]
  float get_width() const {return width;};


};

#endif
