#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Test_1.h"
#include "DynamicEnt.h"
#include "Brofiler/Brofiler.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Input.h"
#include "J1GroupMov.h"
#include <math.h>

Test_1::Test_1(int posx, int posy) : DynamicEnt(DynamicEntityType::TEST_1)
{
	name.create("test_1");

	speed = { 0, 0 };
	vision = 20;
	body = 7;
	collrange = 10;
	position.x = posx;
	position.y = posy;
	to_delete = false;
	isSelected = false;
	selectable = true;
	// Load all animations
}

Test_1::~Test_1()
{}

bool Test_1::Start()
{
	
	return true;
}

bool Test_1::Update(float dt)
{
	BROFILER_CATEGORY("UpdateTest_1", Profiler::Color::BlanchedAlmond);
	speed = { 0, 0 };
	pathSpeed = { 0, 0 };

	CheckAnimation(dt);

	origin = App->map->WorldToMap(position.x, position.y);
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT)
		to_delete = true;

	if (isSelected && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		App->input->GetMousePosition(mouse.x, mouse.y);
		mouse = App->map->WorldToMap(mouse.x, mouse.y);
		App->pathfinding->CreatePath(origin, mouse);

		const p2DynArray<iPoint>* last_path = App->pathfinding->GetLastPath();
		path.Clear();
		for (uint i = 0; i < last_path->Count(); ++i)
		{
			path.PushBack({last_path->At(i)->x, last_path->At(i)->y});
		}
		followpath = 1;
	}

	//----------------------------------------------------------------Path speed

	if (path.At(followpath) != NULL)
	{
		for (uint i = 0; i < path.Count(); ++i)
		{
			iPoint nextPoint = App->map->MapToWorld(path.At(i)->x, path.At(i)->y);
			if (i == followpath)
			{
				App->render->DrawQuad({ nextPoint.x + 14, nextPoint.y + 14, 12, 12 }, 200, 0, 0, 100);
			}
			else {
				App->render->DrawQuad({ nextPoint.x + 14, nextPoint.y + 14, 6, 6 }, 200, 0, 0, 100);

			}
		}
		
		
			if (path.At(followpath)->x < origin.x) {
				pathSpeed.x =- 1;
			}

			if (path.At(followpath)->x > origin.x) {
				pathSpeed.x =+ 1;
			}

			if (path.At(followpath)->y < origin.y) {
				pathSpeed.y =- 1;
			}

			if (path.At(followpath)->y > origin.y) {
				pathSpeed.y = 1;
			}
			if (origin.x == path.At(followpath)->x && origin.y == path.At(followpath)->y)
			{
				followpath++;
			}
				
		
	}

	j1Entity* it;
	list<j1Entity*>::iterator neighbours_it;

	//TODO ----------------------------------------------------------------Save neighbours in two lists

	SaveNeighbours(&close_entity_list, &colliding_entity_list);

	// TODO ---------------------------------------------------------------- Separation Speed
	fPoint separationSpeed;

	if (!colliding_entity_list.empty())
	{
		separationSpeed = App->movement->GetSeparationSpeed(colliding_entity_list, position);
	}
	else
	{
		separationSpeed.x = 0;
		separationSpeed.y = 0;
	}
//	App->render->DrawCircle((int)position.x + 5, (int)position.y + 5, collrange, 200, 200, 0, 200);
//	App->render->DrawCircle((int)position.x + 5, (int)position.y + 5, body, 0, 200, 200, 200);
	

	//TODO ---------------------------------------------------------------- Cohesion speed
	fPoint cohesionSpeed;
	if (!close_entity_list.empty())
	{
		cohesionSpeed = App->movement->GetCohesionSpeed(close_entity_list, position);
	}
	else
	{
		cohesionSpeed.x = 0;
		cohesionSpeed.y = 0;
	}

	//TODO ---------------------------------------------------------------- Direction speed
	fPoint directionSpeed;
	/*if (!close_entity_list.empty())
	{
		directionSpeed = App->movement->GetDirectionSpeed(close_entity_list);
	}
	else
	{
		directionSpeed.x = 0;
		directionSpeed.y = 0;
	}
	*/

	//TODO ---------------------------------------------------------------- Add all speeds

	speed.x += 1.5*pathSpeed.x + 1*separationSpeed.x + 0.5 *cohesionSpeed.x + 0*directionSpeed.x;
	speed.y += 1.5*pathSpeed.y + 1*separationSpeed.y + 0.5 *cohesionSpeed.y + 0*directionSpeed.y;

	//------------------------------------------------------------------Use a preventive collision system

	CheckCollisions(&speed);

	position.y += speed.y;
	position.x += speed.x;
	

	if (isSelected)
		App->render->DrawCircle((int)position.x + 5, (int)position.y + 5, 10, 0, 200, 0, 200);

	App->render->DrawQuad({ (int)position.x, (int)position.y, 10, 10 }, 200, 200, 0);

	return true;
}

bool Test_1::PostUpdate(float dt)
{
	BROFILER_CATEGORY("PostupdateTest_1", Profiler::Color::BurlyWood)

	return true;
}

void Test_1::CheckAnimation(float dt)
{

}

bool Test_1::CleanUp()
{
	close_entity_list.clear();
	colliding_entity_list.clear();
	path.Clear();
	return true;
}

void Test_1::SaveNeighbours(list<j1Entity*>* close_entity_list, list<j1Entity*>* colliding_entity_list)
{

	p2List_item<j1Entity*>* entities_list = App->entity->entities.start;
	close_entity_list->clear();
	colliding_entity_list->clear();
	while (entities_list)
	{
		if (entities_list->data != this && entities_list->data->selectable)
		{
			int x = entities_list->data->position.x;
			int y = entities_list->data->position.y;

			float distance = sqrt(pow((position.x - x), 2) + pow((position.y - y), 2));
			if (distance < collrange + entities_list->data->body)
			{
				colliding_entity_list->push_back(entities_list->data);

			}
			if (distance < vision + entities_list->data->body)
			{
				close_entity_list->push_back(entities_list->data);
			}
		}
		entities_list = entities_list->next;

	}
}

void Test_1::CheckCollisions(fPoint* speed)
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