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
	position = { 0,0 };

	j1Entity* it;
	list<j1Entity*>::iterator childs_list;
	
	for (childs_list = childs.begin(); childs_list != childs.end(); ++childs_list) {
		it = *childs_list;
		position.x += it->position.x;
		position.y += it->position.y;
	}

	
	if (!childs.empty())
	{
		position.x = position.x / childs.size();
		position.y = position.y / childs.size();
		LOG("%d, %d",position.x, position.y);
	}
	else {
		to_delete = true;
	}

	if(isSelected)
		App->render->DrawCircle(position.x, position.y, 10, 0, 0, 200, 150);
	else {
		App->render->DrawCircle(position.x, position.y, 10, 200, 0, 200, 150);
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

list<j1Entity*>* Test_3::ReturnChilds()
{
	return &childs;
}