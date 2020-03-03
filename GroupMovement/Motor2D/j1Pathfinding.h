#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255



class j1PathFinding : public j1Module
{
public:

	j1PathFinding();


	~j1PathFinding();

	bool CleanUp();


	void SetMap(uint width, uint height, uchar* data);

	
	int CreatePath(const iPoint& origin, const iPoint& destination);

	
	const p2DynArray<iPoint>* GetLastPath() const;

	
	bool CheckBoundaries(const iPoint& pos) const;


	bool IsWalkable(const iPoint& pos) const;

	
	uchar GetTileAt(const iPoint& pos) const;

private:
	uint width;
	uint height;
	uchar* map;
	p2DynArray<iPoint> last_path;
};

struct PathList;


struct PathNode
{
	PathNode();
	PathNode(int g, int h, const iPoint& pos, PathNode* parent);
	PathNode(const PathNode& node);
	uint FindWalkableAdjacents(PathList& list_to_fill);
	int Score() const;
	int CalculateF(const iPoint& destination);
	int g;
	int h;
	iPoint pos;
	PathNode* parent; 
};


struct PathList
{
	p2List_item<PathNode>* Find(const iPoint& point) const;
	p2List_item<PathNode>* GetNodeLowestScore() const;
	p2List<PathNode> list;
};



#endif // __j1PATHFINDING_H__