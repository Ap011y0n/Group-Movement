#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "StaticEnt.h"
#include "Brofiler/Brofiler.h"

StaticEnt::StaticEnt(StaticEntType type) : j1Entity(entityType::STATIC)
{

}

StaticEnt::~StaticEnt()
{}



bool StaticEnt::Awake(pugi::xml_node& config)
{

	return true;
}

bool StaticEnt::CleanUp()
{

	return true;
}