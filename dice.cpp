#include "dice.h"

#include <time.h>
#include <cstdlib>

void Dice::roll_dice(int attackingArmies, int defendingArmies)
{		
	int replace;
	
	srand(time(NULL) );   //Allows to generate unique numbers
		
	attack_dice[2] = 0;
	attack_dice[1] = 0;
	attack_dice[0] = 0;

	//Assigning numbers 1 to 6 to dices for attacker, also includes validation
	if(attackingArmies >= 3)          
	{
		attack_dice[2] = rand() % 6 + 1;
		attack_dice[1] = rand() % 6 + 1;
		attack_dice[0] = rand() % 6 + 1;
	}
	else if(attackingArmies == 2)
	{
		attack_dice[1] = rand() % 6 + 1;
		attack_dice[0] = rand() % 6 + 1;
	}
	else if(attackingArmies == 1)
	{
		attack_dice[0] = rand() % 6 + 1;
	}
	
	//Verify that the array has the two highest dices numbers first for attacker
	if (attackingArmies >1){
	  for(int i = 1; i <= 2; i++)            
	  {
		  if(attack_dice[i-1] < attack_dice[i])
		  {
			replace = attack_dice[i-1];
			attack_dice[i-1] = attack_dice[i];
			attack_dice[i] = replace;
		  }
	  }
	}
	
	defend_dice[1] = 0; 
	defend_dice[0] = 0;
	
	//Assigning numbers 1 to 6 to dices for defender, also includes validation
	if(defendingArmies >= 2)        
	{
	    defend_dice[1] = rand() % 6 + 1;
	    defend_dice[0] = rand() % 6 + 1;
	}
	else if(defendingArmies == 1)
	{
	    defend_dice[0] = rand() % 6 + 1;
	}
	
	//Verify that the array has highest numbers first for defender
	if(defend_dice[0] < defend_dice[1])   
	{
		replace = defend_dice[0];
		defend_dice[0] = defend_dice[1];
		defend_dice[1] = replace;
	}
}
