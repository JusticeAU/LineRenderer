#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"
#include "Spawner.h"

CollisionFramework::CollisionFramework()
{
	// Create our spawner tools and give it a reference to the vector of world shapes.
	spawner = new Spawner(&shapes);

	// Set up world border planes
	/*Vec2 planeNormal = Vec2(1, 0);
	planeNormal = glm::normalize(planeNormal);
	testPlane = new Plane(planeNormal, -4, { 1,1,1 });
	testCircle = (Shape*)new Circle(Vec2(0), 1.0f, 1.0f);*/

	//shapes.push_back(new Plane({ 0,1 }, -9, { 0,0,0 }));
	//shapes.push_back(new Plane({ 1,0 }, -16, { 0,0,0 }));
	//shapes.push_back(new Plane({ -1,0 }, -16, { 0,0,0 }));
}

void CollisionFramework::Update(float delta)
{
	// test Collsiion world post testing
	/*testCircle->m_position = cursorPos;
	testCircle->Draw(*lines);
	testPlane->Draw(*lines);
	CollisionData colTest = CircleOnPlane(testCircle, testPlane);
	lines->DrawCross(colTest.worldPosition, 1.0f);*/

	//for (auto& shape : shapes)
	//{
	//	if (shape->GetShape() != SHAPE::CONVEX_POLY)
	//		continue;

	//	ConvexPolygon* poly = (ConvexPolygon*)shape;
	//	//Get min and max extents
	//	float minX = -poly->aabb.HalfWidth() + shape->m_position.x;
	//	float maxX = poly->aabb.HalfWidth() + shape->m_position.x;
	//	float minY = -poly->aabb.HalfHeight() + shape->m_position.y;
	//	float maxY = poly->aabb.HalfHeight() + shape->m_position.y;

	//	int hits = 0;
	//	double areaUnit = 0.1f;
	//	double moment = 0.0f;
	//	for (float x = minX; x <= maxX; x += areaUnit)
	//	{
	//		for (float y = minY; y <= maxY; y += areaUnit)
	//		{
	//			Vec2 castPos(x, y);

	//			if (shape->PointInShape(castPos))
	//			{
	//				lines->DrawCross(castPos, 0.1f, { 0,1,0 });
	//				double distance = glm::distance(castPos, shape->m_position);
	//				moment += distance * distance * areaUnit * areaUnit;
	//				hits++;
	//			}
	//			else
	//			{
	//				//lines->DrawCross(castPos, 0.1f, { 1,1,0 });
	//			}
	//		}
	//	}
	//	std::cout << "Approximated Area: " << (float)hits * areaUnit * areaUnit << std::endl;
	//	std::cout << "Approximated Moment: " << moment << std::endl;

	//}

	// Delete any Shapes that need deleting
	for (int i = 0; i < shapes.size(); i++)
	{
		Shape* shape = shapes[i];
		if (shape->IsMarkedForDeletion())
		{
			shapes.erase(shapes.begin() + i);
			i--;
		}
	}

	// Do whacky gravity stuff ehehe.
	if (middleMouseDown)
	{
		for (auto& shape : shapes)
		{
			Vec2 toCursor = cursorPos - shape->m_position;
			toCursor = glm::normalize(toCursor);
			shape->ApplyImpulse(toCursor * (1.0f / shape->GetInverseMass()), Vec2(0));
		}
	}

	// Update all Shapes
	for (auto& shape : shapes)
		shape->Update(delta);

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
			{
				// Disable Collisions for now
				col.Resolve();

				// Draw dose contact point velocities if they exist
				/*for (auto& point : col.supplementaryPoints)
				{
					lines->DrawLineSegment(col.worldPosition, col.worldPosition + col.supplementaryPoints[0], { 1,1,1 });
				}*/


				// Lets draw the collisions
				lines->DrawCircle(col.worldPosition, 0.1f, {0,1,1});
				//lines->DrawLineSegment(col.worldPosition, col.worldPosition + col.normal * col.depth, {0,1,1});
			}


		}
	}

	// Draw all shapes in simulation.
	for (auto s : shapes)
	{
		s->Draw(*lines);
		if (s->GetShape() == SHAPE::CONVEX_POLY)
		{
			ConvexPolygon* poly = (ConvexPolygon*)s;
			poly->CalculateMomentOfInertia(*lines);
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