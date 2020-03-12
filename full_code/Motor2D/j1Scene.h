#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Animation.h"

#define COORDS(a) a+3000 

struct SDL_Texture;
class j1Entity;
class DynamicEnt;
class StaticEnt;;


class GuiItem;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	
private:
	bool changeEntities = false;

public:
	p2SString current_level;
	SDL_Texture* Hello;
	SDL_Texture* debug_tex;
	bool debug;
};

#endif // __j1SCENE_H__