#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include <vector>

class CollisionFramework : public Application
{
public:
	CollisionFramework();

	void Update(float delta) override;

	Circle* yourCircle = new Circle({ -5,0 }, 2.0f, 1.0f, { 1,0,0 });
	Plane* yourPlane = new Plane({0,0}, 1, {1, 1, 0});
	AABB* yourAABB = new AABB({ 0,0 }, 5, 2, 1.0f);
	std::vector<Shape*> shapes;
	bool rightDown = false;
	const int MAX_COLLISION_PASSES = 6;
};