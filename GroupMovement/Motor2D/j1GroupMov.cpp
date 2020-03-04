#include "J1EntityManager.h"
#include "p2Defs.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "J1GroupMov.h"
#include "j1Pathfinding.h"

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
		p2List_item<j1Entity*>* entities_list = App->entity->entities.start;
		while (entities_list)
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
			entities_list = entities_list->next;
		}
	}
	

	//App->pathfinding->CreatePath(origin, mouse);

//	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	return true;
}
bool j1GroupMov::PostUpdate(float dt) {
	return true;

}
bool j1GroupMov::CleanUp() {
	return true;
}
