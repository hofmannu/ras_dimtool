/*
  File      rack_selector.cpp
  Author    Urs Hofmann
  Date      31. Okt 2017
  Version   1.0
  Mail      urshofmann@gmx.net, hofmannu@student.ethz.ch
  Phone     +49 1573 0711 493

  This class selects a rack out of a database as a function of stroke and
  force
*/

#include "rack_selector.h"

rack rack_selector::select_rack(const float& _length, const float& _force)
{
  // Set requirements for rack mechanism
  length = _length;
  force = _force;

  // Scale height according to transmitted force
  selected_rack.set_height(force / 150 * 4.5); // [mm]
  selected_rack.set_width(force / 150 * 4.5 * 4 /
  selected_rack.get_height());

  // Set mass per length according to area
  selected_rack.set_mass_per_length(0.0011688888889 *
  selected_rack.get_height() * selected_rack.get_width());

  // Set modulus to 0.5
  selected_rack.set_modulus(0.5);

  // Set lenght
  selected_rack.set_length(length);

  return selected_rack;
}
