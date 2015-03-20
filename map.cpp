#include "map.h"

#include <iostream>
#include <sstream>
#include <time.h>

Map::Map(std::string mapFilePath){
  countryCount = 0;
  continentCount = 0;
  views = new std::list<Observer*>();
  loadMap(mapFilePath);
}

Map::~Map(){
  
  //Destruct all the countries
  for(int x = 0; x < countryCount; x++){
    delete countries[x];
  }
  delete [] countries;
  countries = NULL;
  
  //Destruct all the continents
  for(int x = 0; x < continentCount; x++){
    delete continents[x];
  }
  delete [] continents;
  continents = NULL;
  
}

//add a continent to a big array of continents
void Map::addContinent(Continent * newContinent){
  continents = insertElement(continents, continentCount, *newContinent);
  
  std::cout << newContinent->getName() << " added to the map" << std::endl;
}

//Simply add countries to a big array of countries
//It is expected that they will be connected later using "addNeighbour" functions
void Map::addCountry(Country * newCountry){
  countries = insertElement(countries, countryCount, *newCountry);
  
  std::cout << newCountry->getName() << " added to the map." << std::endl;
}

//retrieve a continent based on its name alone
Continent& Map::getContinent(std::string continentName){
  return findElement(continents, continentCount, continentName);
}

//Returns the number of countries owned by the given player
int Map::countCountriesOwned(int playerIndex){
  int countriesOwnedCount = 0;
  for (int x = 0; x < countryCount; x ++){
    if ( countries[x]->getOwnerIndex() == playerIndex){
      countriesOwnedCount++;
    }
  }
  return countriesOwnedCount;
}

/*
 *********This is not a map specific function and should be considered for refactoring
 */
int Map::computeContinentsBonuses(int playerIndex){
  int continentBonuses = 0;
  for (int x = 0; x < continentCount; x ++){
    if(continents[x]->isOwnedByPlayer(playerIndex) ){
      continentBonuses += continents[x]->getControlValue();
    }
  }
  return continentBonuses;
}

//Assigns each player to own an equal number of randomly selected countries
void Map::setupCountryOwners(int numOfPlayers)
{
	int playerIndex = 0;
	int countryIndex = 0;
	
	srand(time(NULL));   //Allows to generate unique numbers
	
	for(int i = 0;i< countryCount;i++){
		//randomly assign a country to the player
		playerIndex = i % numOfPlayers;
		countryIndex = rand() % (countryCount);
		
		//while the country assigned to the player is already taken, assign another
		while(countries[countryIndex]->getOwnerIndex() != -1){
		  countryIndex = rand() % countryCount;
		}
		countries[countryIndex]->setOwnerIndex(playerIndex);
	}
}


//retrieve a country based on its name alone
Country& Map::getCountry(std::string countryName){
  return findElement(countries, countryCount, countryName);   
}

//IMPLEMENTING OBSERVABLE-----------------------
void Map::attach(Observer & ob){
  //push an observer to the list
  views->push_front(&ob);
}

void Map::detach(Observer & ob){
  bool found = false;
  std::list<Observer*>::iterator findIter = views->begin();
  
  //go through the list of all the observers looking for the given one
  do{
      if (*findIter == &ob){found = true;}
  }while(findIter++ != views->end() && found == false);
  
  //if the given observer is found, remove it from the list
  if(found){
    views->erase(findIter);
  } 
}

void Map::notify(){
  std::list<Observer *>::iterator i = views->begin();

  //go through the list of all attached observers and notify them
  for (; i != views->end(); ++i){
    (*i)->update();
  }
}

//get a list of all neighbours of a country who do not belong to the owner of that country
std::list<std::string> Map::getEnemyNeighbours(std::string &countryName, int playerIndex){
  std::list<std::string> enemyNeighbourList;
  
  Country attackingCountry = findElement(countries, countryCount, countryName);
  
  if(&attackingCountry != NULL){
    //for every neighbouring country, check if the owner is the same as the given owner
    for(int x = 0; x < attackingCountry.getNeighbourCount(); x++){
      //if the owners are not the same, add that neighbour to the list of enemy countries
      if(attackingCountry.getAllNeighbours()[x]->getOwnerIndex() != attackingCountry.getOwnerIndex()){
	enemyNeighbourList.push_front(attackingCountry.getAllNeighbours()[x]->getName());
      } 
    }
  }
  return enemyNeighbourList;
}

//COUNTRY CONNECTION FUNCTIONS---------------------------------------------------
//recursively searches for any friendly neighbouring countries (and any of their friendly neightbours) and add them to the list parameter
void Map::recursiveGetConnectedFriendlyCountries(Country &startCountry, int playerIndex, std::list<std::string> &connectedCountryList){
  //the the current country to the list
  connectedCountryList.push_front(startCountry.getName());
  //for every neighbour of the country
  for (int x = 0; x < startCountry.getNeighbourCount(); x++){
    //if that neighbour is friendly and not already in the list call the function on them
    if (startCountry.getAllNeighbours()[x]->getOwnerIndex() == playerIndex 
      && !listContains(connectedCountryList, startCountry.getAllNeighbours()[x]->getName())){
      recursiveGetConnectedFriendlyCountries(*startCountry.getAllNeighbours()[x], playerIndex, connectedCountryList);
    }
  }  
}

//recursively find all friendly countries connected to the given country
std::list<std::string> Map::getConnectedFriendlyCountries(std::string &startCountry, int playerIndex){
  std::list<std::string> connectedCountryList;
  
  Country startCountryObject = findElement(countries, countryCount, startCountry);
  
  //simply call the private recursive country search algorithm
  if (&startCountryObject != NULL){  
    recursiveGetConnectedFriendlyCountries(startCountryObject, playerIndex, connectedCountryList);
  }
  
  //return everything found by the recursive search function
  return connectedCountryList;
  
}




//COUNTRY ACCESSORS & MUTATORS---------------------------------------------------
int Map::setCountryArmies(std::string countryName, int numOfArmies, bool updateMap){
  //if the country exists
  if (countryExists(countryName)){
    //get the actual country object and update it
    findElement(countries, countryCount, countryName).setArmies(numOfArmies);
    //if the action doesn't have view updates disabled
    if (updateMap){
      //update all relevant observers of the change to the model
      notify();
    }
    return 1;
  }
  return -1;
}

int Map::setCountryOwnerIndex(std::string countryName, int playerIndex, bool updateMap){
  //if the country actually exists
  if (countryExists(countryName)){
    //update the country object's owner
    findElement(countries, countryCount, countryName).setOwnerIndex(playerIndex);
    //if the action doesn't have view updates disabled
    if (updateMap){
      //update all relevant observers of the change to the model
      notify();
    }
    return 1;
  }
  return -1; 
}

//simple accessor.  Finds the country and returns its number of armies
int Map::getCountryArmies(std::string countryName){
 if (countryExists(countryName)){
    return findElement(countries, countryCount, countryName).getArmies();
  }
  return -1;  
}

//simple accessor.  Finds the country and returns its owner
int Map::getCountryOwnerIndex(std::string countryName){
  if (countryExists(countryName)){
    return findElement(countries, countryCount, countryName).getOwnerIndex();
  }
  return -1;  
}

//simple accessor, returns true is the country exists on the map
bool Map::countryExists(std::string countryName){return &(findElement(countries, countryCount, countryName)) != NULL;}
  
  //returns a string representation of the map
std::string Map::to_string(){
  std::ostringstream oss;
  for (int x = 0; x < continentCount; x++){
    oss << continents[x]->to_string();
  }
  return oss.str();
} 


void Map::loadMap(std::string mapFilePath){
  
  
  mapIO.openInputFile(mapFilePath);
  mapIO.readMapFromFile();
  mapIO.closeInputFile();
  
  //add the continents to the map
  std::list<Continent> continents = mapIO.getContinents();
  
  for (std::list<Continent>::iterator it = continents.begin(); it != continents.end(); ++it)
  {
    addContinent(new Continent((*it).getName()));
  }
  
  //add the countries to the map and register them with their continents
  std::list<Territory> territories = mapIO.getTerritories();
  
  for (std::list<Territory>::iterator it = territories.begin(); it != territories.end(); ++it)
  {
    addCountry(new Country((*it).getName(), getContinent((*it).getParentCont())));
  }
  
  //connect the countries
  for (std::list<Territory>::iterator it = territories.begin(); it != territories.end(); ++it)
  {
    Territory tempTerr = *it;
    std::list<string> terr = tempTerr.getAdjacent();
    
    for (std::list<string>::iterator itStr = terr.begin(); itStr != terr.end(); ++itStr)
    {
      getCountry((*it).getName()).addNeighbour(getCountry(*itStr));
    }  
  } 
}

void Map::saveMap(std::string mapFilePath){
  
  mapIO.openOutputFile(mapFilePath);
  mapIO.saveMapToFile();
  mapIO.closeOutputFile();
}

//Sets up a map (hard coded for now until file loading can be implemented
void Map::setupHardcodedMap(){
  //add the continents to the map
  addContinent(new Continent("North America"));
  addContinent(new Continent("South America"));
  addContinent(new Continent("Europe"));
  addContinent(new Continent("Africa"));
  addContinent(new Continent("Asia"));
  addContinent(new Continent("Australia"));
  
  //add the countries to the map and register them with their continents
  addCountry(new Country("Alaska", getContinent("North America")));
  addCountry(new Country("Alberta", getContinent("North America")));
  addCountry(new Country("Ontario", getContinent("North America")));
  addCountry(new Country("Quebec", getContinent("North America")));
  addCountry(new Country("Greenland", getContinent("North America")));
  addCountry(new Country("Northwest Territory", getContinent("North America")));
  addCountry(new Country("Western United States", getContinent("North America")));
  addCountry(new Country("Eastern United States", getContinent("North America")));
  addCountry(new Country("Central America", getContinent("North America")));
  
  addCountry(new Country("Argentina", getContinent("South America")));
  addCountry(new Country("Brazil", getContinent("South America")));
  addCountry(new Country("Peru", getContinent("South America")));
  addCountry(new Country("Venezuela", getContinent("South America")));
  
  addCountry(new Country("Great Britain", getContinent("Europe")));
  addCountry(new Country("Iceland", getContinent("Europe")));
  addCountry(new Country("Northern Europe", getContinent("Europe")));
  addCountry(new Country("Scandinavia", getContinent("Europe")));
  addCountry(new Country("Southern Europe", getContinent("Europe")));
  addCountry(new Country("Ukraine", getContinent("Europe")));
  addCountry(new Country("Western Europe", getContinent("Europe")));
  
  addCountry(new Country("Congo", getContinent("Africa")));
  addCountry(new Country("East Africa", getContinent("Africa")));
  addCountry(new Country("Egypt", getContinent("Africa")));
  addCountry(new Country("Madagascar", getContinent("Africa")));
  addCountry(new Country("North Africa", getContinent("Africa")));
  addCountry(new Country("South Africa", getContinent("Africa")));
  
  addCountry(new Country("Afghanistan", getContinent("Asia")));
  addCountry(new Country("China", getContinent("Asia")));
  addCountry(new Country("India", getContinent("Asia")));
  addCountry(new Country("Irkutsk", getContinent("Asia")));
  addCountry(new Country("Japan", getContinent("Asia")));
  addCountry(new Country("Kamchatka", getContinent("Asia")));
  addCountry(new Country("Middle East", getContinent("Asia")));
  addCountry(new Country("Mongolia", getContinent("Asia")));
  addCountry(new Country("Siam", getContinent("Asia")));
  addCountry(new Country("Siberia", getContinent("Asia")));
  addCountry(new Country("Ural", getContinent("Asia")));
  addCountry(new Country("Yakutsk", getContinent("Asia")));
  
  addCountry(new Country("Eastern Australia", getContinent("Australia")));
  addCountry(new Country("Indonesia", getContinent("Australia")));
  addCountry(new Country("New Guinea", getContinent("Australia")));
  addCountry(new Country("Western Australia", getContinent("Australia")));
  
  //connect the countries
  getCountry("Argentina").addNeighbour(getCountry("Brazil"));
  getCountry("Argentina").addNeighbour(getCountry("Peru"));
  getCountry("Peru").addNeighbour(getCountry("Brazil"));
  getCountry("Peru").addNeighbour(getCountry("Venezuela"));
  getCountry("Brazil").addNeighbour(getCountry("Venezuela"));
  getCountry("Brazil").addNeighbour(getCountry("North Africa"));
  getCountry("Venezuela").addNeighbour(getCountry("Central America"));
  getCountry("Central America").addNeighbour(getCountry("Eastern United States"));
  getCountry("Central America").addNeighbour(getCountry("Western United States"));
  getCountry("Western United States").addNeighbour(getCountry("Eastern United States"));
  getCountry("Western United States").addNeighbour(getCountry("Ontario"));
  getCountry("Western United States").addNeighbour(getCountry("Alberta"));
  getCountry("Eastern United States").addNeighbour(getCountry("Ontario"));
  getCountry("Eastern United States").addNeighbour(getCountry("Quebec"));
  getCountry("Alberta").addNeighbour(getCountry("Ontario"));
  getCountry("Alberta").addNeighbour(getCountry("Alaska"));
  getCountry("Alberta").addNeighbour(getCountry("Northwest Territory"));
  getCountry("Ontario").addNeighbour(getCountry("Quebec"));
  getCountry("Ontario").addNeighbour(getCountry("Northwest Territory"));
  getCountry("Ontario").addNeighbour(getCountry("Greenland"));
  getCountry("Quebec").addNeighbour(getCountry("Greenland"));
  getCountry("Northwest Territory").addNeighbour(getCountry("Greenland"));
  getCountry("Northwest Territory").addNeighbour(getCountry("Alaska"));
  getCountry("Greenland").addNeighbour(getCountry("Iceland"));
  getCountry("Alaska").addNeighbour(getCountry("Kamchatka"));
  getCountry("Iceland").addNeighbour(getCountry("Scandinavia"));
  getCountry("Iceland").addNeighbour(getCountry("Great Britain"));
  getCountry("Great Britain").addNeighbour(getCountry("Scandinavia"));
  getCountry("Great Britain").addNeighbour(getCountry("Northern Europe"));
  getCountry("Great Britain").addNeighbour(getCountry("Western Europe"));
  getCountry("Scandinavia").addNeighbour(getCountry("Northern Europe"));
  getCountry("Scandinavia").addNeighbour(getCountry("Ukraine"));
  getCountry("Western Europe").addNeighbour(getCountry("North Africa"));
  getCountry("Western Europe").addNeighbour(getCountry("Northern Europe"));
  getCountry("Western Europe").addNeighbour(getCountry("Southern Europe"));
  getCountry("Northern Europe").addNeighbour(getCountry("Southern Europe"));
  getCountry("Northern Europe").addNeighbour(getCountry("Ukraine"));
  getCountry("Southern Europe").addNeighbour(getCountry("Egypt"));
  getCountry("Southern Europe").addNeighbour(getCountry("North Africa"));
  getCountry("Southern Europe").addNeighbour(getCountry("Middle East"));
  getCountry("Ukraine").addNeighbour(getCountry("Ural"));
  getCountry("Ukraine").addNeighbour(getCountry("Afghanistan"));
  getCountry("Ukraine").addNeighbour(getCountry("Middle East"));
  getCountry("North Africa").addNeighbour(getCountry("Egypt"));
  getCountry("North Africa").addNeighbour(getCountry("East Africa"));
  getCountry("North Africa").addNeighbour(getCountry("Congo"));
  getCountry("Egypt").addNeighbour(getCountry("East Africa"));
  getCountry("East Africa").addNeighbour(getCountry("Congo"));
  getCountry("East Africa").addNeighbour(getCountry("Madagascar"));
  getCountry("East Africa").addNeighbour(getCountry("South Africa"));
  getCountry("Congo").addNeighbour(getCountry("South Africa"));
  getCountry("South Africa").addNeighbour(getCountry("Madagascar"));  
  getCountry("Middle East").addNeighbour(getCountry("Afghanistan"));
  getCountry("Middle East").addNeighbour(getCountry("India"));
  getCountry("Afghanistan").addNeighbour(getCountry("India"));
  getCountry("Afghanistan").addNeighbour(getCountry("China"));
  getCountry("Afghanistan").addNeighbour(getCountry("Ural"));
  getCountry("Ural").addNeighbour(getCountry("Siberia"));
  getCountry("Ural").addNeighbour(getCountry("China"));
  getCountry("Siberia").addNeighbour(getCountry("China"));
  getCountry("Siberia").addNeighbour(getCountry("Yakutsk"));
  getCountry("Siberia").addNeighbour(getCountry("Irkutsk"));
  getCountry("Siberia").addNeighbour(getCountry("Mongolia"));
  getCountry("Yakutsk").addNeighbour(getCountry("Irkutsk"));
  getCountry("Yakutsk").addNeighbour(getCountry("Kamchatka"));
  getCountry("Irkutsk").addNeighbour(getCountry("Mongolia"));
  getCountry("Irkutsk").addNeighbour(getCountry("Kamchatka"));
  getCountry("Kamchatka").addNeighbour(getCountry("Japan"));
  getCountry("Mongolia").addNeighbour(getCountry("Japan"));
  getCountry("Mongolia").addNeighbour(getCountry("China"));
  getCountry("China").addNeighbour(getCountry("India"));
  getCountry("China").addNeighbour(getCountry("Siam"));
  getCountry("India").addNeighbour(getCountry("Siam"));
  getCountry("Siam").addNeighbour(getCountry("Indonesia"));
  getCountry("Indonesia").addNeighbour(getCountry("New Guinea"));
  getCountry("Indonesia").addNeighbour(getCountry("Western Australia"));
  getCountry("Western Australia").addNeighbour(getCountry("Eastern Australia"));
  getCountry("Western Australia").addNeighbour(getCountry("New Guinea"));
  getCountry("Eastern Australia").addNeighbour(getCountry("New Guinea"));
}