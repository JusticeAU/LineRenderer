#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{
	shapes.push_back(yourCircle);
	shapes.push_back(yourPlane);

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
		shapes.push_back(new Circle(cursorPos, 1, 1.0f, {r,g,b}));
	}
	else if (!rightMouseDown && rightDown)
		rightDown = false;

	// Line Debug
	yourPlane->m_normal = glm::normalize(cursorPos);
	yourPlane->m_distance = glm::length(cursorPos);

	// Perform collision tests and resolution 1 pass against all shapes.
	if (shapes.size() >= 2)
	{
		for (int i = 0; i < MAX_COLLISION_PASSES; i++)
		{
			std::vector<CollisionData> collisions;
			for (int a = 0; a+1 < shapes.size(); a++)
			{
				for (int b = a + 1; b < shapes.size(); b++)
				{
					CollisionData col = TestCollisions(shapes[a], shapes[b]);
					if (col.IsCollision())
						collisions.push_back(col);
				}
			}

			// if there are no collisions then we can bail out of any further passes
			if (collisions.empty())
				break;
			else
			{
				for (auto& col : collisions)
				{
					col.Resolve();
				}
			}
		}
	}

	//Draw
	for (auto s : shapes)
	{
		s->Draw(*lines);
	}

	AABB aabb = AABB({0,0}, 5, 2, 1.0f);
	aabb.Draw(*lines);

	CollisionData col = CircleOnAABB(yourCircle, &aabb);

	lines->DrawCross(col.worldPosition, 0.1f);
	lines->DrawLineSegment(col.worldPosition, col.worldPosition + (col.normal * col.depth));

	/*Vec2 closestPoint = aabb.GetClosestPoint(cursorPos);
	lines->DrawCross(closestPoint, 0.1f);*/
}