#include "requirements.h"
#include "pneumatic_tube.h"
#include "file_handling.h"
#include <vector>

#ifndef PNEUMATIC_TUBE_SELECTOR
#define PNEUMATIC_TUBE_SELECTOR

class pneumatic_tube_selector
{
private:

  requirements my_requirements;
  vector<pneumatic_tube> pneumatic_tube_database;
  pneumatic_tube selected_pneumatic_tube;
  vector<vector<float>> temp_dataset;
  file_handling database;

  float working_pressure; // [bar]
  float cylinder_bore_diameter; // [mm]

  void read_in_database();
  void exclude_too_small_tubes();
  void select_smallest_lightest_tube();

public:

  pneumatic_tube select_pneumatic_tube(const requirements& _my_requirements,
    const float& _working_pressure, const float& _cylinder_bore_diameter);

};

#endif
