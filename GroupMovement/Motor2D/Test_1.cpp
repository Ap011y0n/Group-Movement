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
#include <math.h>

Test_1::Test_1(int posx, int posy) : DynamicEnt(DynamicEntityType::TEST_1)
{
	name.create("test_1");

	actualState = ST_TEST_1_IDLE;
	speed = { 0, 0 };
	cost = 5;
	vision = 20;
	collrange = 10;
	body = 7;
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
	separationSpeed = { 0, 0 };
	cohesion = { 0, 0 };
	CheckAnimation(dt);

	origin = App->map->WorldToMap(position.x, position.y);

	


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
		
		
			//This makes a comparison with the players position to make the correct move
			if (path.At(followpath)->x < origin.x) {
				pathSpeed.x -= 1;
			}

			if (path.At(followpath)->x > origin.x) {
				pathSpeed.x += 1;
			}

			if (path.At(followpath)->y < origin.y) {
				pathSpeed.y -= 1;
			}

			if (path.At(followpath)->y > origin.y) {
				pathSpeed.y += 1;
			}
			if (origin.x == path.At(followpath)->x && origin.y == path.At(followpath)->y)
			{
				followpath++;
			}
				
		
	}

	j1Entity* it;
	list<j1Entity*>::iterator neighbours_it;

	//----------------------------------------------------------------Save neighbours in two lists

	p2List_item<j1Entity*>* entities_list = App->entity->entities.start;
	closer_entity_list.clear();
	colliding_entity_list.clear();
	while (entities_list)
	{
		if (entities_list->data != this && entities_list->data->selectable)
		{
			int x = entities_list->data->position.x;
			int y = entities_list->data->position.y;

			float distance = sqrt(pow((position.x - x), 2) + pow((position.y - y), 2));
			if (distance < collrange + entities_list->data->body)
			{
				colliding_entity_list.push_back(entities_list->data);
				
			}
			if (distance < vision + entities_list->data->body)
			{
				closer_entity_list.push_back(entities_list->data);
			}
		}
		entities_list = entities_list->next;

	}

	//---------------------------------------------------------------- Separation Speed

	for (neighbours_it = colliding_entity_list.begin(); neighbours_it != colliding_entity_list.end(); ++neighbours_it) {
		it = *neighbours_it;
		separationSpeed.x += position.x - it->position.x;
		separationSpeed.y += position.y - it->position.y;
	}
	

	if (!colliding_entity_list.empty())
	{
		separationSpeed.x = separationSpeed.x / colliding_entity_list.size();
		separationSpeed.y = separationSpeed.y / colliding_entity_list.size();
	
	float norm = sqrt(pow((separationSpeed.x), 2) + pow((separationSpeed.y), 2));
	separationSpeed.x = separationSpeed.x / norm;
	separationSpeed.y = separationSpeed.y / norm;
	}
	//App->render->DrawCircle((int)position.x + 5, (int)position.y + 5, collrange, 200, 200, 0, 200);
	//App->render->DrawCircle((int)position.x + 5, (int)position.y + 5, body, 0, 200, 200, 200);

	//---------------------------------------------------------------- Cohesion speed
	
	fPoint MassCenter{ position.x, position.y };

	for (neighbours_it = closer_entity_list.begin(); neighbours_it != closer_entity_list.end(); ++neighbours_it) {
		it = *neighbours_it;
		MassCenter.x += it->position.x;
		MassCenter.y += it->position.y;
	}


	if (!closer_entity_list.empty())
	{
		MassCenter.x = MassCenter.x / (closer_entity_list.size()+1);
		MassCenter.y = MassCenter.y / (closer_entity_list.size()+1);

		cohesion.x = position.x - MassCenter.x;
		cohesion.y = position.y - MassCenter.y;
		float norm = sqrt(pow((cohesion.x), 2) + pow((cohesion.y), 2));

		if (cohesion.x < 11 && cohesion.x > -11)
		{
			cohesion.x = 0;
		}
		else
		{
		
			cohesion.x = -1 * cohesion.x / norm;
		}
		if (cohesion.y < 11 && cohesion.y > -11)
		{
			cohesion.y = 0;
		}
		else
		{
		
			cohesion.y = -1 * cohesion.y / norm;
		}
	}
	
	//App->render->DrawCircle((int)MassCenter.x, (int)MassCenter.y, vision, 200, 200, 0, 200);

	//---------------------------------------------------------------- Add all speeds
	speed.x += 1.5*pathSpeed.x + 1*separationSpeed.x + 0.5 *cohesion.x;
	speed.y += 1.5*pathSpeed.y + 1*separationSpeed.y + 0.5 *cohesion.y;
	
	//------------------------------------------------------------------Use a collision system

	iPoint coord;
	p2List_item<MapLayer*>* layer_iterator = App->map->data.layers.start;
	MapLayer* layer = App->map->data.layers.start->data;

	while (layer_iterator != NULL) 
	{
		layer = layer_iterator->data;
		if (layer->returnPropValue("Navigation") == 1) {
			coord = App->map->WorldToMap(position.x + speed.x, position.y);
			if(coord.x < 10000 && coord.x > -10000)
			{
				if (layer->Get(coord.x, coord.y) != 0) speed.x = 0;
			}
			if (coord.x < 10000 && coord.x > -10000)
			{
				coord = App->map->WorldToMap(position.x, position.y + speed.y);
				if (layer->Get(coord.x, coord.y) != 0) speed.y = 0;
			
			}
		}
		layer_iterator = layer_iterator->next;
	}

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

	return true;
}