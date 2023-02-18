#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"
#include "Spawner.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{
	spawner = new Spawner(&shapes);

	/*circle = new Circle({ 0,0 }, 1.0f, 1.0f, { 0,1,0 });
	shapes.push_back(circle);

	aabb = new AABB({ 4, -1.25 }, 1.2, 2.1, 1, {0,1,0});
	shapes.push_back(aabb);*/

	// Set up world border planes
	shapes.push_back(new Plane({ 0,1 }, -9, { 1,1,1 }));
	shapes.push_back(new Plane({ 1,0 }, -16, { 1,1,1 }));
	shapes.push_back(new Plane({ 0,-1 }, -9, { 1,1,1 }));
	shapes.push_back(new Plane({ -1,0 }, -16, { 1,1,1 }));


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
	//lines->SetColour({ .8,.8,.8 });
	//for (auto& point : spawningVerts)
	//	lines->AddPointToLine(point);
	//lines->FinishLineStrip();
	//if(spawningVerts.size() > 0)
	//{
	//	potentialVert = cursorPos;
	//	Vec2 from = spawningVerts.back();

	//	Vec2 lastEdgeNormalised, lastEdgePerpendicular, fromPos, lastVertToPotentialPos;
	//	float dot, dotPerpendicular;

	//	Vec2 toHome, toHomePerpendicular;
	//	float dotToHome, dotPerpendicularToHome;

	//	toHome = glm::normalize(spawningVerts.front() - spawningVerts.back());
	//	toHomePerpendicular = { -toHome.y, toHome.x };

	//	Vec2 firstEdgeNormalized, firstEdgePerpendicular, firstVertToPotentialPos;
	//	float dotfirstEdge, dotfirstEdgePerpendicular;

	//	float potentialVertdistanceToHome = 0.0f;;

	//	if (spawningVerts.size() > 1)
	//	{
	//		lastEdgeNormalised = glm::normalize(spawningVerts[spawningVerts.size() - 1] - spawningVerts[spawningVerts.size() - 2]);
	//		firstEdgeNormalized = glm::normalize(spawningVerts[1] - spawningVerts[0]);
	//		firstEdgePerpendicular = { firstEdgeNormalized.y, -firstEdgeNormalized.x };
	//	}
	//	else
	//		lastEdgeNormalised = { 1,0 };
	//		
	//	lastEdgePerpendicular = { lastEdgeNormalised.y, -lastEdgeNormalised.x };

	//	fromPos = spawningVerts[spawningVerts.size() - 1];
	//	lastVertToPotentialPos = potentialVert - spawningVerts.back();
	//	potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
	//		

	//		




	//	lines->SetColour({ .5,.5,.5 });

	//	// check for attempt to make concave polygon in counter clockwise direction
	//	dot = glm::dot(lastVertToPotentialPos, lastEdgeNormalised);
	//	dotPerpendicular = glm::dot(lastVertToPotentialPos, lastEdgePerpendicular);
	//	if (dotPerpendicular < 0)
	//	{
	//		potentialVert = fromPos + (lastEdgeNormalised * glm::max(0.0f,dot));
	//		potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
	//	}
	//		
	//	// check for attempt to make concave polygon in clockwise direction
	//	dotToHome = glm::dot(lastVertToPotentialPos, toHome);
	//	dotPerpendicularToHome = glm::dot(lastVertToPotentialPos, toHomePerpendicular);
	//	if (dotPerpendicularToHome < 0 && spawningVerts.size() > 2)
	//	{
	//		potentialVert = fromPos + (toHome * glm::max(0.0f, dotToHome));
	//		potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
	//	}

	//	// check for attemp to make edge going past starting vector
	//	firstVertToPotentialPos = potentialVert - spawningVerts[0];
	//	dotfirstEdge = glm::dot(firstVertToPotentialPos, firstEdgeNormalized);
	//	dotfirstEdgePerpendicular = glm::dot(firstVertToPotentialPos, firstEdgePerpendicular);
	//	if (spawningVerts.size() > 2 && dotfirstEdgePerpendicular < 0.0f)
	//	{

	//		// check if our poly is closing in on itself so that we can define a max distance along this vector.
	//		float maxDistance = FLT_MAX;

	//		Vec2 v1 = spawningVerts[0] + (-firstEdgeNormalized * 50.0f);
	//		Vec2 v2 = spawningVerts[spawningVerts.size() - 1] + (lastEdgeNormalised * 50.0f);



	//		float dot = glm::dot(glm::normalize(v1), glm::normalize(v2));
	//		if (dot > 0.0f)
	//		{
	//			std::cout << "vecs are similar" << std::endl;
	//			Vec2 A, B, C, D;
	//			A = spawningVerts[0];
	//			B = v1;
	//			C = spawningVerts[spawningVerts.size() - 1];
	//			D = v2;

	//			Vec2 maxPoint = LineIntersection(A, B, C, D);
	//			float dotto = glm::dot(glm::normalize(spawningVerts[0] - maxPoint), firstEdgeNormalized);
	//			std::cout << dotto << std::endl;
	//			if (dotto > 0.99f)
	//			{
	//				std::cout << "poly is closing in on itself" << std::endl;
	//				lines->DrawCircle(maxPoint, 0.3f);
	//				lines->DrawLineSegment(spawningVerts[spawningVerts.size() - 1], v2, { 1,0,0 });
	//				lines->DrawLineSegment(spawningVerts[0], v1, { 1,0,0 });
	//				maxDistance = glm::distance(spawningVerts[0], maxPoint);
	//			}
	//		}

	//		potentialVert = spawningVerts[0] + (firstEdgeNormalized * glm::min(0.0f, glm::max(dotfirstEdge, -maxDistance)));
	//	}

	//	// check for closing the loop
	//	if (glm::distance(cursorPos, spawningVerts[0]) < 0.2f)
	//		potentialVert = spawningVerts[0];

	//	lines->DrawLineSegment(from, potentialVert);
	//	lines->DrawCircle(potentialVert, 0.1);

	//	lines->SetColour({ .8,.8,.8 });
	//}
	//else
	//	lines->DrawCircle(cursorPos, 0.1);
		
	
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

	if(spawner != nullptr)
		spawner->OnLeftClick(cursorPos);

	return;
	// Poly spawner stuff
	if (spawningVerts.size() > 0)
	{
		if (glm::distance(potentialVert, spawningVerts.front()) < 0.2f)
		{
			// Create the shape
			ConvexPolygon* poly = new ConvexPolygon({ 0,0 }, 1, spawningVerts, { 1,1,1 });
			spawningVerts.clear();
			shapes.push_back(poly);
		}
		else
		{
			for (auto& vert : spawningVerts)
			{
				if (glm::distance(potentialVert, vert) < 0.2f)
					return;
			}
			spawningVerts.push_back(potentialVert);
		}
	}
	else
		spawningVerts.push_back(cursorPos);



}

void CollisionFramework::OnLeftRelease()
{
	if (spawner != nullptr)
		spawner->OnLeftRelease();
}

void CollisionFramework::OnRightClick()
{
	if (spawner != nullptr)
		spawner->OnRightClick();
}

void CollisionFramework::OnRightRelease()
{
	if (spawner != nullptr)
		spawner->OnRightRelease();
}

void CollisionFramework::OnMouseScroll(double delta)
{
	if (spawner != nullptr)
		spawner->OnMouseScroll(delta);
}

Vec2 CollisionFramework::LineIntersection(Vec2 A, Vec2 B, Vec2 C, Vec2 D)
{
	// Line AB represented as a1x + b1y = c1
	double a1 = B.y - A.y;
	double b1 = A.x - B.x;
	double c1 = a1 * (A.x) + b1 * (A.y);

	// Line CD represented as a2x + b2y = c2
	double a2 = D.y - C.y;
	double b2 = C.x - D.x;
	double c2 = a2 * (C.x) + b2 * (C.y);

	double determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// The lines are parallel. This is simplified
		// by returning a pair of FLT_MAX
		return Vec2(FLT_MAX, FLT_MAX);
	}
	else
	{
		double x = (b2 * c1 - b1 * c2) / determinant;
		double y = (a1 * c2 - a2 * c1) / determinant;
		return Vec2(x, y);
	}
}