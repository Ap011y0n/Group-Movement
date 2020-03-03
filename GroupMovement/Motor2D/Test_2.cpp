#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Test_2.h"
#include "DynamicEnt.h"
#include "Brofiler/Brofiler.h"

Test_2::Test_2(int posx, int posy) : DynamicEnt(DynamicEntityType::TEST_2)
{
	name.create("test_1");

	actualState = ST_TEST_2_IDLE;
	speed = { 2, 0 };
	cost = 1;
	position.x = posx;
	position.y = posy;
	to_delete = false;
	// Load all animations
}

Test_2::~Test_2()
{}

bool Test_2::Start()
{

	return true;
}

bool Test_2::Update(float dt)
{
	BROFILER_CATEGORY("UpdateTest_2", Profiler::Color::BlanchedAlmond);

	CheckAnimation(dt);

	//App->render->Blit(App->entity->test_1_graphics, position.x + current_animation->pivotx[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), 1.0f);
	App->render->DrawQuad({ position.x, position.y, 10, 10 }, 200, 0, 0);
	position.x += speed.x;
	position.y += speed.y;
	if (position.x > 800)
		to_delete = true;
	return true;
}

bool Test_2::PostUpdate(float dt)
{
	BROFILER_CATEGORY("PostupdateTest_2", Profiler::Color::BurlyWood)

	return true;
}

void Test_2::CheckAnimation(float dt)
{

}