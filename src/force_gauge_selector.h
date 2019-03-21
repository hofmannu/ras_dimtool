/*
  file    force_gauge_selector.h
  author  Urs Hofmann
  date    27. Dez 2017
  mail    urshofmann@gmx.net
*/

#include "force_gauge.h"
#include "file_handling.h"
#include <vector>

#ifndef FORCE_GAUGE_SELECTOR_H
#define FORCE_GAUGE_SELECTOR_H

class force_gauge_selector
{
private:
  float reqForceLevel;

  file_handling myFile;

  vector<force_gauge> ForceGaugeDatabase;
  force_gauge selectedForceGauge;
  force_gauge forceGaugeTemp;

  void read_in_and_sort_database();
  void exclude_weak_gauges();
  void select_best_matching_force_gauge();

public:

  void set_req_force_level(const float& _reqForceLevel);
  force_gauge select_force_gauge();

};

#endif
