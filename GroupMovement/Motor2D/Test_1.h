#ifndef __TEST_1_H__
#define __TEST_1_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "DynamicEnt.h"

enum states_test_1
{
	ST_TEST_1_IDLE,
	ST_TEST_1_MOVE,
	ST_TEST_1_DEAD
};

class Test_1 : public DynamicEnt
{
public:
	// Constructor
	Test_1(int posx, int posy);

	// Destructor
	~Test_1();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	bool CleanUp();
	// Animation
	void CheckAnimation(float dt);

public:
	// Animations
	states_test_1	actualState;
	Animation* current_animation = nullptr;
	//Animation idle;
	int attackrange;
	
	iPoint origin, mouse;
	int counter = 0;
	p2DynArray<iPoint> path;
	int followpath;
	list<j1Entity*> closer_entity_list;
	list<j1Entity*> colliding_entity_list;
};

#endif // __TEST_1_H__
