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
	Center.pos.x = 0;
	Center.pos.y = 0;
	NewGroup = false;
	return true;

}
bool j1GroupMov::Update(float dt) {

	static iPoint origin, mouse;



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
		int totalSelected = 0;
	

		p2List_item<j1Entity*>* entities_list = App->entity->entities.start;
		while (entities_list)
		{
			if (entities_list->data->selectable)
			{

				entities_list->data->isSelected = false;
				int x = entities_list->data->position.x, y = entities_list->data->position.y;

				if (x > origin.x && x < mouse.x) {
					if (y < origin.y && y > mouse.y)
					{
						entities_list->data->isSelected = true;
					}
					else if (y > origin.y && y < mouse.y)
					{
						entities_list->data->isSelected = true;
					}
				}
				else if (x < origin.x && x > mouse.x) {
					if (y < origin.y && y > mouse.y)
					{
						entities_list->data->isSelected = true;
					}
					else if (y > origin.y && y < mouse.y)
					{
						entities_list->data->isSelected = true;
					}
				}

				if (entities_list->data->isSelected)
				{
					if (!NewGroup)
					{
						selected.clear();
					}
					NewGroup = true;
					

					selected.add(entities_list->data);
				}
			}
			entities_list = entities_list->next;
		}

		if (NewGroup)
		{
			NewGroup = false;
						
			
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
	j1Entity* it;
	list<j1Entity*>::iterator neighbours_it;
	fPoint separationSpeed = { 0,0 };

	for (neighbours_it = colliding_entity_list.begin(); neighbours_it != colliding_entity_list.end(); ++neighbours_it) {
		it = *neighbours_it;
		separationSpeed.x += position.x - it->position.x;
		separationSpeed.y += position.y - it->position.y;
	}

		separationSpeed.x = separationSpeed.x / colliding_entity_list.size();
		separationSpeed.y = separationSpeed.y / colliding_entity_list.size();

		float norm = sqrt(pow((separationSpeed.x), 2) + pow((separationSpeed.y), 2));

		if (norm != 0)
		{
			separationSpeed.x = separationSpeed.x / norm;
			separationSpeed.y = separationSpeed.y / norm;
		}

	
	return separationSpeed;
}

fPoint j1GroupMov::GetCohesionSpeed(list<j1Entity*>close_entity_list, fPoint position)
{
	fPoint cohesionSpeed = { 0,0 };
	fPoint MassCenter{ position.x, position.y };
	j1Entity* it;
	list<j1Entity*>::iterator neighbours_it;

	for (neighbours_it = close_entity_list.begin(); neighbours_it != close_entity_list.end(); ++neighbours_it) {
		it = *neighbours_it;
		MassCenter.x += it->position.x;
		MassCenter.y += it->position.y;
	}

		MassCenter.x = MassCenter.x / (close_entity_list.size() + 1);
		MassCenter.y = MassCenter.y / (close_entity_list.size() + 1);

		cohesionSpeed.x = position.x - MassCenter.x;
		cohesionSpeed.y = position.y - MassCenter.y;

		float norm = sqrt(pow((cohesionSpeed.x), 2) + pow((cohesionSpeed.y), 2));

		if (cohesionSpeed.x < 11 && cohesionSpeed.x > -11)
		{
			cohesionSpeed.x = 0;
		}
		else
		{
			cohesionSpeed.x = -1 * cohesionSpeed.x / norm;
		}
		if (cohesionSpeed.y < 11 && cohesionSpeed.y > -11)
		{
			cohesionSpeed.y = 0;
		}
		else
		{
			cohesionSpeed.y = -1 * cohesionSpeed.y / norm;
		}
	
	return cohesionSpeed;
}

fPoint j1GroupMov::GetDirectionSpeed(list<j1Entity*>close_entity_list)
{
	
	fPoint directionSpeed{ 0,0 };
	/*j1Entity* it;
	list<j1Entity*>::iterator neighbours_it;

	for (neighbours_it = close_entity_list.begin(); neighbours_it != close_entity_list.end(); ++neighbours_it) {

		it = *neighbours_it;

		directionSpeed.x += it->direction.x;

		directionSpeed.y += it->direction.x;
	}

	if (!close_entity_list.empty())

	{
		directionSpeed.x = directionSpeed.x / close_entity_list.size();

		directionSpeed.y = directionSpeed.y / close_entity_list.size();

		float norm = sqrt(pow((directionSpeed.x), 2) + pow((directionSpeed.y), 2));

		if (norm != 0)

		{
			directionSpeed.x = directionSpeed.x / norm;
			directionSpeed.y = directionSpeed.y / norm;
		}

	}*/
	return directionSpeed;
}