#include "CollisionFramework.h"
#include "CollisionData.h"

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
		shapes.push_back(new Circle(cursorPos, 1));
	}
	else if (!rightMouseDown && rightDown)
		rightDown = false;

	// Perform collision tests and resolution 1 pass against all shapes.
	if (shapes.size() >= 2)
	{
		for (int i = 0; i < MAX_COLLISION_PASSES; i++)
		{
			for (int a = 0; a < shapes.size(); a++)
			{
				for (int b = a + 1; b < shapes.size(); b++)
				{
					CollisionData col = shapes[a]->CollisionWithCircle(shapes[b]);
					col.Resolve();
					col.DebugDraw(*lines);
				}
			}
		}
	}


	//Draw
	for (auto s : shapes)
	{
		s->Draw(*lines);
	}

}