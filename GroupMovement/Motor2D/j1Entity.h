#ifndef __j1Entity_H__
#define __j1Entity_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"

struct SDL_Texture;

class j1Entity : public j1Module
{
public:
	enum entityType
	{
		NO_TYPE,
		STATIC,
		DYNAMIC
	};

	entityType type;

	// Constructor
	j1Entity(entityType type);

	// Destructor
	~j1Entity();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

public:

	// Position
	iPoint position;

	int hp;
	bool isSelected;


	SDL_Texture* texture;
	//SDL_Rect Rect;

	int cost;
	int MaxCreationTime;
	int CurrentCreationTime;

	// Blit
	bool blit = false;

	// Colliders
	Collider* collider;
	bool to_delete;

};

#endif // __j1Entity_H__