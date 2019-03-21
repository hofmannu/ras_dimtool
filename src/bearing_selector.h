/*
	file 		bearing_selector.h
	author	Urs Hofmann
	date 		27. Okt 2017
	mail 		urshofmann@gmx.net
*/

#include "bearing.h"
#include "file_handling.h"
#include <cmath>
#include <vector>

using namespace std;

#ifndef BEARING_SELECTOR_H
#define BEARING_SELECTOR_H

class bearing_selector
{
private:
  bool doInteractive = 0;

  file_handling database_file;

  vector<vector<float>> database_raw_data;
  vector<bearing> database_bearing;

  bearing selected_bearing;

  void read_database();
  void exclude_weak_bearings();
  void select_smallest_and_lightest_bearing();

  // relevant variables for selection (input)
  vector<float> F_r_1; // [N] radial force upper bearing
  vector<float> F_r_2; // [N] radial force lower bearing
  float F_a_1 = 0; // [N] axial force upper bearing
  float F_a_2 = 0; // [N] axial force lower bearing

  float F_r_max; // [N] Maximum radial force
  float F_a_max = 0;

  // relevant variables for selection (calculated)
  float P_0; // [N] statisch äquivalente Lagerbelastung
  float C_0; // [N] statische Tragzahl
  float P; // [N] zeitlich äquivalente Belastung
  float C; // [N] dynamische Tragzahl

  // True distances
  float d_r1_rope;
  float d_rope_gear;
  float d_gear_r2;

  // constant values for radial ball bearing
  const float X_0 = 0.6;
  const float Y_0 = 0.5;
  const float X = 1;
  const float Y = 0;
  const float f_s = 0.5; // statische Tragsicherheit;

public:
  bearing select_bearing(const float& _F_rope, const float& _F_gear,
    const float& _d_r1_rope, const float& _d_rope_gear, const float& _d_gear_r2);
    // _F_rope        Force acting on the rope
    // _F_gear        Force acting on the rack / output force
    // _d_r1_rope     Distance between upper bearing and mid of winch
    // _d_rope_gear   Distance between mid of winch and mid of gear
    // _d_gear_r2     Distance between mid of gear and lower bearing
    // Careful: The distances do not include the thickness of the corresponding
    // bearing since this might differ between the bearings themselve. We have
    // to add it later in the selection process.

  void set_do_interactive(const bool& _doInteractive);
};

#endif
