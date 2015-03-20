#include "textview.h"
#include <iostream>
#include <string>

//Store a reference to the map being observed and the attach itself to it as an observer
TextView::TextView(Map & newMap){
  m = & newMap;
  m->attach(*this);
}

TextView::~TextView(){
}

//update simply outputs the map's string representation
void TextView::update(){
  inform(m->to_string());
}

//output something normally
void TextView::inform(std::string msg){
  std::cout << msg << std::endl;
}

//output something with formatting to indicate to the user that they should input something
void TextView::prompt(std::string msg){
  std::cout << "--> "  << msg << std::endl;
}