/*
  * File:     rack_selector.h
  * Author:   Urs Hofmann
  * Date:     30. Okt 2017
  * Version:  1.0
  * Mail:     urshofmann@gmx.net, hofmannu@student.ethz.ch
  * Phone:    +49 1573 0711 493
  *
  * This class select a rack out of a database as a function of stroke and force.
*/

#include "rack.h"
#include "file_handling.h"

#ifndef RACK_SELECTOR_H
#define RACK_SELECTOR_H

class rack_selector
{
private:
  float length;
  float force;

  rack selected_rack;

public:
  rack select_rack(const float& _length, const float& _force);
};


#endif
