#include "country.h"
#include <sstream>
#include <iostream>


Country::Country(std::string cName, Continent & setContinent){
  neighbourCount = 0;
  name = cName;
  myContinent = &setContinent;
  myContinent->addCountry(*this);
  ownerIndex = -1;
  armies = 2;
}

/*
 * Adds countries to an array which represents all the countries connected to this country
 * 
 * By default, it will make a two-way link between countries unless "unidirectional" is set to true
 */
void Country::addNeighbour(Country & newNeighbour, bool unidirectional){
  addNeighbourLink(newNeighbour);
  
  if (!unidirectional){
    newNeighbour.addNeighbourLink(*this);
  }
}

//ACCESSORS------------------------------------
int Country::getArmies(){return armies;}
Country ** Country::getAllNeighbours(){return neighbours;}
int Country::getNeighbourCount(){return neighbourCount;}
std::string Country::getName(){return name;}
int Country::getOwnerIndex(){return ownerIndex;}

Country * Country::getNeighbour(std::string countryName){
  return & (findElement(neighbours, neighbourCount, countryName));
}

bool Country::isNeighbour(std::string countryName){
  return &(findElement(neighbours, neighbourCount, countryName)) != NULL;
}

//MUTATORS------------------------------------

void Country::setOwnerIndex(int newOwnerIndex){ownerIndex = newOwnerIndex;}
void Country::setArmies(int newArmies){armies = newArmies;}

//PRIVATE FUNCTIONS---------------------------
/*
 * Creates a link between two countries, by adding "newNeighbour" to a country's "neighbours" array
 * This function is used to enable one class to call another to get the other to add the first one to itself.
 * 
 * Post: newNeighbour is added to neighbours in alphabetical order by name.
 */
void Country::addNeighbourLink(Country &newNeighbour){
  this->neighbours = insertElement(this->neighbours, this->neighbourCount, newNeighbour);
  std::cout << newNeighbour.getName() << " connected to " << this->getName() << std::endl;
}

std::string Country::to_string(){
  std::ostringstream oss;
  oss << name << " : player "  << ownerIndex <<" : " <<  armies << " armies" << " : Connected to " ;
  if(neighbourCount == 0){
  oss << "no one.";  
  }
  else{
    for (int x = 0; x < neighbourCount; x++){
      oss << neighbours[x]->getName();
      if (x < neighbourCount - 1){
	oss << ", ";
      }
      
    }
  }
  return oss.str();
}