#pragma once
#ifndef __StaticEnt_H__
#define __StaticEnt_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Entity.h"



class StaticEnt : public j1Entity
{
public:
	
	enum StaticEntType
	{
		NO_TYPE,
		TEST_3,
	};

	// Constructor
	StaticEnt(StaticEntType type);

	// Destructor
	~StaticEnt();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

public:

//	SDL_RendererFlip flip = SDL_FLIP_NONE;
	j1Entity* target;
	int vision;
	int damage;
	int armor;

};

#endif // __j1Entity_H__