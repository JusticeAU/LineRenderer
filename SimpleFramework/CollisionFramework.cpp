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
	convexPoly = new ConvexPolygon(Vec2(-1, -1), 1, somePoints);
	convexPoly->m_colour = { 0,1,0 };
	shapes.push_back(convexPoly);

	std::vector<Vec2> someOtherPoints = std::vector<Vec2>();
	someOtherPoints.push_back(Vec2(0, 1));
	someOtherPoints.push_back(Vec2(1.8, 0.8));

	someOtherPoints.push_back(Vec2(1, -1));
	someOtherPoints.push_back(Vec2(-1, -1));
	convexPoly2 = new ConvexPolygon(Vec2(1.5, 1), 1, someOtherPoints);
	convexPoly2->m_colour = { 0,0,1 };
	shapes.push_back(convexPoly2);

	// Set up world border planes
	//shapes.push_back(new Plane({ 0,1 }, -10, { 1,1,1 }));
}

void CollisionFramework::Update(float delta)
{
	ConvexPolygon* poly1 = (ConvexPolygon*)convexPoly;
	ConvexPolygon* poly2 = (ConvexPolygon*)convexPoly2;

	std::vector<Vec2> normals = std::vector<Vec2>();

	for (int i = 0; i < poly1->m_points.size(); i++)
		normals.push_back(poly1->GetSurfaceNormal(i));
	for (int i = 0; i < poly2->m_points.size(); i++)
		normals.push_back(poly2->GetSurfaceNormal(i));


	Vec2 planeNormal = -normals[vertIndex];

	float distance = -4.0f;
	Plane plane = Plane(planeNormal, distance, { 1,1,1 });
	plane.Draw(*lines);


	Vec2 planeOrigin = planeNormal * distance;
	lines->DrawCircle(planeOrigin, 0.1f);
	Vec2 planePerpendicular = { -planeNormal.y, planeNormal.x };
	float poly1min = FLT_MAX;
	float poly1max = -FLT_MAX;
	float poly2min = FLT_MAX;
	float poly2max = -FLT_MAX;

	// check all of poly1s points
	for (int i = 0; i < poly1->m_points.size(); i++)
	{
		float point = glm::dot(poly1->GetVertexInWorldspace(i), planePerpendicular);
		poly1min = glm::min(poly1min, point);
		poly1max = glm::max(poly1max, point);
	}

	lines->DrawLineSegment(
		planeOrigin + (planePerpendicular * poly1min),
		planeOrigin + (planePerpendicular * poly1max),
		convexPoly->m_colour);

	// check all of poly2s points
	for (int i = 0; i < poly2->m_points.size(); i++)
	{
		float point = glm::dot(poly2->GetVertexInWorldspace(i), planePerpendicular);
		poly2min = glm::min(poly2min, point);
		poly2max = glm::max(poly2max, point);
	}

	lines->DrawLineSegment(
		planeOrigin + (planePerpendicular * poly2min),
		planeOrigin + (planePerpendicular * poly2max),
		convexPoly2->m_colour);

	float overlap = glm::min(poly1max - poly2min, poly2max - poly1min);
	if (overlap > 0.0f)
	{
		std::cout << overlap << std::endl;
		lines->DrawLineSegment(
			planeOrigin + (planePerpendicular * glm::min(poly1max, poly2min)),
			planeOrigin + (planePerpendicular * glm::max(poly1max, poly2min)),
			{ 1,0,0 });
	}
	
	if (leftMouseDown)
		poly1->m_position = cursorPos;

	if (rightMouseDown)
		poly2->m_position = cursorPos;

	// some cursed shit
	if (middleMouseDown && !indexing)
	{
		indexing = true;
		vertIndex += 1;
		if (vertIndex >= poly1->m_points.size() + poly2->m_points.size())
			vertIndex = 0;
	}
	else if (!middleMouseDown && indexing)
	{
		indexing = false;
	}


	
	// Update all primitives
	for (auto& shape : shapes)
		shape->Update(delta);

	//// Do whacky gravity stuff ehehe.
	//if (middleMouseDown)
	//{
	//	std::cout << "middle down" << std::endl;
	//	for (auto& shape : shapes)
	//	{
	//		Vec2 toCursor = cursorPos - shape->m_position;
	//		toCursor = glm::normalize(toCursor);
	//		shape->ApplyImpulse(toCursor * (1.0f / shape->m_inverseMass));
	//	}
	//}

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