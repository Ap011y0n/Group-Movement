#ifndef __j1EntityManager_H__
#define __j1EntityManager_H__
	
#include "PugiXml/src/pugixml.hpp"
#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "DynamicEnt.h"
#include "StaticEnt.h"


struct SDL_Texture;

class j1EntityManager : public j1Module
{
public:
	// Constructor
	j1EntityManager();

	// Destructor
	~j1EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Create a new entity
	j1Entity* CreateEntity(DynamicEnt::DynamicEntityType type, int posx = 0, int posy = 0);

	j1Entity* CreateStaticEntity(StaticEnt::StaticEntType type, int posx = 0, int posy = 0);


	// Delete an entity
	bool DeleteEntity(j1Entity* entity);

public:
	p2List<j1Entity*> entities;

	// Load entities textures
	p2SString test_1;
	SDL_Texture* test_1_graphics;
};

#endif // __j1EntityManager_H__