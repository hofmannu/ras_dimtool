/*
	file 		bearing_selector.cpp
	author	Urs Hofmann
	date 		27. Okt 2017
	mail 		urshofmann@gmx.net
*/

#include "bearing_selector.h"

bearing bearing_selector::select_bearing(const float& _F_rope, const float&
  _F_gear, const float& _d_r1_rope, const float& _d_rope_gear, const float&
  _d_gear_r2)
{

  // Read in bearing database
  read_database();

  // Variables containing the true distances including the thickness of the bear
  // ings


  // Calculate radial forces for each bearing
  // Assumption at this point: upper and lower bearing are the same
  for (unsigned int i = 0; i < database_bearing.size(); i++)
  {
    // Calculate true distances
    d_r1_rope = _d_r1_rope + database_bearing[i].get_height() / 2;
    d_rope_gear = _d_rope_gear;
    d_gear_r2 = _d_gear_r2 + database_bearing[i].get_height() / 2;

    // Define forces acting on bearing
    F_r_1.clear();
    F_r_2.clear();
    int id = 0;

    for (int i = -1; i < 2; i = i + 2)
    {
      F_r_2.push_back(
        ( d_r1_rope * _F_rope + (d_r1_rope + d_rope_gear) * i * _F_gear)
        / (d_r1_rope + d_rope_gear + d_gear_r2));
      F_r_1.push_back(_F_rope + _F_gear * i - F_r_2[id]);
      id++;
    }

    // Get maximum radial force
    F_r_max = 0;
    for (unsigned int i = 0; i < F_r_1.size(); i++)
    {
      if(abs(F_r_1[i]) > F_r_max)
        F_r_max = abs(F_r_1[i]);

      if(abs(F_r_2[i]) > F_r_max)
        F_r_max = abs(F_r_2[i]);
    }

    // Berechnung statisch äquivalente Lagerbelastung
    P_0 = X_0 * F_r_max + Y_0 * F_a_max;

    // Berechnung statische Tragzahl
    C_0 = f_s * P_0;

    // Berechnung zeitlich äquivalente Belastung
    P = X * F_r_max + Y * F_a_max;

    // Berechning zetilich äquivalente Lagerbelastung
    C = f_s * P;

    //cout << "Zeitlich äquivalente Lagerbelastung: " << C << " N " << endl;

    database_bearing[i].set_C(C);
    database_bearing[i].set_C_0(C_0);
  }

  // Exclude bearings which are not stable enough
  exclude_weak_bearings();

  // Search for lightest and smallest version
  select_smallest_and_lightest_bearing();

  return selected_bearing;
}

// Read in database from file and sort it into vector of bearings

void bearing_selector::read_database()
{
  // Clear database vector
  database_raw_data.clear();
  database_bearing.clear();

  // Read in database file
  database_raw_data = database_file.read_file("databases/ball_bearings.dat");
  bearing temp_bearing;

  // Translate it into internal structure
  for (unsigned int i = 0; i < database_raw_data.size(); i++)
  {
    temp_bearing.set_inner_radius(database_raw_data[i][0]/2);
    temp_bearing.set_outer_radius(database_raw_data[i][1]/2);
    temp_bearing.set_height(database_raw_data[i][2]);
    temp_bearing.set_max_dyn_load(database_raw_data[i][3]);
    temp_bearing.set_max_static_load(database_raw_data[i][4]);
    temp_bearing.set_mass(database_raw_data[i][8]);

    database_bearing.push_back(temp_bearing);
  }
}

// Functions deletes entries in database not providing suffient force

void bearing_selector::exclude_weak_bearings()
{
  unsigned int iBearing = 0;

  while(iBearing < database_bearing.size())
  {
    if (database_bearing[iBearing].get_C() >
      database_bearing[iBearing].get_max_static_load())
    {
      database_bearing.erase(database_bearing.begin()+iBearing);
      // cout << "Excluded" << endl;
    }
    else if (database_bearing[iBearing].get_C_0() >
      database_bearing[iBearing].get_max_dyn_load())
    {
      database_bearing.erase(database_bearing.begin()+iBearing);
      // cout << "Excluded" << endl;
    }
    else
      iBearing++;
  }

  if (database_bearing.size() == 0)
    cout <<"E: No bearing providing sufficient stability was found in database."
      << endl;

}

void bearing_selector::select_smallest_and_lightest_bearing()
{
  float l_mass= 10000000; // [cm^3]
  float l_vol = 10000000; // [g]
  unsigned int l_vol_id = 0;
  unsigned int l_mass_id = 0;

  // Search for lightest and smallest bearing
  for(unsigned int iBearing = 0; iBearing < database_bearing.size(); iBearing++)
  {
    if (l_mass > database_bearing[iBearing].get_mass())
    {
      l_mass = database_bearing[iBearing].get_mass();
      l_mass_id = iBearing;
    }
    if (l_vol > database_bearing[iBearing].get_volume())
    {
      l_vol = database_bearing[iBearing].get_volume();
      l_vol_id = iBearing;
    }

    // If lowest mass version is not the same as lowest volume version
    if (l_mass_id =! l_vol_id)
    {
      unsigned int selection = 0;
      if(doInteractive)
      {
        do{
          cout << "W: Lightest and smallest bearings are not the same. Which part"
          "would you like to optimize?" << endl;
          cout << " [0] Lowest mass" << endl;
          cout << " [1] Lowest volume" << endl;
          cin >> selection;
        }while ((selection != 1) && (selection != 0));

        if (selection == 1)
          selected_bearing = database_bearing[l_mass_id];

        if (selection == 0)
          selected_bearing = database_bearing[l_vol_id];
      }
      else
        selected_bearing = database_bearing[l_mass_id];
    }

  }
}

void bearing_selector::set_do_interactive(const bool& _doInteractive)
{
  doInteractive = _doInteractive;
}
