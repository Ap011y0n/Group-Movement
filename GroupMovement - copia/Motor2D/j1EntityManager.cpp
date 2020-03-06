#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Test_1.h"
#include "Test_2.h"
#include "Test_3.h"

#include "j1App.h"
#include<stdio.h>
#include "p2Log.h"
#include "j1Textures.h"
#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.create("entity_manager");
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading EntityManager");

	

	return true;
}

bool j1EntityManager::Start()
{


	return true;
}

bool j1EntityManager::CleanUp()
{
	p2List_item<j1Entity*>* entities_list = entities.start;

	while (entities_list != nullptr)
	{
		entities_list->data->CleanUp();
		RELEASE(entities_list->data);
		entities_list = entities_list->next;
	}
	entities.clear();

	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("UpdateEntity", Profiler::Color::Bisque);

	p2List_item<j1Entity*>* entities_list = entities.start;
	while (entities_list)
	{
		entities_list->data->Update(dt);
		entities_list = entities_list->next;
	}

	return true;
}

bool j1EntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("PostupdateEntity", Profiler::Color::Azure)


		p2List_item<j1Entity*>* entities_list = entities.start;
	while (entities_list)
	{
		if (entities_list->data->to_delete == true)
		{
			DeleteEntity(entities_list->data);
		}
		else {
			entities_list->data->PostUpdate(dt);
		}
			  
		entities_list = entities_list->next;
	}

		return true;
}

j1Entity* j1EntityManager::CreateEntity(DynamicEnt::DynamicEntityType type, int posx, int posy)
{
	j1Entity* ret = nullptr;

	switch (type)
	{
	case DynamicEnt::DynamicEntityType::TEST_1: ret = new Test_1(posx, posy); break;
	case DynamicEnt::DynamicEntityType::TEST_2: ret = new Test_2(posx, posy); break;

	}

	if (ret != nullptr)
	{
		entities.add(ret);
		entities.end->data->Start();
	}
	return ret;
}

j1Entity* j1EntityManager::CreateStaticEntity(StaticEnt::StaticEntType type, int posx, int posy)
{
	j1Entity* ret = nullptr;

	switch (type)
	{
	case StaticEnt::StaticEntType::TEST_3: ret = new Test_3(posx, posy); break;

	}

	if (ret != nullptr)
	{
		entities.add(ret);
		entities.end->data->Start();
	}
	return ret;
}

bool j1EntityManager::DeleteEntity(j1Entity* entity)
{
	entity->CleanUp();
	entities.del(entities.At(entities.find(entity)));

	return true;
}