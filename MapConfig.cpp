#include "MapConfig.h"
#include "Territory.h"
#include "continent.h"
#include <list>
#include <vector>
#include <sstream>
#include <stdlib.h> 
#include <fstream>
using namespace std;

MapConfig::MapConfig(void)
{

}


MapConfig::~MapConfig(void)
{
}


void MapConfig::openInputFile(string path)
{
	inStream.open (path.c_str());

	if (!inStream.good())
	{
		cout << "FILE OPENING ERROR - file path: " << path << endl;
		cin.get();
		exit(0);
	}
	
}

void MapConfig::closeInputFile()
{
	inStream.close();
}


void MapConfig::openOutputFile(string path)
{
	outStream.open (path.c_str());
	if (!outStream.good())
	{
		cout << "FILE CLOSING ERROR - file path: " << path << endl;
		cin.get();
		exit(0);
	}
}

void MapConfig::closeOutputFile()
{
	outStream.close();
}

void MapConfig::setAuthor(string cAuthor)
{
	author = cAuthor;
}

void MapConfig::setImage(string cImage)
{
	image = cImage;
}

void MapConfig::setWrap(string cWrap)
{
	wrap = cWrap;
}

void MapConfig::setScroll(string cScroll)
{
	scroll = cScroll;
}

void MapConfig::setWarn(string cWarn)
{
	warn = cWarn;
}

void MapConfig::setContinents(std::list<Continent> cContinents)
{
	continents = cContinents;
}

void MapConfig::setTerritories(std::list<Territory> sTerritories)
{
	territories = sTerritories;
}

string MapConfig::getAuthor()
{
	return author;
}

string MapConfig::getImage()
{
	return image;
}

string MapConfig::getWrap()
{
	return wrap;
}

string MapConfig::getScroll()
{
	return scroll;
}

string MapConfig::getWarn()
{
	return warn;
}

list<Continent> MapConfig::getContinents()
{
	return continents;
}

list<Territory> MapConfig::getTerritories()
{
	return territories;
}


void MapConfig::displayMapFileContent()
{
  cout<< "author="<<author<<endl;
  cout<< "image="<<image<<endl;
  cout<< "wrap="<<wrap<<endl;

  cout<<endl;

 for (std::list<Continent>::iterator it = continents.begin(); it != continents.end(); ++it)
  {
	  cout<<(*it).getName()<<"="<<(*it).getScore()<<endl;
  }

  cout<<endl;

  for (std::list<Territory>::iterator it = territories.begin(); it != territories.end(); ++it)
		{
			Territory tempTerr = *it;
			cout << tempTerr.getName() << ", ";
			cout << tempTerr.getX() << ", "<< tempTerr.getY() << ", ";
			cout << tempTerr.getParentCont() << ", ";

			std::list<string> terr = tempTerr.getAdjacent();

			for (std::list<string>::iterator itStr = terr.begin(); itStr != terr.end(); ++itStr)
			{
				cout << *itStr << ", ";
			}
			cout<<endl;
		}
}

//the function to be implemented with an object Map from Part 1 of the assignment
//traversal of the graph will be performed to find out if the map is a connected graph
bool MapConfig::isConnectedGraph()
{
	return true;
}

//the function to be implemented with an object Map from Part 1 of the assignment
//traversal of the graph will be performed to find out if the continents are connected subgraphs
bool MapConfig::areContinentsSubGraphs()
{
	return true;
}

//the function to be implemented with an object Map from Part 1 of the assignment
//traversal of the graph will be performed to find out if a country belongs to exactly one continent
bool MapConfig::isOneCountyPerContinent()
{
	return true;
}



void MapConfig::readMapFromFile()
{
		string skip;
		string read;
		string author;
		string image;
		string wrap;
		string scroll;
		string warn;

		//Sets the end-of-line delimiting character depending on whether the program is being run in linux or not.
		#ifdef linux
			const char END_LINE_CHARACTER = (char)13;
		#else
			const char END_LINE_CHARACTER = '\n';
		#endif

		std::list<Continent> continents;
		string strCont;
		string scoreCont;

		getline(inStream, skip, '\n');

		getline(inStream, skip, '=');//read author line
		getline(inStream, author, '\n');

		setAuthor(author);

		getline(inStream, skip, '=');//read image line
		getline(inStream, image, '\n');

		setImage(image);

		getline(inStream, skip, '=');//read wrap line
		getline(inStream, wrap, '\n');

		setWrap(wrap);

		getline(inStream, skip, '=');//read scroll line
		getline(inStream, scroll, '\n');

		setScroll(scroll);

		getline(inStream, skip, '=');//read warn line
		getline(inStream, warn, '\n');

		setWarn(warn);

		getline(inStream, skip, '\n');//read empty line
		getline(inStream, skip, '\n');//read [continents]

		int index = 0;

		while(inStream.peek() != END_LINE_CHARACTER)
		{
			Continent continent;
			getline(inStream, strCont, '=');
			continent.setName(strCont);
			getline(inStream, scoreCont, '\n');
			continent.setScore(atoi(scoreCont.c_str()));
			continents.push_back(continent);
			index++;
		}

		setContinents(continents);

		getline(inStream, skip, '\n');//read empty line
		getline(inStream, skip, '\n');//read [territories]

		string territoryName;
		string parentContinent;
		std::list<Territory> listTerritories;

		int x, y;


		while(!inStream.eof())
		{
			if (inStream.peek() != END_LINE_CHARACTER && inStream.peek() != -1)
			{
				std::list<string> adjacent;
				getline(inStream, territoryName, ',');//read territory

				Territory territory;
				territory.setName(territoryName);

				inStream >> x;//read x
				territory.setX(x);
				inStream.get();
				inStream >> y;//read y
				territory.setY(y);
				inStream.get();

				getline(inStream, parentContinent, ',');//read continent it belongs to
				territory.setParentCont(parentContinent);
				
				

				getline(inStream, territoryName, END_LINE_CHARACTER);//read adjacents		

				std::istringstream ss(territoryName);
				std::string token;

				while(std::getline(ss, token, ','))
				{
					adjacent.push_back(token);
				}
				territory.setAdjacent(adjacent);
				listTerritories.push_back(territory);
					
			}
			else
			{
				inStream.get();
			}
		}
		setTerritories(listTerritories);
}

void MapConfig::saveMapToFile()
{
	if (isConnectedGraph() && areContinentsSubGraphs() && isOneCountyPerContinent())
	{
		outStream<<"[Map]"<<endl;
		outStream<<"author="<<author<<endl;
		outStream<<"image="<<image<<endl;
		outStream<<"wrap="<<wrap<<endl;
		outStream<<"scroll="<<scroll<<endl;
		outStream<<"warn="<<warn<<endl<<endl;

		outStream<<"[Continents]"<<endl;

		 for (std::list<Continent>::iterator it = continents.begin(); it != continents.end(); ++it)
		  {
			  outStream<<(*it).getName()<<"="<<(*it).getScore()<<endl;
		  }

		outStream<<endl;
		outStream<<"[Territories]"<<endl;

		for (std::list<Territory>::iterator it = territories.begin(); it != territories.end(); ++it)
			{
				Territory tempTerr = *it;
				outStream << tempTerr.getName() << ",";
				outStream << tempTerr.getX() << ","<< tempTerr.getY() << ",";
				outStream << tempTerr.getParentCont() << ",";

				std::list<string> terr = tempTerr.getAdjacent();

				for (std::list<string>::iterator itStr = terr.begin(); itStr != terr.end(); itStr++)
				{
					outStream << *itStr;
					//outputs the separating colon, making sure it doesn't put one after the last neighbour.
					if (next(itStr) !=  terr.end()){
						outStream << ",";
					}
				}
				outStream<<endl;
			}
	}
	else
	{
		cout<<"The map you are proving doesn't meet the criteria of the game";
	}
	outStream.close();
}

