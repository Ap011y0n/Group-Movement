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
	
	//Speed is resetted to 0 each iteration
	speed = { 0, 0 };
	origin = App->map->WorldToMap(position.x, position.y);
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT)
		to_delete = true;

	// TODO 1 ------------------------------------------------------------------
	// If this entity is selected, it will create a path to the mouse position
	// We need to store a path in this entity dynamic array: path
	// First, order the pathfinding module to create a path with origin and mouse (destination);
	// Luckily for you i already created a method in pathfinding module that can do the trick
	// Just use SavePath() fucntion, and proceed to the next path

	if (isSelected && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		App->input->GetMousePosition(mouse.x, mouse.y);
		mouse = App->map->WorldToMap(mouse.x, mouse.y);
	
		App->pathfinding->CreatePath(origin, mouse);
		App->pathfinding->SavePath(&path);

		followpath = 1;			//Don't mind me, i'm just tracking the path
	}

	// TODO 2 ----------------------------------------------------------------
	// We need to give a speed to the entity to actually follow the path. All the logic is done,
	// just use the fPoint  PathSpeed and give it positive or negative value dependig on witch tile
	// is the next in the path. 

	fPoint pathSpeed{ 0,0 };
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

	// TODO 3 ----------------------------------------------------------------
	// Before we calculate the other speeds, we need to store this entity neighbours in two lists. 
	// Close neighbours (those which are near this entity) and colliding, thos which are in contact with this entity
	// To do that, we'll use three radius, vision, body and collrange;
	// Use SaveNeighbours function, and pass the two lists as reference

	SaveNeighbours(&close_entity_list, &colliding_entity_list);

	//TODO 4 ---------------------------------------------------------------- 
	// Now we should be are ready to get a new speed in the formula. The separation speed
	// Create an fPoint, just like PathSpeed, to store this new values.
	// Use GetSeparationSpeed method from movement module, and call it if there's at least one member in colliding list, if not, separationSpeed should be 0
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
	

	// TODO 5 ---------------------------------------------------------------- 
	// Cohesion speed
	// Just like before, but using GetCohesionSpeed method, close list and another fPoint
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

	// OPTIONAL TODO ---------------------------------------------------------------- 
	// Direction speed
	// We'll use another fpoint, another method (GetDirectionSpeed()) and the close list
	fPoint directionSpeed;
	if (!close_entity_list.empty())
	{
		directionSpeed = App->movement->GetDirectionSpeed(close_entity_list);
	}
	else
	{
		directionSpeed.x = 0;
		directionSpeed.y = 0;
	}


	//TODO 2 ---------------------------------------------------------------- 
	// We need to add all new speed to the speed vector. Add pathSpeed, but everytime you calculate another speed
	// add it here
	// To obtain different results, try using some constants to multiply each speed

	speed.x += 1.5*pathSpeed.x + 1*separationSpeed.x + 0.5 *cohesionSpeed.x + 0.1*directionSpeed.x;
	speed.y += 1.5*pathSpeed.y + 1*separationSpeed.y + 0.5 *cohesionSpeed.y + 0.1*directionSpeed.y;
	LOG("%f", directionSpeed.x);
	// TODO 6 ------------------------------------------------------------------
	// If you got this far, congratulations, now your entities react between themselves
	// But don't forget about walls, use a preventive collision system, so in case it is needed
	// total speed value should be set to 0.
	// I provided you a simple collision detection method in DynamicEnt class, just call it with the speed fpoint

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


bool Test_1::CleanUp()
{
	close_entity_list.clear();
	colliding_entity_list.clear();
	path.Clear();
	name.Clear();
	return true;
}

void Test_1::SaveNeighbours(list<j1Entity*>* close_entity_list, list<j1Entity*>* colliding_entity_list)
{
	//	TODO 3 -------------------------------------------------
	//  First, clear both lists before adding new members to them
	//  Then we should be iterating all entities in entity manager except for this entity
	//	Using the formulas, store those in vision range to close list, and those in collision range in colliding list
	list<j1Entity*>::iterator entities_list;
	j1Entity* it;
	colliding_entity_list->clear();
	close_entity_list->clear();

	for (entities_list = App->entity->entities.begin(); entities_list != App->entity->entities.end(); ++entities_list) {
		it = *entities_list;
		if (it != this && it->selectable)
		{
			int x = it->position.x;
			int y = it->position.y;

			float distance = sqrt(pow((position.x - x), 2) + pow((position.y - y), 2));
			if (distance < collrange + it->body)
			{
				colliding_entity_list->push_back(it);

			}
			if (distance < vision + it->body)
			{
				close_entity_list->push_back(it);
			}
		}
	}
}

