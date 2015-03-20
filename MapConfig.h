#include<string>
#include <iostream>
#include <fstream>
#include <list>
#include "Territory.h"
#include "continent.h"
using namespace std;

#pragma once

class MapConfig
{
public:
	MapConfig(void);
	~MapConfig(void);

	void openInputFile(string path);
	void closeInputFile();

	void openOutputFile(string path);
	void closeOutputFile();

	void readMapFromFile();
	void saveMapToFile();

	void setAuthor(string author);
	void setImage(string image);
	void setWrap(string wrap);
	void setScroll(string scroll);
	void setWarn(string warn);
	void setContinents(std::list<Continent> cContinents);
	void setTerritories(list<Territory> sTerritories);
	void displayMapFileContent();

	string getAuthor();
	string getImage();
	string getWrap();
	string getScroll();
	string getWarn();
	list<Continent> getContinents();
	list<Territory> getTerritories();
	//list<Territory> MapConfig::getTerritoriesArray();
	bool isConnectedGraph();
	bool areContinentsSubGraphs();
	bool isOneCountyPerContinent();

private:
	string author;
	string image;
	string wrap;
	string scroll;
	string warn;
	std::list<Continent> continents;
	list<Territory> territories;
	std::ifstream inStream;
	std::ofstream outStream;
};

