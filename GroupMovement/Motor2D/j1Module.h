// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;
struct Collider;
class GuiItem;


class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate(float dt = 0)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt = 0)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate(float dt = 0)
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnCollision(Collider*, Collider*) {}
	virtual void GuiInput(GuiItem*) {}

public:

	p2SString	name;
	bool		active;

};

#endif // __j1MODULE_H__