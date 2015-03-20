#pragma once
#include "observer.h"

class Observable{
  
public:
	virtual void attach(Observer & ob)=0;
	virtual void detach(Observer & ob)=0;
	virtual void notify()=0;
};