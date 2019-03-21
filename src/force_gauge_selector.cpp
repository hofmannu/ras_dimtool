/*
  file    force_gauge_selector.h
  author  Urs Hofmann
  date    27. Dez 2017
  mail    urshofmann@gmx.net
*/

#include "force_gauge_selector.h"

void force_gauge_selector::set_req_force_level(const float& _reqForceLevel)
{
  reqForceLevel = _reqForceLevel;
}

void force_gauge_selector::read_in_and_sort_database()
{
  vector<vector<float>> ForceGaugeData =
    myFile.read_file("databases/force_gauges.dat");

    ForceGaugeDatabase.clear();

    unsigned int sizeDatabase = ForceGaugeData.size();

    for(unsigned int iGauge = 0; iGauge < sizeDatabase; iGauge++)
    {
      forceGaugeTemp.set_max_force(ForceGaugeData[iGauge][0]);
      forceGaugeTemp.set_length(ForceGaugeData[iGauge][1]);
      forceGaugeTemp.set_mass(ForceGaugeData[iGauge][2]);

      ForceGaugeDatabase.push_back(forceGaugeTemp);
    }
}

// Exclude force gauges which cannot measure sufficient force

void force_gauge_selector::exclude_weak_gauges()
{
  unsigned int iGauge = 0;

  while(iGauge < ForceGaugeDatabase.size())
  {
    if (ForceGaugeDatabase[iGauge].get_max_force() < reqForceLevel)
    {
      ForceGaugeDatabase.erase(ForceGaugeDatabase.begin()+iGauge);
    }
    else
    {
      iGauge++;
    }
  }

  // Give warning if no gauge can measure the force
  if (ForceGaugeDatabase.size() == 0)
    cout << "[force_gauge_selector] W: No force gauge can measure the force" <<
      endl;

}

force_gauge force_gauge_selector::select_force_gauge()
{

  read_in_and_sort_database();

  exclude_weak_gauges();

  unsigned int idLMass = 0;

  for (unsigned int iGauge = 0; iGauge < ForceGaugeDatabase.size(); iGauge++)
  {
    if (ForceGaugeDatabase[iGauge].get_mass() <
      ForceGaugeDatabase[idLMass].get_mass())
    {
      idLMass = iGauge;
    }
  }

  return ForceGaugeDatabase[idLMass];
}
