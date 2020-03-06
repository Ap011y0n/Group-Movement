#ifndef __TEST_2_H__
#define __TEST_2_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "DynamicEnt.h"

enum states_Test_2
{
	ST_TEST_2_IDLE,
	ST_TEST_2_MOVE,
	ST_TEST_2_DEAD
};

class Test_2 : public DynamicEnt
{
public:
	// Constructor
	Test_2(int posx, int posy);

	// Destructor
	~Test_2();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Animation
	void CheckAnimation(float dt);

public:
	// Animations
	states_Test_2	actualState;
	Animation* current_animation;
	//Animation idle;
	int attackrange;

};

#endif // __TEST_1_H__
