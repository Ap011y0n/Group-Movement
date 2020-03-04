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
	App->input->GetMousePosition(mouse.x, mouse.y);
	mouse = App->map->WorldToMap(mouse.x, mouse.y);

	

	//App->pathfinding->CreatePath(origin, mouse);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	return true;
}
bool j1GroupMov::PostUpdate(float dt) {
	return true;

}
bool j1GroupMov::CleanUp() {
	return true;

}
