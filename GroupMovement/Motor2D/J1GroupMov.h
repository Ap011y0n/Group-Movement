#pragma once
#ifndef __j1GROUPMOV_H__
#define __j1GROUPMOV_H__


#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include <list>

using namespace std;
struct MassCenter
{


	iPoint				pos;
	p2List<j1Entity*>	selected;
};

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

	
public:
	MassCenter Center;
	bool RelocateCenter;
	iPoint pos;
};

#endif