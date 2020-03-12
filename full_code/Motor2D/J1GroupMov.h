#pragma once
#ifndef __j1GROUPMOV_H__
#define __j1GROUPMOV_H__


#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include <list>

using namespace std;

class j1GroupMov : public j1Module
{
public:
	j1GroupMov();
	~j1GroupMov();
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	fPoint GetSeparationSpeed(list<j1Entity*>, fPoint);
	fPoint GetCohesionSpeed(list<j1Entity*>, fPoint);
	fPoint GetDirectionSpeed(list<j1Entity*>);
	list<j1Entity*>	selected;
	bool NewGroup;

};

#endif