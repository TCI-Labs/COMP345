#pragma once

#include "country.h"
#include <string>
#include <sstream>
class Country; //forward declaration needed due to circular dependancy between Continent and Country;

class Continent{
public:
	Continent(void);
  Continent(std::string name);
  void addCountry(Country& newCountry);
  Country& getCountry(std::string countryName);
  bool isOwnedByPlayer(int playerIndex);
  
//ACCESSORS-------------------------------------  
  int getControlValue();
  Country ** getCountries();
  int getCountryCount();
  std::string getName();

 
	void setName(std::string cContName);
	void setScore(int score);
	int getScore();
	
  std::string to_string();
  
//MUTATORS-------------------------------------
  void setControlValue(int newControlValue);
  
private:
  int countryCount;
  Country ** countries;
  std::string name;
  int controlValue;

  //andrey's stuff
  std::string continentName;
	int score;
    
};

std::ostream& operator<<(std::ostream& os, Continent & outputContinent);