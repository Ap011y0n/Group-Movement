#ifndef __j1Entity_H__
#define __j1Entity_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include <list>

using namespace std;

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
	bool isSelected;
	bool selectable;
	fPoint position;
	bool to_delete;
	int body;
	fPoint speed;

	
};

#endif // __j1Entity_H__