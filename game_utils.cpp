#include "game_utils.h"


#include <algorithm> //std::find function

//HELPER METHOD FOR FORTIFY
bool listContains(std::list<std::string> & lst, std::string str){
  std::list<std::string>::iterator findIter = std::find(lst.begin(), lst.end(), str);
  
  return findIter != lst.end() || findIter == lst.end() && lst.back().compare(str) == 0;
}

std::string intToString(int i){
  int temp = i;
  int digit = 0;
  int counter = 0;
  int length = 0;

  do{
    temp /= 10;
    counter++;
  }while (temp != 0);
  
  length = counter;
  
  if (i < 0){
   length++; 
   temp = -1 * i;
  }else{
    temp = i;
    counter --;
  }
  
  char * toString = new char[length];
  
  if (i<0){
    toString[0] = '-';
  }
  
  do{
    digit = temp %10;
    toString[counter]= (char)digit + 48;
    
    temp /=10;
    counter --;
    
  }while (temp != 0);
  
  std::string returnString (toString, length);

  delete [] toString;
      
  return returnString;

}