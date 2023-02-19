#include "Spawner.h"
#include "Circle.h"
#include "AABB.h"
#include "Plane.h"
#include "ConvexPolygon.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"

#include "LineRenderer.h"
#include "glm.hpp"

#include <iostream>
Spawner::Spawner(std::vector<Shape*>* shapes)
{
	this->shapes = shapes;

	// Set up template shapes
	shapeTemplates.push_back(new Circle(Vec2(0), 1, 0, templateColour));
	shapeTemplates.push_back(new AABB(Vec2(0), 1, 1.0f, 0, templateColour));

	// set up hard coded poly for now:
	std::vector<Vec2> somePoints = std::vector<Vec2>();
	somePoints.push_back(Vec2(0, 1));
	somePoints.push_back(Vec2(1.8, 0.8));

	somePoints.push_back(Vec2(1, -1));
	somePoints.push_back(Vec2(-1, -1));
	shapeTemplates.push_back(new ConvexPolygon(Vec2(0), 1, somePoints, templateColour));
	ConvexPolygon* poly = (ConvexPolygon*)shapeTemplates.back();
	poly->CalculateCentroid();

	shapeTemplates.push_back(new Plane(Vec2(0), 1, templateColour));
}

void Spawner::Update(float delta, Vec2 cursorPos)
{
	timeSinceStart += delta;
	this->cursorPos = cursorPos;

	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		// Update our tool
		if (selectedTool < 3) // circle, aabb, poly
		{
			if (shapeTemplates[selectedTool]->m_position != cursorPos)
			{
				Vec2 shapeToCursor = cursorPos - shapeTemplates[selectedTool]->m_position;
				Vec2 shapeToCursorNormalized = glm::normalize(shapeToCursor);
				shapeTemplates[selectedTool]->m_position += shapeToCursor * delta * 10.0f;
			}
		}
		else if (selectedTool == (int)SPAWNER_TOOL::SPAWN_PLANE)
		{
			Plane* plane = (Plane*)shapeTemplates[selectedTool];
			plane->m_normal = -glm::normalize(cursorPos);
			plane->m_distance = -glm::length(cursorPos);
		}
		else if (selectedTool == (int)SPAWNER_TOOL::LINE_CUTTER)
		{
			// some cool shit.
		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (selectedTool == (int)SPAWNER_TOOL::SPAWN_CIRCLE)
		{
			Circle* circle = (Circle*)shapeTemplates[(int)SHAPE::CIRCLE];
			if (shapeTemplates[selectedTool]->m_position != cursorPos)
				circle->m_radius = glm::distance(shapeTemplates[selectedTool]->m_position, cursorPos);

			if (circle->m_radius <= 0.3f)
				circle->m_radius = 0.3f;

		}
		else if (selectedTool == (int)SPAWNER_TOOL::SPAWN_AABB)
		{
			AABB* aabb = (AABB*)shapeTemplates[(int)SHAPE::AABB];
			float halfWidth = glm::abs(cursorPos.x - aabb->m_position.x);
			float halfHeight = glm::abs(cursorPos.y - aabb->m_position.y);

			halfWidth = glm::max(halfWidth, 0.2f);
			halfHeight = glm::max(halfHeight, 0.2f);

			aabb->m_halfWidth = halfWidth;
			aabb->m_halfHeight = halfHeight;

		}
		else if (selectedTool == (int)SPAWNER_TOOL::SPAWN_CONVEX_POLY)
		{
			DoPolygonConstructionUpdate(delta, cursorPos);
		}
		else if (selectedTool == (int)SPAWNER_TOOL::SPAWN_PLANE)
		{
			Plane* plane = (Plane*)shapeTemplates[(int)SHAPE::PLANE];
			plane->m_normal = -glm::normalize(cursorPos);
			plane->m_distance = -glm::length(cursorPos);
		}
		break;
	}
	case SPAWNER_STATE::LAUNCH:
	{
		break;
	}
	case SPAWNER_STATE::GRAB:
	{
		if (grabbed->m_position != cursorPos)
		{
			Vec2 shapeToCursor = cursorPos - grabbed->m_position;
			Vec2 shapeToCursorNormalized = glm::normalize(shapeToCursor);
			grabbed->m_velocity = shapeToCursor * 10.0f;
		}
		break;
	}
	}
}

void Spawner::Draw(LineRenderer& lines)
{
	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		if(selectedTool < 4)
			shapeTemplates[selectedTool]->Draw(lines);
		else
		{

		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (selectedTool < 3)
			shapeTemplates[selectedTool]->Draw(lines);
		else if(selectedTool == (int)SPAWNER_TOOL::SPAWN_CONVEX_POLY)
			DoPolygonConstructionDraw(lines);
		else if (selectedTool == (int)SPAWNER_TOOL::LINE_CUTTER)
		{

		}
		break;
	}
	case SPAWNER_STATE::LAUNCH:
	{
		spawn->Draw(lines);
		lines.DrawLineSegment(spawn->m_position, cursorPos);
		break;
	}
	case SPAWNER_STATE::GRAB:
	{
		Vec3 oldColour = grabbed->m_colour;
		grabbed->m_colour = { 1,1,1 };
		grabbed->Draw(lines);
		grabbed->m_colour = oldColour;
	}
	case SPAWNER_STATE::CUT:
	{
		lines.DrawLineSegment(cursorDownPos, cursorPos, { 1,0,0 });
	}
	default:
		break;
	}
}

void Spawner::OnLeftClick(Vec2 cursorPos)
{
	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		// Check if were selecting an object
		for (auto* shape : *shapes)
		{
			if (shape->PointInShape(cursorPos))
			{
				grabbed = shape;
				break;
			}
		}

		if (grabbed != nullptr)
		{
			// grab object logic
			state = SPAWNER_STATE::GRAB;
			grabbedInverseMass = grabbed->m_inverseMass;
			grabbed->m_inverseMass = 0;
		}
		else
		{
			if (selectedTool < 4) // Spawnables
			{

				// Spawn and Launch object logic
				state = SPAWNER_STATE::LAUNCH;
				// Spawn current template
				switch (shapeTemplates[selectedTool]->GetShape())
				{
				case SHAPE::CIRCLE:
				{
					Circle* circle = new Circle(*(Circle*)(shapeTemplates[selectedTool]));
					circle->m_position = cursorPos;
					spawn = circle;
					break;
				}
				case SHAPE::AABB:
				{
					AABB* aabb = new AABB(*dynamic_cast<AABB*>(shapeTemplates[selectedTool]));
					aabb->m_position = cursorPos;
					spawn = aabb;
					break;
				}
				case SHAPE::PLANE:
				{
					Plane* plane = new Plane(*static_cast<Plane*>(shapeTemplates[selectedTool]));
					spawn = plane;
					break;
				}
				case SHAPE::CONVEX_POLY:
				{
					ConvexPolygon* poly = new ConvexPolygon(*(ConvexPolygon*)shapeTemplates[selectedTool]);
					spawn = poly;
					break;
				}
				}
			}
			else if (selectedTool == (int)SPAWNER_TOOL::LINE_CUTTER)
			{
				cursorDownPos = cursorPos;
				state = SPAWNER_STATE::CUT;
			}
		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (selectedTool == (int)SPAWNER_TOOL::SPAWN_CONVEX_POLY)
		{
			// Poly spawner stuff
			if (spawningVerts.size() > 0)
			{
				if (glm::distance(potentialVert, spawningVerts.front()) < 0.2f)
				{
					// Create the shape
					ConvexPolygon* poly = new ConvexPolygon({ 0,0 }, 1, spawningVerts, templateColour);
					spawningVerts.clear();
					delete shapeTemplates[selectedTool];
					shapeTemplates[selectedTool] = poly;
					state = SPAWNER_STATE::IDLE;
					poly->CalculateCentroid();
					
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
		break;
	}
	default:
		break;
	}


}

void Spawner::OnLeftRelease()
{
	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		//state = SPAWNER_STATE::IDLE;
		break;
	}
	case SPAWNER_STATE::LAUNCH:
	{
		state = SPAWNER_STATE::IDLE;

		if (spawn != nullptr)
		{
			spawn->CalculateMassFromArea();

			// Random Colour
			float r = static_cast<float>(rand()) / (float)(RAND_MAX);
			float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			spawn->m_colour = { r,g,b };

			// Set its velocity to be a slingshot from start and current mouse pos.
			if (spawn->GetShape() != SHAPE::PLANE)
			{
				Vec2 velocity = (spawn->m_position - cursorPos) * 5.0f;
				spawn->m_velocity = velocity;
			}
			shapes->push_back(spawn);
		}
		spawn = nullptr;
		break;
	}
	case SPAWNER_STATE::GRAB:
	{
		grabbed->m_inverseMass = grabbedInverseMass;
		grabbed = nullptr;
		state = SPAWNER_STATE::IDLE;
		break;
	}
	case SPAWNER_STATE::CUT:
	{
		state = SPAWNER_STATE::IDLE;
		// make line from cursorDownPos to cursorPos and calculate intersection across all convex polys
	}
	default:
		break;
	}



}

void Spawner::OnRightClick()
{
	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		state = SPAWNER_STATE::BUILD;
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		state = SPAWNER_STATE::IDLE;
		break;
	}
	case SPAWNER_STATE::LAUNCH:
	{
		break;
	}
	case SPAWNER_STATE::GRAB:
	{
		// Delete grabbed object
		for (int i = 0; i < shapes->size(); i++)
		{
			if (grabbed == shapes->at(i))
			{
				shapes->erase(shapes->begin()  + i);
				grabbed = nullptr;
				state = SPAWNER_STATE::IDLE;
				break;
			}
		}
	}
	default:
		break;
	}
}

void Spawner::OnRightRelease()
{
	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (selectedTool < 3) // simple buildables
			state = SPAWNER_STATE::IDLE;
		break;
	}
	case SPAWNER_STATE::LAUNCH:
	{
		break;
	}
	case SPAWNER_STATE::GRAB:
	{

	}
	default:
		break;
	}
}

void Spawner::OnMouseScroll(double delta)
{
	if (state == SPAWNER_STATE::IDLE)
	{
		if (glm::sign<double>(delta) == 1)
		{
			selectedTool += 1;
			if (selectedTool == (int)SPAWNER_TOOL::COUNT)
				selectedTool = 0;
		}
		else if (glm::sign<double>(delta) == -1)
		{
			selectedTool -= 1;
			if (selectedTool == -1)
				selectedTool = (int)SPAWNER_TOOL::COUNT-1;
		}
	}

}

void Spawner::DoPolygonConstructionUpdate(float delta, Vec2 cursorPos)
{
	//Poly Spawner shit
	potentialVert = cursorPos;
	if(spawningVerts.size() > 0)
	{
		Vec2 from = spawningVerts.back();

		Vec2 lastEdgeNormalised, lastEdgePerpendicular, fromPos, lastVertToPotentialPos;
		float dot, dotPerpendicular;

		Vec2 toHome, toHomePerpendicular;
		float dotToHome, dotPerpendicularToHome;

		toHome = glm::normalize(spawningVerts.front() - spawningVerts.back());
		toHomePerpendicular = { -toHome.y, toHome.x };

		Vec2 firstEdgeNormalized, firstEdgePerpendicular, firstVertToPotentialPos;
		float dotfirstEdge, dotfirstEdgePerpendicular;

		float potentialVertdistanceToHome = 0.0f;;

		if (spawningVerts.size() > 1)
		{
			lastEdgeNormalised = glm::normalize(spawningVerts[spawningVerts.size() - 1] - spawningVerts[spawningVerts.size() - 2]);
			firstEdgeNormalized = glm::normalize(spawningVerts[1] - spawningVerts[0]);
			firstEdgePerpendicular = { firstEdgeNormalized.y, -firstEdgeNormalized.x };
		}
		else
			lastEdgeNormalised = { 1,0 };
			
		lastEdgePerpendicular = { lastEdgeNormalised.y, -lastEdgeNormalised.x };

		fromPos = spawningVerts[spawningVerts.size() - 1];
		lastVertToPotentialPos = potentialVert - spawningVerts.back();
		potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
			



		//lines->SetColour({ .5,.5,.5 });

		// check for attempt to make concave polygon in counter clockwise direction
		dot = glm::dot(lastVertToPotentialPos, lastEdgeNormalised);
		dotPerpendicular = glm::dot(lastVertToPotentialPos, lastEdgePerpendicular);
		if (dotPerpendicular < 0)
		{
			potentialVert = fromPos + (lastEdgeNormalised * glm::max(0.0f,dot));
			potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
		}
			
		// check for attempt to make concave polygon in clockwise direction
		dotToHome = glm::dot(lastVertToPotentialPos, toHome);
		dotPerpendicularToHome = glm::dot(lastVertToPotentialPos, toHomePerpendicular);
		if (dotPerpendicularToHome < 0 && spawningVerts.size() > 2)
		{
			potentialVert = fromPos + (toHome * glm::max(0.0f, dotToHome));
			potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
		}

		// check for attemp to make edge going past starting vector
		firstVertToPotentialPos = potentialVert - spawningVerts[0];
		dotfirstEdge = glm::dot(firstVertToPotentialPos, firstEdgeNormalized);
		dotfirstEdgePerpendicular = glm::dot(firstVertToPotentialPos, firstEdgePerpendicular);
		if (spawningVerts.size() > 2 && dotfirstEdgePerpendicular < 0.0f)
		{

			// check if our poly is closing in on itself so that we can define a max distance along this vector.
			float maxDistance = FLT_MAX;

			Vec2 v1 = spawningVerts[0] + (-firstEdgeNormalized * 50.0f);
			Vec2 v2 = spawningVerts[spawningVerts.size() - 1] + (lastEdgeNormalised * 50.0f);



			float dot = glm::dot(glm::normalize(v1), glm::normalize(v2));
			if (dot > 0.0f)
			{
				std::cout << "vecs are similar" << std::endl;
				Vec2 A, B, C, D;
				A = spawningVerts[0];
				B = v1;
				C = spawningVerts[spawningVerts.size() - 1];
				D = v2;

				Vec2 maxPoint = LineIntersection(A, B, C, D);
				float dotto = glm::dot(glm::normalize(spawningVerts[0] - maxPoint), firstEdgeNormalized);
				std::cout << dotto << std::endl;
				if (dotto > 0.99f)
				{
					std::cout << "poly is closing in on itself" << std::endl;
					//lines->DrawCircle(maxPoint, 0.3f);
					//lines->DrawLineSegment(spawningVerts[spawningVerts.size() - 1], v2, { 1,0,0 });
					//lines->DrawLineSegment(spawningVerts[0], v1, { 1,0,0 });
					maxDistance = glm::distance(spawningVerts[0], maxPoint);
				}
			}

			potentialVert = spawningVerts[0] + (firstEdgeNormalized * glm::min(0.0f, glm::max(dotfirstEdge, -maxDistance)));
		}

		// check for closing the loop
		if (glm::distance(cursorPos, spawningVerts[0]) < 0.2f)
			potentialVert = spawningVerts[0];

		//lines->DrawLineSegment(from, potentialVert);
		//lines->DrawCircle(potentialVert, 0.1);

		//lines->SetColour({ .8,.8,.8 });
	}
	else
	{
		//lines->DrawCircle(cursorPos, 0.1);

	}
}

void Spawner::DoPolygonConstructionDraw(LineRenderer& lines)
{
	//draw current poly progress
	lines.SetColour(templateColour);
	for (auto& point : spawningVerts)
		lines.AddPointToLine(point);
	lines.FinishLineStrip();

	// Draw next potential line
	if(spawningVerts.size() > 0)
		lines.DrawLineSegment(spawningVerts.back(), potentialVert);

	// Draw Cursor
	lines.DrawCircle(potentialVert, 0.2f);
}

Vec2 Spawner::LineIntersection(Vec2 A, Vec2 B, Vec2 C, Vec2 D)
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