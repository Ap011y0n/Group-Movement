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
	enum entityDirection
	{
		UP,
		DOWN,
		FRONT,
		DIAGONALUP,
		DIAGONALDOWN,
	};

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



	entityDirection direction;
//	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int collrange;
	int vision;

	int damage;
	int armor;
	
};

#endif // __j1Entity_H__