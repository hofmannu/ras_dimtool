#include "pneumatic_tube_selector.h"
#include <math.h>

// Main procedure
pneumatic_tube pneumatic_tube_selector::select_pneumatic_tube
  (const requirements& _my_requirements, const float& _working_pressure,
  const float& _cylinder_bore_diameter)
{
  // Set key variables
  my_requirements = _my_requirements;
  cylinder_bore_diameter = _cylinder_bore_diameter; // [mm]
  working_pressure = _working_pressure; // [bar]

  // Read in pneumatic tube database
  // cout << "Read in pneuamtic tube database..." << endl;
  read_in_database();

  // Exclude turbulent flow tubes
  // cout << "Exclude too small tubes..." << endl;
  exclude_too_small_tubes();

  // Select smallest and lightest tube
  select_smallest_lightest_tube();

  return selected_pneumatic_tube;
}

// Read in database pneumatic_tube_database from file
void pneumatic_tube_selector::read_in_database()
{
  // Clear previously stored datasset
  temp_dataset.clear();
  pneumatic_tube_database.clear();

  // Read file to temporary datasset
  temp_dataset = database.read_file("databases/pneumatic_tubes.dat");
  pneumatic_tube temp_tube;

  /* Translate it into structure
   database:
   inner_diameter[mm] outer_diameter[mm] mass_per_length[g/mm] max_pressure[bar]
  */
  for(unsigned int i = 0; i < temp_dataset.size(); i++)
  {
    temp_tube.set_inner_diameter(temp_dataset[i][0]);
    temp_tube.set_outer_diameter(temp_dataset[i][1]);
    temp_tube.set_mass_per_length(temp_dataset[i][2]);
    temp_tube.set_max_pressure(temp_dataset[i][3]);

    pneumatic_tube_database.push_back(temp_tube);
  }
}

// Exclude tubes which are too small
void pneumatic_tube_selector::exclude_too_small_tubes()
{
  // Exclusion is based upon laminar or turbulent flow
  unsigned int id = 0;
  float reynolds_number = 0;
  float reynolds_number_crit = 2300;
  float density = working_pressure * 100000 / 287.058 / 300;
  /* [density] = Pa * kg * K / J / K
  = N / m^2 * kg / Nm
  = kg / m^3 */

  float dyn_viscosity = 1.85 / 100000;
  // [dyn_viscosity] = Pa * s

  while(id < pneumatic_tube_database.size())
  {
  float velocity = my_requirements.get_output_speed() *
    pow(cylinder_bore_diameter/2 , 2) /
    pow(pneumatic_tube_database[id].get_inner_diameter()/2 , 2);
      /* [velocity] = mm/s * mm^2 / mm^2;
      = mm/s */
    reynolds_number = density * velocity *
    pneumatic_tube_database[id].get_inner_diameter() / dyn_viscosity
    / 1000000;
    // cout << reynolds_number << endl;

    /* [reynolds_number] = kg /m^3 * mm/s * mm / (Pa * s)
    = kg /m^3 * mm/s * mm / (N / m^2 * s)
    = kg /m * mm / s * mm / (kg * m / s^2 * s)
    = mm / m * mm / (m)
    = 10^-6 */

    // Compare Re to Re_critical
    if (reynolds_number > reynolds_number_crit)
      pneumatic_tube_database.erase(pneumatic_tube_database.begin()+id);
    else
      id++;
  }
}

void pneumatic_tube_selector::select_smallest_lightest_tube()
{
  unsigned int id_vol = 99;
  unsigned int id_mass = 99;
  float low_vol = 100000000000; // [cm^3]
  float low_mass = 100000000000; // [g]

  for (unsigned int i = 0; i <  pneumatic_tube_database.size(); i++)
  {
    if (pneumatic_tube_database[i].get_mass_per_length() < low_mass)
    {
      low_mass = pneumatic_tube_database[i].get_mass_per_length();
      id_mass = i;
    }
    if (pneumatic_tube_database[i].get_outer_diameter() < low_vol)
    {
      low_vol = pneumatic_tube_database[i].get_outer_diameter();
      id_vol = i;
    }
  }

  // Check if id_mass == id_vol
  if (id_mass == id_vol)
    selected_pneumatic_tube = pneumatic_tube_database[id_mass];
  else
  {
    int selection = 0;
    cout << "Lightest and smallest tube are not the same. What should I choose?"
      << endl;
    cout << " [1] Lighter tube" << endl;
    cout << " [2] Smaller tube" << endl;
    do {
      cin >> selection;
    } while((selection != 1)&&(selection != 2));

    if (selection == 1)
      selected_pneumatic_tube = pneumatic_tube_database[id_mass];
    if (selection == 2)
      selected_pneumatic_tube = pneumatic_tube_database[id_vol];

  }
}
