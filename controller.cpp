#include "controller.h"
#include <iostream>



int Controller::getInt(){
  using std::cin;
  
  int inInt = 0;
  do{
    if(!cin){
      cin.clear();
      cin.ignore(256,'\n');
    }
    
    cin >> inInt;
    
    if(!cin){
      std::cout << "Invalid input. Please enter a valid integer." << std::endl;
    }
  }while (!cin);
    
  return inInt;
  
}

std::string Controller::getString(){
	  std::string i;
	  //This filters out extra \n's left over from previous input and gets input
	  do{
	    getline( std::cin, i);
	  }while((int)i[0] == 0);

	  return i;
}