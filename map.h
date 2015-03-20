#pragma once

#include <string>
#include <list>
#include <cstdlib>
#include "country.h"
#include "continent.h"
#include "observable.h"
#include "game_utils.h"
#include "MapConfig.h"

class MapView;

class Map: public Observable{
public:
  Map(std::string mapFilePath);
  ~Map();
       
  //MAP LOADING RELATED FUNCTIONS---------------------------------------
  void setupCountryOwners(int numOfPlayers);
  void setupHardcodedMap();
  
  //MAIN GAME USED FUNCATIONS-------------------------------------------
  int countCountriesOwned(int playerIndex);
  int computeContinentsBonuses(int playerIndex);
  std::list<std::string> getEnemyNeighbours(std::string &countryName, int playerIndex);
  std::list<std::string> getConnectedFriendlyCountries(std::string &startCountry, int playerIndex);
  
//COUNTRY ACCESSORS & MUTATORS---------------------------------------------------
  int setCountryArmies(std::string countryName, int numOfArmies, bool updateMap = true);
  int setCountryOwnerIndex(std::string countryName, int playerIndex, bool updateMap = true);
  int getCountryArmies(std::string countryName);
  int getCountryOwnerIndex(std::string countryName);
  bool countryExists(std::string countryName);
  
//IMPLEMENTING OBSERVABLE FUNCTIONS-----------------------
  void attach(Observer & ob);
  void detach(Observer & ob);
  void notify();
  
//INPUT AND OUTPUT OF THE MAP-------------------------------
  std::string to_string();
  void saveMap(std::string filePath);

  
  
private:
  //getConnectedFriendlyCountries HELPER FUNCTIONS---------------------------------------------------
  void recursiveGetConnectedFriendlyCountries(Country &startCountry, int , std::list<std::string> &connectedCountryList);
  
  //MAP SETUP HELPER FUNCTIONS------------------------------------------
  void addContinent(Continent * newContinent);
  void addCountry(Country * newCountry);
  Continent& getContinent(std::string continentName);
  Country& getCountry(std::string countryName);
  
  //MAP SETUP FUNCTIONS-------------------------------------------------
  void loadMap(std::string filePath);
  
  int countryCount;
  int continentCount;
  Country ** countries;
  Continent ** continents;
  std::list<Observer*> * views;
  MapConfig mapIO;
    
};

std::ostream& operator<<(std::ostream& os, Map & outputMap);