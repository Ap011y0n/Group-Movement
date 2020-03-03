#include "j1EntityManager.h"
#include "j1Entity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"

j1Entity::j1Entity(entityType type) : type(type)
{
	name.create("entity");
}

j1Entity::~j1Entity()
{}

bool j1Entity::Awake(pugi::xml_node& config)
{

	return true;
}

bool j1Entity::CleanUp()
{

	return true;
}