#ifndef __TEST_1_H__
#define __TEST_1_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "DynamicEnt.h"

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

	void SaveNeighbours(list<j1Entity*>*, list<j1Entity*>*);

	private:

};

#endif // __TEST_1_H__
