#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"
#include "Spawner.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{
	spawner = new Spawner(&shapes);

	//std::vector<Vec2> somePoints = std::vector<Vec2>();
	//somePoints.push_back(Vec2(0, 1));
	//somePoints.push_back(Vec2(1.8, 0.8));

	//somePoints.push_back(Vec2(1, -1));
	//somePoints.push_back(Vec2(-1, -1));
	//convexPoly2 = new ConvexPolygon(Vec2(1.5, 1), 1, somePoints);
	//convexPoly2->m_colour = { 0,0,1 };
	//shapes.push_back(convexPoly2);

	//aabb = new Circle({ 0,0 }, 1.0f, 1.0f, { 0,1,0 });
	////shapes.push_back(circle);

	//aabb = new AABB({ 4, -1.25 }, 1.2, 2.1, 1, {0,1,0});
	//shapes.push_back(aabb);

	// Set up world border planes
	shapes.push_back(new Plane({ 0,1 }, -10, { 1,1,1 }));
}

void CollisionFramework::Update(float delta)
{
	//Draw
	for (auto s : shapes)
		s->Draw(*lines);
		
	
	// Update all primitives
	for (auto& shape : shapes)
		shape->Update(delta);

	// Do whacky gravity stuff ehehe.
	if (middleMouseDown)
	{
		for (auto& shape : shapes)
		{
			Vec2 toCursor = cursorPos - shape->m_position;
			toCursor = glm::normalize(toCursor);
			shape->ApplyImpulse(toCursor * (1.0f / shape->m_inverseMass));
		}
	}

	// Perform collision tests and resolution passes against all shapes.
	for (int i = 0; i < MAX_COLLISION_PASSES; i++)
	{
		std::vector<CollisionData> collisions;
		for (int a = 0; a+1 < shapes.size(); a++)
		{
			for (int b = a + 1; b < shapes.size(); b++)
			{
				CollisionData col = TestCollisions(shapes[a], shapes[b]);
				//lines->DrawCross(col.worldPosition, 1.0f);
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
				col.Resolve();
		}
	}



	// Handle spawner if we've created it.
	if (spawner != nullptr)
	{
		spawner->Update(delta, cursorPos);
		spawner->Draw(*lines);
	}
}

void CollisionFramework::OnLeftClick()
{
	std::cout << "left down" << std::endl;
	
	if(spawner != nullptr)
		spawner->OnLeftClick(cursorPos);

	//SAT Spawner
	///*std::vector<Vec2> somePoints = std::vector<Vec2>();
	//somePoints.push_back(Vec2(0, 1));
	//somePoints.push_back(Vec2(1, -1));
	//somePoints.push_back(Vec2(-1, -1));
	//ConvexPolygon* poly = new ConvexPolygon(Vec2(-1, -1), 1, somePoints);*/

	//Circle* circle = new Circle(cursorPos, 1, 1);
	////poly->m_position = cursorPos;
	//circle->m_velocity = -glm::normalize(cursorPos);
	//shapes.push_back(circle);

}

void CollisionFramework::OnLeftRelease()
{
	std::cout << "left release" << std::endl;
	
	if (spawner != nullptr)
		spawner->OnLeftRelease();
}

void CollisionFramework::OnRightClick()
{
	std::cout << "right click" << std::endl;

	//SAT Spawner
	/*std::vector<Vec2> somePoints = std::vector<Vec2>();
	somePoints.push_back(Vec2(0, 1));
	somePoints.push_back(Vec2(1.8, 0.8));

	somePoints.push_back(Vec2(1, -1));
	somePoints.push_back(Vec2(-1, -1));
	ConvexPolygon* poly = new ConvexPolygon(Vec2(-1, -1), 1, somePoints);
	poly->m_position = cursorPos;
	poly->m_velocity = -glm::normalize(cursorPos);
	shapes.push_back(poly);*/
}

void CollisionFramework::OnRightRelease()
{
	std::cout << "right release" << std::endl;
	
	if (spawner != nullptr)
		spawner->OnRightRelease();
}