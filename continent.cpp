#include "continent.h"
#include <iostream>
#include <sstream>

Continent::Continent(std::string newName){
  name = newName;
  countryCount = 0;
  controlValue = 1;
}

Continent::Continent(void)
{
}


void Continent::addCountry(Country & newCountry){
  countries = insertElement(countries, countryCount, newCountry);
  
  std::cout << newCountry.getName() << " added to " << name << std::endl;
}

Country& Continent::getCountry(std::string countryName){
  return findElement(countries, countryCount, countryName);
}

bool Continent::isOwnedByPlayer(int playerIndex){
  for(int x = 0; x < countryCount; x++){
    if (countries[x]->getOwnerIndex() != playerIndex){
      return false;
    }
  }
  return true;
}

//ACCESSORS-------------------------------------
int Continent::getControlValue(){return controlValue;}
Country ** Continent::getCountries(){return countries;}
int Continent::getCountryCount(){return countryCount;}
std::string Continent::getName(){return name;}
  
//MUTATORS-------------------------------------
void Continent::setControlValue(int newControlValue){controlValue = newControlValue;}

void Continent::setName(std::string cContName)
{
	name = cContName;
}


void Continent::setScore(int cScore)
{
	score = cScore;
}

int Continent::getScore()
{
	return score;
}

std::string Continent::to_string(){
  std::ostringstream oss;
  using std::endl;
  oss << getName() << ":" << endl;
  for (int y = 0; y < getCountryCount(); y ++){
    oss << "  " << getCountries()[y]->to_string() << endl;
  }
  return oss.str();
}
