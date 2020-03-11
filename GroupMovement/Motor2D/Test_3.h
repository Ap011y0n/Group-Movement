#ifndef __TEST_3_H__
#define __TEST_3_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "StaticEnt.h"

enum states_test_3
{
	ST_TEST_3_IDLE,
	ST_TEST_3_MOVE,
	ST_TEST_3_DEAD
};

class Test_3 : public StaticEnt
{
public:
	// Constructor
	Test_3(int posx, int posy);

	// Destructor
	~Test_3();

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
	states_test_3	actualState;
	Animation* current_animation = nullptr;
	//Animation idle;
	int attackrange;

};

#endif // __TEST_1_H__
