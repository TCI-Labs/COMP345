#include <string>
#include <list>
using namespace std;

#pragma once
class Territory
{
public:
	Territory(void);
	~Territory(void);
	void setAdjacent(list<string> adjacent);
	void setX(int x);
	void setY(int y);
	void setParentCont(string parent);
	void setName(string name);

	list<string> getAdjacent();
	int getX();
	int getY();
	string getParentCont();
	string getName();

private:
	int x;
	int y;
	string parentCont;
	list<string> adjacent;
	string name;


};

