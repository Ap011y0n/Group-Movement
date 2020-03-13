#include "J1EntityManager.h"
#include "p2Defs.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "J1GroupMov.h"
#include "j1Pathfinding.h"
#include "j1Entity.h"

j1GroupMov::j1GroupMov() {
	name.create("entity");
}


j1GroupMov::~j1GroupMov() {

}

bool j1GroupMov::Awake(pugi::xml_node& config) {


	return true;
}

bool j1GroupMov::Start() {
	return true;

}
bool j1GroupMov::Update(float dt) {

	static iPoint origin, mouse;

	// TODO 0 ---------------------- Nothing to do here, just getting you in context
	// Every time we press leftclick button, we create a rect. The we check all entities with
	// selectable bool activated. If selectable entity is inside the rectangle, we turn their
	// isSelected bool to true
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		App->input->GetMousePosition(origin.x, origin.y);

	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		App->input->GetMousePosition(mouse.x, mouse.y);
		App->render->DrawQuad({ origin.x, origin.y, mouse.x - origin.x, mouse.y - origin.y }, 0, 200, 0, 100, false);
		App->render->DrawQuad({ origin.x, origin.y, mouse.x - origin.x, mouse.y - origin.y }, 0, 200, 0, 50);
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		list<j1Entity*>::iterator entities_list;
		j1Entity* it;
		for (entities_list = App->entity->entities.begin(); entities_list != App->entity->entities.end(); ++entities_list) {
			it = *entities_list;

			if (it->selectable)
			{
				it->isSelected = false;
				int x = it->position.x, y = it->position.y;
				// We store x and y positions
				// Now we check if it's inside the rect, so we can "select this entity"
				if (x > origin.x&& x < mouse.x) {
					if (y < origin.y && y > mouse.y)
					{
						it->isSelected = true;
					}
					else if (y > origin.y&& y < mouse.y)
					{
						it->isSelected = true;
					}
				}
				else if (x < origin.x && x > mouse.x) {
					if (y < origin.y && y > mouse.y)
					{
						it->isSelected = true;
					}
					else if (y > origin.y&& y < mouse.y)
					{
						it->isSelected = true;
					}
				}
			}
		}
	}

	return true;
}
bool j1GroupMov::PostUpdate(float dt) {
	return true;

}
bool j1GroupMov::CleanUp() {
	return true;
}

fPoint j1GroupMov::GetSeparationSpeed(list<j1Entity*>colliding_entity_list, fPoint position) 
{
	// TODO 4 Iterate all neighbours and add their relative position vectors in a FPoint.
	// Then divide it by the totalneighbours number to obtain the average vector
	// Before normalizing the resulting vector, remember to invert it multiplying by -1.
	// Do not normalize it if the norm is 0, instead return speed = 0

	//	REMEMBER norm = sqrt(var1^2 + var2^2);
	fPoint separationSpeed = { 0,0 };

	
	return separationSpeed;
}

fPoint j1GroupMov::GetCohesionSpeed(list<j1Entity*>close_entity_list, fPoint position)
{
	// TODO 5 Pretty much like before, we iterate all close neighbours
	// But there's an addition. We need another fPoint, the MassCenter, which will initially use this 
	// entity position
	// To calculate it we add all the neighbours positions and divide by the total neighbours number to get the average
	// Now, we can get that cohesion speed, using mass center as reference, calculate a vector that will attract them
	// Don't forget to normalize it
	
	fPoint cohesionSpeed = { 0,0 };

	return cohesionSpeed;
}

fPoint j1GroupMov::GetDirectionSpeed(list<j1Entity*>close_entity_list)
{
	//OPTIONAL TODO
	// We add the speed vectors of our neighbours, then divide it, and then normalize it
	// It's actually quite simple
	fPoint alignmentSpeed{ 0,0 };

	
	return alignmentSpeed;
}

