#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"
#include "Spawner.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{
	//spawner = new Spawner(&shapes);

	std::vector<Vec2> somePoints = std::vector<Vec2>();
	somePoints.push_back(Vec2(0, 1));
	somePoints.push_back(Vec2(1, -1));
	somePoints.push_back(Vec2(-1, -1));
	convexPoly = new ConvexPolygon(Vec2(0, 0), 1, somePoints);
	shapes.push_back(convexPoly);

	std::vector<Vec2> someOtherPoints = std::vector<Vec2>();
	someOtherPoints.push_back(Vec2(0, 1));
	someOtherPoints.push_back(Vec2(1, -1));
	someOtherPoints.push_back(Vec2(-1, -1));
	convexPoly2 = new ConvexPolygon(Vec2(0, 0), 1, someOtherPoints);
	shapes.push_back(convexPoly2);

	// Set up world border planes
	//shapes.push_back(new Plane({ 0,1 }, -10, { 1,1,1 }));
}

void CollisionFramework::Update(float delta)
{
	Vec2 planeNormal = -glm::normalize(cursorPos);
	float distance = -glm::length(cursorPos);
	Plane plane = Plane(planeNormal, distance, { 1,1,1 });
	plane.Draw(*lines);

	//std::vector<Vec2> pointsToDraw = std::vector<Vec2>();

	ConvexPolygon* butActually = (ConvexPolygon*)convexPoly;
	ConvexPolygon* butActually2 = (ConvexPolygon*)convexPoly2;

	Vec2 planeOrigin = planeNormal * distance;
	lines->DrawCircle(planeOrigin, 0.1f);
	Vec2 planePerpendicular = { -planeNormal.y, planeNormal.x };
	for (int i = 0; i < butActually->m_points.size(); i++)
	{
		float pointToDraw = glm::dot(butActually->GetVertexInWorldspace(i), planePerpendicular);
		lines->SetColour({ 1,0,0 });
		lines->DrawCircle(planeOrigin + (planePerpendicular * pointToDraw), 0.1f);
		lines->SetColour({ 1,1,1 });
		lines->DrawLineSegment(butActually->GetVertexInWorldspace(i), planeOrigin + (planePerpendicular * pointToDraw));

		pointToDraw = glm::dot(butActually2->GetVertexInWorldspace(i), planePerpendicular);
		lines->SetColour({ 0,1,0 });
		lines->DrawCircle(planeOrigin + (planePerpendicular * pointToDraw), 0.1f);
		lines->SetColour({ 1,1,1 });
		lines->DrawLineSegment(butActually2->GetVertexInWorldspace(i), planeOrigin + (planePerpendicular * pointToDraw));
	}
	
	if (leftMouseDown)
		butActually->m_position = cursorPos;

	if (rightMouseDown)
		butActually2->m_position = cursorPos;



	
	// Update all primitives
	for (auto& shape : shapes)
		shape->Update(delta);

	// Do whacky gravity stuff ehehe.
	if (middleMouseDown)
	{
		std::cout << "middle down" << std::endl;
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

	//Draw
	for (auto s : shapes)
		s->Draw(*lines);

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