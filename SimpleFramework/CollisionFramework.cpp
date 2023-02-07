#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"

CollisionFramework::CollisionFramework()
{
	shapeTemplates.push_back(new Circle(cursorPos, 1, 1.0f, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new Plane(cursorPos, 1, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new AABB(cursorPos, 1, 1.0f, 1, { 0.2f,0.2f,0.2f }));

}

void CollisionFramework::Update(float delta)
{

	//if (leftMouseDown)
	
	if (rightMouseDown && !rightDown)
	{
		/*rightDown = true;
		float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		shapes.push_back(new Circle(cursorPos, 1, 1.0f, {r,g,b}));*/
		shapes.push_back(shapeTemplates[templateIndex]);
	}
	else if (!rightMouseDown && rightDown)
		rightDown = false;

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

	// Draw template
	if (shapeTemplates[templateIndex]->GetShape() == SHAPE::PLANE)
	{
		Plane* plane = static_cast<Plane*>(shapeTemplates[templateIndex]);
		plane->m_normal = glm::normalize(cursorPos);
		plane->m_distance = glm::length(cursorPos);
	}
	else
		shapeTemplates[templateIndex]->m_position = cursorPos;
	shapeTemplates[templateIndex]->Draw(*lines);
}

void CollisionFramework::OnLeftRelease()
{
	templateIndex += 1;
	if (templateIndex == (int)SHAPE::COUNT)
		templateIndex = 0;
}