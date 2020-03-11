#pragma once
#ifndef __DynamicEnt_H__
#define __DynamicEnt_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Entity.h"




class DynamicEnt : public j1Entity
{
public:

	enum DynamicEntityType
	{
		NO_TYPE,
		TEST_1,
		TEST_2
	};

	// Constructor
	DynamicEnt(DynamicEntityType type);

	// Destructor
	~DynamicEnt();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

public:



protected:
	int followpath;
	int collrange;
	int vision;


	iPoint origin, mouse;
	p2DynArray<iPoint> path;
	list<j1Entity*> close_entity_list;
	list<j1Entity*> colliding_entity_list;

	
};

#endif // __j1Entity_H__