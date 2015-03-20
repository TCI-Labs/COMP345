#pragma once

class Dice
{
public:
  void roll_dice(int attackingArmies, int defendingArmies);  //function to generate dice roll
  inline int getFirstAttackDie(){return attack_dice[0];}
  inline int getFirstDefendDie(){return defend_dice[0];}
  inline int getSecondAttackDie(){return attack_dice[1];}
  inline int getSecondDefendDie(){return defend_dice[1];}
private:
  int attack_dice [3];
  int defend_dice [2];
};