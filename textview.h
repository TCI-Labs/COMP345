#pragma once
#include "observer.h"
#include "map.h"
#include <sstream>

class Map; //forward declaration

class TextView : public Observer{
public:
  TextView(Map & newMap);
  ~TextView();
  void update();
  void inform(std::string msg);
  void prompt(std::string msg);

private:
  Map * m;
};