#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Test_3.h"
#include "StaticEnt.h"
#include "Brofiler/Brofiler.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

Test_3::Test_3(int posx, int posy) : StaticEnt( StaticEntType::TEST_3)
{
	name.create("test_1");

	actualState = ST_TEST_3_IDLE;
	cost = 1;
	position.x = posx;
	position.y = posy;
	to_delete = false;
	selectable = false;
	// Load all animations
}

Test_3::~Test_3()
{}

bool Test_3::Start()
{
	move = false;
	return true;
}

bool Test_3::Update(float dt)
{
	BROFILER_CATEGORY("UpdateTest_1", Profiler::Color::BlanchedAlmond);

	CheckAnimation(dt);

	//App->render->Blit(App->entity->test_1_graphics, position.x + current_animation->pivotx[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), 1.0f);



	
	if (isSelected && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		App->input->GetMousePosition(mouse.x, mouse.y);
		mouse = App->map->WorldToMap(mouse.x, mouse.y);
		move = true;
	}

	origin = App->map->WorldToMap(position.x, position.y);
	if (move)
	{


		App->pathfinding->CreatePath(origin, mouse);

		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint nextPoint = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			if (i == 0)
				LOG("%d %d", nextPoint.x, nextPoint.y);
			{
				//App->render->Blit(App->scene->debug_tex, nextPoint.x, nextPoint.y);
				App->render->DrawQuad({ nextPoint.x + 14, nextPoint.y + 14, 6, 6 }, 200, 0, 0, 100);
			}
		}
		if (path->At(1) != NULL)
		{
			//This makes a comparison with the players position to make the correct move
			if (path->At(1)->x < origin.x) {
				position.x -= 2;
			}

			if (path->At(1)->x > origin.x) {
				position.x += 2;
			}

			if (path->At(1)->y < origin.y) {
				position.y -= 2;
			}

			if (path->At(1)->y > origin.y) {
				position.y += 2;
			}
		}
	}




	if (childs.count() == 0)
		to_delete = true;
	if(isSelected)
	App->render->DrawQuad({ position.x, position.y, 20, 20 }, 0, 0, 200);
	else {
		App->render->DrawQuad({ position.x, position.y, 20, 20 }, 200, 0, 200);
	}
	return true;
}

bool Test_3::PostUpdate(float dt)
{
	BROFILER_CATEGORY("PostupdateTest_1", Profiler::Color::BurlyWood)

	return true;
}

void Test_3::CheckAnimation(float dt)
{

}

bool Test_3::CleanUp()
{

	return true;
}

p2List<j1Entity*>* Test_3::ReturnChilds()
{
	return &childs;
}