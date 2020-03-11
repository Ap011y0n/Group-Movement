#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "DynamicEnt.h"
#include "Brofiler/Brofiler.h"
#include "j1Map.h"

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

void DynamicEnt::CheckCollisions(fPoint* speed)
{
	iPoint coord;
	p2List_item<MapLayer*>* layer_iterator = App->map->data.layers.start;
	MapLayer* layer = App->map->data.layers.start->data;

	while (layer_iterator != NULL)
	{
		layer = layer_iterator->data;
		if (layer->returnPropValue("Navigation") == 1) {
			coord = App->map->WorldToMap((int)(position.x + speed->x), (int)position.y);
			if (layer->Get(coord.x, coord.y) != 0) speed->x = 0;

			coord = App->map->WorldToMap((int)position.x, (int)(position.y + speed->y));
			if (layer->Get(coord.x, coord.y) != 0) speed->y = 0;

		}
		layer_iterator = layer_iterator->next;
	}
}