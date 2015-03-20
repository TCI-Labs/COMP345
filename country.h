#pragma once

#include <string>
#include "continent.h"

class Continent; //forward declaration needed due to circular dependancy between Continent and Country;

class Country{
public:
  Country(std::string cName, Continent & setContinent);
  
  void addNeighbour(Country & newNeighbour, bool unidirectional = true); 
  
  
  //ACCESSORS------------------------------------
  int getArmies();
  std::string getName();
  int getNeighbourCount();
  Country ** getAllNeighbours();
  Country * getNeighbour(std::string countryName);
  int getOwnerIndex();
  bool isNeighbour(std::string countryName);
  std::string to_string();


  //MUTATORS-------------------------------------
  void setOwnerIndex(int newOwnerIndex);
  void setArmies(int newArmies);

/*  
	int defender_army; //other person's army

	inline void set_country(std::string);

	inline void set_defender(std::string);
*/
  
private:
  void addNeighbourLink(Country &newNeighbour);
  int armies;
  std::string name;
  int neighbourCount;
  Country ** neighbours; //array of countries that constitute the countries adjacent to this country
  int ownerIndex;
  Continent * myContinent;
    
};

std::ostream& operator<<(std::ostream& os, Country & outputCountry);

//INDEPENDANT HELPER TEMPLATE METHODS------------------
//(Templates have to be defined entirely in headers because they don't have function headers)

/*
 * Adds elements to an array by order of name
 * It assumes the first letter is a capital for comparison purposes.
 * 
 * Post: Array is in alphabetical order, a being at 0 and z being at the end.
 */
template<typename T>
T** insertElement(T * oldArray[], int & elementCount, T & newElement){
   
  bool addFlag = false;  
  elementCount++;
  T ** newElementArray = new T*[elementCount];
    
      for(int x = 0; x < elementCount; x++){
	
	//if the newElement is alphabetically ahead of element[x], insert it
	if (!addFlag && x < elementCount - 1 && oldArray[x]->getName().compare(newElement.getName()) > 0){
	  newElementArray[x] = &newElement;
	  addFlag = true;
	}
	else
	{
	  //if the new element hasn't been inserted yet and we're at the last element, insert it
	  if(!addFlag && x == elementCount - 1){
	    newElementArray[x] = &newElement;
	  }
	  //if the new element hasn't been inserted yet, just copy values directly from the old array
	  else if (!addFlag){
	    newElementArray[x] = oldArray[x];
	  }
	  //if the new element has been inserted, copy values from the old array position to one
	  //	position further right (because the inserted element is now taking up a space)
	  else{
	    newElementArray[x] = oldArray[x-1];
	  }
	}
      }
  
  return newElementArray;
  
}


/*
 * Returns an element searched for by name using binary search
 * NOTE: RETURNS NULL IF NO ELEMENT IS FOUND
 * 
 * Pre: array is sorted.
 */
template<typename T>
T & findElement(T * array[], int & elementCount, std::string & elementName){
  T * returnElement = NULL;
  bool end = false;
  int leftBound = 0;
  int checkElement;
  int rightBound = elementCount - 1;
  
  if (elementCount != 0){
    
   while(!end){
     
      checkElement = rightBound + (leftBound - rightBound) /2  ;
      
      //if the element has been found, return it
      if(array[checkElement]->getName().compare(elementName) == 0){
	returnElement = array[checkElement];
	end = true;
      }
      //if the lower bound, the upper bound and the element being checked are all the same
      //  or if the lowerBound is higher than the upper bound
      //  then quit.
      else if (leftBound == checkElement && rightBound == checkElement || leftBound > rightBound){
		if (returnElement == NULL){
	}
		  end = true;
      }
      //if the element being checked is alphabetically lower than the search item
      //   move the lowest possible search item to one higher than the element being checked
      else if (array[checkElement]->getName().compare(elementName) < 0){
	leftBound = checkElement + 1;
      }
      //if the element being checked is alphabetically higher than the search item
      //   move the highest possible search item to one lower than the element being checked
      else {
	rightBound = checkElement - 1;
      }
    }
  }
  return *returnElement;
}
