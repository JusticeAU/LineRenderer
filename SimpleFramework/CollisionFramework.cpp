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
	somePoints.push_back(Vec2(1.8, 0.8));

	somePoints.push_back(Vec2(1, -1));
	somePoints.push_back(Vec2(-1, -1));
	convexPoly2 = new ConvexPolygon(Vec2(1.5, 1), 1, somePoints);
	convexPoly2->m_colour = { 0,0,1 };
	shapes.push_back(convexPoly2);

	aabb = new Circle({ 0,0 }, 1.0f, 1.0f, { 0,1,0 });
	//shapes.push_back(circle);

	aabb = new AABB({ 4, -1.25 }, 1.2, 2.1, 1, {0,1,0});
	shapes.push_back(aabb);

	// Set up world border planes
	shapes.push_back(new Plane({ 0,1 }, -10, { 1,1,1 }));
}

void CollisionFramework::Update(float delta)
{
	//Draw
	for (auto s : shapes)
		s->Draw(*lines);

	// start SAT dev
	AABB* aabb1 = (AABB*)aabb;
	ConvexPolygon* poly2 = (ConvexPolygon*)convexPoly2;

	std::vector<Vec2> vertexDirections = std::vector<Vec2>();
	std::vector<Vec2> pointA = std::vector<Vec2>();
	std::vector<Vec2> pointB = std::vector<Vec2>();

	// Add the polys verts
	for (int i = 0; i < poly2->m_points.size(); i++)
	{
		vertexDirections.push_back(-poly2->GetVertexDirection(i));
		pointA.push_back(poly2->GetVertexInWorldspace(i));
		pointB.push_back(poly2->GetVertexInWorldspace(i+1));
	}
	
	Vec2* corners = aabb1->GetCorners();
	// Add the boxes verts
	vertexDirections.push_back({ -1,0 });
	pointA.push_back(corners[3]);
	pointB.push_back(corners[0]);
	vertexDirections.push_back({ 0,1 });
	pointA.push_back(corners[0]);
	pointB.push_back(corners[1]);
	vertexDirections.push_back({ 1,0 });
	pointA.push_back(corners[1]);
	pointB.push_back(corners[2]);
	vertexDirections.push_back({ 0,-1 });
	pointA.push_back(corners[2]);
	pointB.push_back(corners[3]);


	Vec2 renderPlaneNormal = -vertexDirections[vertIndex];
	lines->DrawLineSegment({ 0,0 }, renderPlaneNormal, {1,0,1});
	//Vec2 renderPlaneNormal = -glm::normalize(cursorPos);
	float renderPlaneDistance = -4.5f;
	Plane renderPlane = Plane(renderPlaneNormal, renderPlaneDistance, { 1,1,1 });
	renderPlane.Draw(*lines);


	// visulise one vert for debug
	Vec2 planeOrigin = renderPlaneNormal * renderPlaneDistance;
	//lines->DrawCircle(planeOrigin, 0.1f);
	Vec2 planePerpendicular = { -renderPlaneNormal.y, renderPlaneNormal.x };
	float poly1min = FLT_MAX;
	float poly1max = -FLT_MAX;
	float poly2min = FLT_MAX;
	float poly2max = -FLT_MAX;

	// check all of AABBs points
	for(int i = 0; i < 4; i++)
	{
		float point = glm::dot(corners[i], planePerpendicular);
		poly1min = glm::min(poly1min, point);
		poly1max = glm::max(poly1max, point);
	}

	lines->DrawLineSegment(
		planeOrigin + (planePerpendicular * poly1min),
		planeOrigin + (planePerpendicular * poly1max),
		aabb->m_colour);

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

	float overlapA = poly1max - poly2min;
	float overlapB = poly2max - poly1min;
	float overlap = glm::min(overlapA, overlapB);
	if (overlap > 0.0f)
	{
		//std::cout << overlap << std::endl;

		if (overlapA > overlapB)
		{
			lines->DrawLineSegment(
				planeOrigin + (planePerpendicular * glm::min(poly1min, poly1max)),
				planeOrigin + (planePerpendicular * glm::max(poly2min, poly2max)),
				{ 1,0,0 });
		}
		else
		{
			lines->DrawLineSegment(
				planeOrigin + (planePerpendicular * glm::max(poly1min, poly1max)),
				planeOrigin + (planePerpendicular * glm::min(poly2min, poly2max)),
				{ 1,0,0 });
		}
	}
	//std::cout << poly1max - poly2min << "\t" << poly2max - poly1min << std::endl;


	//lines->DrawCircle(planeOrigin + (planePerpendicular * poly1min), 0.1f, { 0, 0, 1 }); // min is blue 
	//lines->DrawCircle(planeOrigin + (planePerpendicular * poly1max), 0.1f, { 1, 0,0 }); // max is red

	//lines->DrawCircle(planeOrigin + (planePerpendicular * poly2min), 0.1f, { 0, 0, 1 }); // min is blue 
	//lines->DrawCircle(planeOrigin + (planePerpendicular * poly2max), 0.1f, { 1, 0,0 }); // max is red

	// Highlight the vert we're testing currently
	Vec2 a, b;
	a = pointA[vertIndex];
	b = pointB[vertIndex];

	// Draw the vert we're targetting
	lines->DrawLineSegment(a, b, { 1,1,1 });
		
	
	if (leftMouseDown)
		aabb->m_position = cursorPos;

	if (rightMouseDown)
		convexPoly2->m_position = cursorPos;

	// some cursed shit
	if (middleMouseDown && !indexing)
	{
		indexing = true;
		vertIndex += 1;
		if (vertIndex >= vertexDirections.size())
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