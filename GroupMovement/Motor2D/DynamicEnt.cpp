#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "DynamicEnt.h"
#include "Brofiler/Brofiler.h"

DynamicEnt::DynamicEnt(DynamicEntityType type) : j1Entity(entityType::DYNAMIC)
{

}

DynamicEnt::~DynamicEnt()
{}



bool DynamicEnt::Awake(pugi::xml_node& config)
{

	return true;
}

bool DynamicEnt::CleanUp()
{

	return true;
}