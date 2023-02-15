#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include <vector>

class Spawner;

class CollisionFramework : public Application
{
public:
	CollisionFramework();

	void Update(float delta) override;
	void OnLeftClick() override;
	void OnLeftRelease() override;

	void OnRightClick() override;
	void OnRightRelease() override;

	std::vector<Shape*> shapes;
	bool rightDown = false;
	const int MAX_COLLISION_PASSES = 6;

	Spawner* spawner;
	Shape* convexPoly;
	Shape* convexPoly2;
	Shape* circle;
	Shape* aabb;

	int vertIndex = 0;
	bool indexing = false;

	// Poly Spawner shit
	std::vector<Vec2> spawningVerts;
	Vec2 potentialVert;
};