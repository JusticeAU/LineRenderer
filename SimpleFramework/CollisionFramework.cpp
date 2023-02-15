#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"
#include "Spawner.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{
	//spawner = new Spawner(&shapes);

	//std::vector<Vec2> somePoints = std::vector<Vec2>();
	//somePoints.push_back(Vec2(0, 1));
	//somePoints.push_back(Vec2(1.8, 0.8));

	//somePoints.push_back(Vec2(1, -1));
	//somePoints.push_back(Vec2(-1, -1));
	//convexPoly2 = new ConvexPolygon(Vec2(1.5, 1), 1, somePoints);
	//convexPoly2->m_colour = { 0,0,1 };
	//shapes.push_back(convexPoly2);

	circle = new Circle({ 0,0 }, 1.0f, 1.0f, { 0,1,0 });
	shapes.push_back(circle);

	aabb = new AABB({ 4, -1.25 }, 1.2, 2.1, 1, {0,1,0});
	shapes.push_back(aabb);

	// Set up world border planes
	shapes.push_back(new Plane({ 0,1 }, -10, { 1,1,1 }));

	// Poly Spawner shit
	spawningVerts = std::vector<Vec2>();

}

void CollisionFramework::Update(float delta)
{
	//Draw
	for (auto s : shapes)
		s->Draw(*lines);


	// Poly Spawner shit
	// draw current poly
	lines->SetColour({ .8,.8,.8 });
	for (auto& point : spawningVerts)
		lines->AddPointToLine(point);
	lines->FinishLineStrip();
	if(spawningVerts.size() > 0)
	{
		Vec2 from = spawningVerts.back();
		if (glm::distance(cursorPos, spawningVerts.front()) < 0.2f && spawningVerts.size() > 2)
		{
			lines->DrawLineSegment(from, spawningVerts.front());
			lines->DrawCircle(spawningVerts.front(), 0.1);
		}
		else
		{
			// do some normal testing shenanigans here
			Vec2 desiredPoint = cursorPos;
			Vec2 lastEdgeNormalised, lastEdgePerpendicular, fromPos, lastVertToCursor;
			float dot, dotPerpendicular;
			Vec2 toHome, toHomePerpendicular;
			float dotPerpendicularToHome;

			toHome = glm::normalize(spawningVerts.front() - spawningVerts.back());
			toHomePerpendicular = { -toHome.y, toHome.x };

			if (spawningVerts.size() > 1)
				lastEdgeNormalised = glm::normalize(spawningVerts[spawningVerts.size() - 1] - spawningVerts[spawningVerts.size() - 2]);
			else
				lastEdgeNormalised = { 1,0 };

			lastEdgePerpendicular = { lastEdgeNormalised.y, -lastEdgeNormalised.x };
			fromPos = spawningVerts[spawningVerts.size() - 1];
			lastVertToCursor = cursorPos - spawningVerts.back();
			dot = glm::dot(lastVertToCursor, lastEdgeNormalised);
			dotPerpendicular = glm::dot(lastVertToCursor, lastEdgePerpendicular);
			dotPerpendicularToHome = glm::dot(lastVertToCursor, toHomePerpendicular);

			lines->SetColour({ .5,.5,.5 });
			if (dotPerpendicular < 0)
			{
				potentialVert = fromPos + (lastEdgeNormalised * dot);
				lines->DrawCircle(fromPos + (lastEdgeNormalised * dot), 0.1f);
				lines->DrawLineSegment(from, fromPos + (lastEdgeNormalised * dot));
			}
			else if (dotPerpendicularToHome < 0 && spawningVerts.size() > 2)
			{
				std::cout << dotPerpendicularToHome << std::endl;
				potentialVert = fromPos + (toHome * dotPerpendicular);
				lines->DrawCircle(fromPos + (toHome * dotPerpendicular), 0.1f);
				lines->DrawLineSegment(from, fromPos + (toHome * dotPerpendicular)); // Progress
			}
			else
			{
				potentialVert = cursorPos;
				lines->DrawLineSegment(from, cursorPos);
				lines->DrawCircle(cursorPos, 0.1);
			}

			lines->DrawLineSegment(fromPos, fromPos + lastEdgeNormalised);
			if (spawningVerts.size() > 2)
			{

				lines->DrawLineSegment(spawningVerts.back(), spawningVerts.back() + toHome);
			}

			lines->SetColour({ .8,.8,.8 });
		}
	}
	else
		lines->DrawCircle(cursorPos, 0.1);
		
	
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


	// Poly spawner stuff
	if (spawningVerts.size() > 0)
	{
		if (glm::distance(cursorPos, spawningVerts.front()) < 0.2f)
		{
			// Create the shape
			ConvexPolygon* poly = new ConvexPolygon({ 0,0 }, 1, spawningVerts, { 1,1,1 });
			spawningVerts.clear();
			shapes.push_back(poly);
		}
		else
		{
			spawningVerts.push_back(potentialVert);
		}
	}
	else
		spawningVerts.push_back(cursorPos);



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
}

void CollisionFramework::OnRightRelease()
{
	std::cout << "right release" << std::endl;
	
	if (spawner != nullptr)
		spawner->OnRightRelease();
}