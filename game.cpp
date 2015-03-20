#include "game.h"
#include <sstream>
#include <iostream>

Game::Game(int newNumOfPlayers, std::string * newPlayerNames){
  numOfPlayers = newNumOfPlayers;
  playerNames = new std::string[numOfPlayers]();
  for(int x = 0; x < numOfPlayers; x++){
    playerNames[x] = newPlayerNames[x];
  }
  
  map = new Map("World.map"); ///home/pc/Desktop/Concordia 15-01/Comp345/345projectWorkingFolder/
  //map->setupHardcodedMap();  
  map->setupCountryOwners(numOfPlayers);
  
  textview = new TextView(*map);
  controller = new Controller();
  dice = new Dice();
}

Game::~Game(){
  delete map;
  delete textview;
  delete controller;
  delete dice;
  delete [] playerNames;
}

int Game::play(){
  int playerTurns = 0;
  int playerIndex = 0;
  
	//Main Game Loop
	while(true){
		map->notify();
		textview->inform("Round " + intToString(playerTurns/numOfPlayers + 1) + " : " + playerNames[playerIndex] + " (player " + intToString(playerIndex)+ ")'s turn");
		
		//reinforce, attack and move are the 3 actions a given player can do during his turn
		reinforce(playerIndex);
		
		attack(playerIndex);
	
		fortify(playerIndex);
		
		playerTurns++;
		playerIndex = playerTurns % numOfPlayers;

		
	}
  return 0;
}

bool Game::countryExistsAndFriendly(std::string country, int playerIndex){
  if (!map->countryExists(country)){
    textview->inform(country + " does not exist.");
    return false;
  }
  else if(map->getCountryOwnerIndex(country) != playerIndex){
    textview->inform(country + " does not belong to you.");
    return false;
  }
  return true;
}

void Game::outputCountryList(std::list<std::string> countryList){
  //output list of countries
  std::list<std::string>::iterator iter = countryList.begin();
  while(iter != countryList.end()){
    textview->inform("  " + *iter);
    iter++;
  }
}

void Game::fortify(int playerIndex){
	std::string answer;
	textview->prompt (playerNames[playerIndex] + ", would you like to make a fortification move (y/n)?");
	answer = controller->getString();

	if(answer == "n"){
		return;
	}
	
	std::string sourceCountry;
	std::string destinationCountry;
	std::list<std::string> lst;
	int numToMove;
	
	//input loop: prompt user for a friendly country until they pick one with friendly neighbours
	do{
	  //input loop: prompt user for a friendly country until they pick one with enough armies to move elsewhere
	  do{
	    textview->prompt("Choose a country to move troops from.");
	    sourceCountry = controller->getString();	    
	    //if country doesn't have enough armies to move some to a neighbour
	    if(map->countryExists(sourceCountry) && map->getCountryOwnerIndex(destinationCountry) == playerIndex && map->getCountryArmies(sourceCountry) < 2){
		textview->inform(sourceCountry + " does not have enough armies to move troops to another country");
	    }
	  
	  }while(!(countryExistsAndFriendly(sourceCountry, playerIndex) && map->getCountryArmies(sourceCountry) > 2));
	
	  //get list of all countries connected to the selected country
	  lst = map->getConnectedFriendlyCountries(sourceCountry, playerIndex);
	  
	  //if the country has no countries connected to it (other than itself)
	  if(lst.size() <= 1){
	    textview->inform(sourceCountry + " is not connected to any friendly neighbours to which it can move armies.");
	  }
	}while(lst.size() <= 1);
	
	textview->inform(sourceCountry + " is connected to:");
	outputCountryList(lst);
	
	//Input loop: prompt for a friendly country until they enter one on the list of connected countries
	do{

	    textview->prompt("Choose the country to fortify.");
	    destinationCountry = controller->getString();   
	
	  //if the entered country is not on the list of connected countries
	  if(map->countryExists(destinationCountry) && map->getCountryOwnerIndex(destinationCountry) == playerIndex && !listContains(lst,destinationCountry)){
	    textview->inform(destinationCountry + " is not connected to " + sourceCountry + " and therefore cannot be fortified with armies from " + sourceCountry + ".");
	  }
	}while(!(countryExistsAndFriendly(destinationCountry, playerIndex) && listContains(lst,destinationCountry)));
	
	//Input loop: prompt for the number of armies to move until they pick a number between 1 and the max they can move
	textview->prompt (sourceCountry + " can move up to " + intToString (map->getCountryArmies(sourceCountry) -1) + " armies to " + destinationCountry + ".  How many would you like to move?");
	do{	  
	  numToMove = controller->getInt();
	  
	  //if they pick a number not between 1 and the number they are allowed to move
	  if (numToMove < 1 || numToMove > map->getCountryArmies(sourceCountry) -1){
	    textview->inform("Invalid input.");
	    textview->prompt("Please enter a number between 1 and " + intToString(map->getCountryArmies(sourceCountry)-1) + ".");
	}
	} while(numToMove < 1 || numToMove > map->getCountryArmies(sourceCountry) -1);
	
	//update the two countries army counts
	map->setCountryArmies(sourceCountry, map->getCountryArmies(sourceCountry) - numToMove, false);
	map->setCountryArmies(destinationCountry, map->getCountryArmies(destinationCountry) + numToMove);
	
	textview->inform (intToString(numToMove) + " armies moved from " + sourceCountry + " to " + destinationCountry + ".");  
}


void Game::reinforce(int playerNum){
  int bonusArmies = 0;
  int numToReinforce = 0;
  std::string countryToReinforce;
  
  //calculate # of reinforcements
  bonusArmies = map->computeContinentsBonuses(playerNum);  
  bonusArmies += map->countCountriesOwned(playerNum) / 3;
  
  //****Need a way to track cards belonging to the player, and here check if they want 
  //to cash them in and add the appropriate armies if so********
  
  do{
    //Get the country to be reinforced
    textview->inform(playerNames[playerNum] + ", you have " + intToString(bonusArmies) + " armies of reinforcements to distribute.");

    textview->prompt("Please input the country you want to reinforce.");
    countryToReinforce = controller->getString();
    
    if (countryExistsAndFriendly(countryToReinforce, playerNum)){
      //Get number of reinforcements to deploy in the selected country
      do{
	textview->prompt("Please input the number of armies to add to " + countryToReinforce);
	numToReinforce = controller->getInt();
	if (numToReinforce < 1 || numToReinforce > bonusArmies){
	  textview->inform("Invalid input, please enter a number between 1 and " + intToString(bonusArmies));
	}
      } while(numToReinforce < 1 || numToReinforce > bonusArmies);
      
      //put the number of armies specified in the country specifeid and decrement the number of bonus armies
      map->setCountryArmies(countryToReinforce, map->getCountryArmies(countryToReinforce) + numToReinforce);
      bonusArmies -= numToReinforce;
      textview->inform(countryToReinforce + " reinforced with " + intToString(numToReinforce) + " armies and now has " + intToString(map->getCountryArmies(countryToReinforce)) + " armies.");
    }    
  }while(bonusArmies > 0);
  
}


void Game::attack(int playerNum){
  std::string inString = "f";
  
  std::string attackingCountry;
  std::string defendingCountry;
  std::list<std::string> enemyNeighbourList;
  
  //Prompt whether the user wants to make an attack move.
  while(true){
    if(inString.compare("f") == 0){
      textview->prompt(playerNames[playerNum] + ", would you like make an attack (y/n)?");
    }else{
      textview->prompt(playerNames[playerNum] + ", would you like to attack another country (y/n)?");
    }
    inString = controller->getString();
    
    //If the user indicated no, exit the attack phase
    if(inString.compare("n") == 0){
      return;
    }
        
//GET THE ATTACKING COUNTRY************************************************************   
    do{
      do{
	textview->prompt("Choose a country you wish to launch your attack from.");
	attackingCountry = controller->getString();    
  
	//if country doesn't have enough armies to attack another country
	if(map->countryExists(attackingCountry) && map->getCountryOwnerIndex(attackingCountry) == playerNum && map->getCountryArmies(attackingCountry) < 2){
	  textview->inform(attackingCountry + " does not have enough armies to launch an attack.");
	}
  
      }while(!(countryExistsAndFriendly(attackingCountry, playerNum) && map->getCountryArmies(attackingCountry) > 1));
      
      enemyNeighbourList = map->getEnemyNeighbours(attackingCountry, playerNum);
      
      //it is possible that a country has nothing to attack; for example when a country is in between friendly countries
      //the statement below avoids such possiblity
      if(enemyNeighbourList.empty()){
	textview->inform(attackingCountry + " has no enemy neighbours to attack.");
      }
      
    }while(enemyNeighbourList.empty());
  
//GET THE DEFENDING COUNTRY************************************************************
    //output all available countries to attack
    textview->inform(attackingCountry + "'s enemy neighbours are:");
    outputCountryList(enemyNeighbourList);
   
    //Ask for user input and validates to see whether player chose an enemy country       
    do{
      //get country choice and error check
      textview->prompt("Choose the country you want to attack");
      defendingCountry = controller->getString();
      
      //if you selected a country that isn't a neighbour
      if(!map->countryExists(defendingCountry)){
	textview->inform(defendingCountry + " does not exist.");
      }
      //if you selected a friendly country to attack
      else if (map->getCountryOwnerIndex(defendingCountry) == playerNum)
      {
	textview->inform("Invalid choice: " + defendingCountry + " already belongs to you.");
      }
      else if (!listContains(enemyNeighbourList, defendingCountry)){
	textview->inform("Invalid choice: " + defendingCountry + " is not a neighbour of " + attackingCountry + ".");
      }
    }while(!(map->countryExists(defendingCountry) && map->getCountryOwnerIndex(defendingCountry) != playerNum && 
      listContains(enemyNeighbourList, defendingCountry)));
    
    battle(attackingCountry, defendingCountry);
    
  }
  
}//end ATTACK function
  
  //Pre: attackingCountry and defendingCountry are validly selected countries to do battle.
//DO BATTLE**************************************************************
void Game::battle(std::string attackingCountry, std::string defendingCountry)
{   
  // Boolean variable which will verify whether the attacker wants to keep attacking, it's gonna be used at very end of the while loop
  bool continueBattle = true;
  int lastAttackDice;
  int attackingArmies = map->getCountryArmies(attackingCountry);
  int defendingArmies = map->getCountryArmies(defendingCountry);
  
  std::string outString;
  std::string inString;
  int inInt;
  
  textview->inform("\n" + attackingCountry +" attacks " + defendingCountry + " with " + intToString(attackingArmies) + " armies against " + intToString(defendingArmies) + " armies.");
  
  // This while loop is placed to verify whether the attacker wants to keep attacking in case the battle phase is finished and no one won yet
  
  do{
    //function to randomly generates the dices
    dice->roll_dice(attackingArmies, defendingArmies);  

//BATTLE DAMAGES CALCULATIONS--------------------------------------------------------------
    //function to determine the fightning phase, who wins and who losses
    
    outString = "";
    if(attackingArmies >1 && defendingArmies > 1){
      if(dice->getFirstAttackDie() > dice->getFirstDefendDie()){
	outString = "The attacker won ";
	defendingArmies--;
      }
      else{
	outString = "The attacker lost "; 
	attackingArmies--;
      }
      outString += "the first attack " + intToString(dice->getFirstAttackDie()) + ":" + intToString(dice->getFirstDefendDie());
      
      if(dice->getSecondAttackDie() > dice->getSecondDefendDie()){
	outString += "\nThe attacker won ";
	defendingArmies--;
      }
      else{
	outString += "\nThe attacker lost ";
	attackingArmies--;
      }
      outString += "the second attack " + intToString(dice->getSecondAttackDie()) + ":" + intToString(dice->getSecondDefendDie());
      lastAttackDice = dice->getSecondAttackDie();
    }  
    else{
      if(dice->getFirstAttackDie() > dice->getFirstDefendDie()){
	outString = "The attacker won ";
	defendingArmies--;
      }
      else{
	outString = "The attacker lost "; 
	attackingArmies--;
      }
      outString += "the attack " + intToString(dice->getFirstAttackDie()) + ":" + intToString(dice->getFirstDefendDie());
      lastAttackDice = dice->getFirstAttackDie();
    }
    
    textview->inform(outString);
    
    //output updated army counts 
    textview->inform(attackingCountry +": " + intToString(attackingArmies) + " armies remaining \n" + defendingCountry + ": " + intToString(defendingArmies) + " armies remaining");
    
    
//BATTLE OUTCOME CALCULATIONS--------------------------------------------------------------
    outString = "";
    if(attackingArmies <= 1)    //Verify whether the attacker has no army left
    {
      continueBattle = false;
      outString += "\n" + attackingCountry +" does not have enough armies to continue the attack";
      
    }
    else if(defendingArmies <= 0)    //Verify whether the defender has no army left
    {
      continueBattle = false;
      outString += "\nYou have conquered " + defendingCountry + ".";
      outString += "\nYou must settle at least " + intToString(lastAttackDice) + " armies in this newly conquered territory.";
      
      //The attacker won, so now he must deploy his army
      if(attackingArmies > lastAttackDice)                 
      {
	//Validates to see whether the player has deployed a valid amount of army to conquered country
	do{
	  textview->inform(outString);
	  outString = "";
	  textview->prompt("How many armies would you like to settle in "+ defendingCountry + "?");
	  inInt = controller->getInt();
	  //Asking the user again to deploy the right amount
	  if(inInt > attackingArmies - 1 || inInt < lastAttackDice){
	    textview->inform("Invalid input. Need to add between " + intToString(lastAttackDice) + " and " + intToString(attackingArmies-1) + " armies.");
	  }
	}while(inInt > attackingArmies - 1 || inInt < lastAttackDice);
	
	//Update the number of armies in the two countries
	defendingArmies = inInt;
	attackingArmies -= inInt;	
	
	outString = intToString(inInt) + " armies moved from " + attackingCountry + " to " + defendingCountry + ".\n";
      }
      else    // This is when the attacker won but doesn't have enough army greater than his previous rolled dice
      {
	outString += "\nBecause you have only " + intToString(attackingArmies) + " armies left in " + attackingCountry + ", you automatically settle " + intToString(attackingArmies-1) + " in " + defendingCountry + ".";
	
	//Update the army counts
	defendingArmies = attackingArmies-1;
	attackingArmies = 1;
      }
      
      //change the owner of the country
      map->setCountryOwnerIndex(defendingCountry, map->getCountryOwnerIndex(attackingCountry), false);
    }
    
    //If both sides attacker and defender still have armies left after the battle phase then the attacker will receive the choice to continue attacking or stop
    else                
    {
      textview->prompt("Would you like to continue the attack (y/n)?");
      inString = controller->getString();
      continueBattle = inString.compare("n") != 0;
    }
  }while(continueBattle);
  
  //Update the number of armies in the two countries before terminating the function
  map->setCountryArmies(defendingCountry, defendingArmies, false);
  map->setCountryArmies(attackingCountry, attackingArmies);
  
  
//Output summary of the battle via outString
  outString += "\nAt the end of battle:";
  outString += "\n  " + attackingCountry + " has " + intToString(attackingArmies) + " armies";
  outString += "\n  " + defendingCountry + " has " + intToString(defendingArmies) + " armies";
  outString += "\n  " + defendingCountry;
    
 if (map->getCountryOwnerIndex(defendingCountry) == map->getCountryOwnerIndex(attackingCountry)){
   outString += " now ";
 }
 else
 {
   outString += " still ";
 }
 
 outString += " belongs to " + playerNames[map->getCountryOwnerIndex(defendingCountry)] + "\n";
 
 textview->inform(outString);
  
}//END battle function