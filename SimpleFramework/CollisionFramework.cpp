#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{
	shapes.push_back(yourCircle);
}

void CollisionFramework::Update(float delta)
{

	if (leftMouseDown)
		yourCircle->m_position = cursorPos;
	
	if (rightMouseDown && !rightDown)
	{
		rightDown = true;
		float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		shapes.push_back(new Circle(cursorPos, 1, {r,g,b}));
	}
	else if (!rightMouseDown && rightDown)
		rightDown = false;

	// Perform collision tests and resolution 1 pass against all shapes.
	if (shapes.size() >= 2)
	{
		for (int i = 0; i < MAX_COLLISION_PASSES; i++)
		{
			for (int a = 0; a < shapes.size()-1; a++)
			{
				for (int b = a + 1; b < shapes.size(); b++)
				{
					Circle* circleA = dynamic_cast<Circle*>(shapes[a]);
					Circle* circleB = dynamic_cast<Circle*>(shapes[b]);

					CollisionData col = CircleOnCircle(*circleA, *circleB);
					col.Resolve();
					col.DebugDrawCircleInfo(*lines);
				}
			}
		}
	}


	//Draw
	for (auto s : shapes)
	{
		s->Draw(*lines);
	}

	/*AABB aabb = AABB({0,0}, 5, 2);
	aabb.Draw(*lines);*/

	// Line Debug
	Vec2 normal = glm::normalize(cursorPos);
	float distance = glm::length(cursorPos);
	Plane plane(normal, distance, {1, 1, 0});
	plane.Draw(*lines);

	CollisionData colTest = CircleOnPlane(*yourCircle, plane);
	if (colTest.IsCollision())
		lines->SetColour({ 1,0,0 });
	else
		lines->SetColour({ 0,1,0 });

	lines->DrawLineSegment(
		colTest.worldPosition,
		colTest.worldPosition + plane.m_normal * colTest.depth);

}