#pragma once

#include "Spawner.h"
#include "Circle.h"
#include "AABB.h"
#include "Plane.h"
#include "ConvexPolygon.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"
#include "LineRenderer.h"
#include "glm.hpp"

Spawner::Spawner(std::vector<Shape*>* shapes)
{
	this->shapes = shapes;

	// Set up template shapes
	// Circle
	shapeTemplates.push_back(new Circle(Vec2(0), 1, 0, templateColour));

	// Box Poly
	std::vector<Vec2> boxPoints;
	boxPoints.push_back({ 1,1 });
	boxPoints.push_back({ 1,-1 });
	boxPoints.push_back({ -1,-1 });
	boxPoints.push_back({ -1,1 });
	shapeTemplates.push_back(new ConvexPolygon(Vec2(0), 1, boxPoints, templateColour));

	// "Complex" Poly
	std::vector<Vec2> somePoints = std::vector<Vec2>();
	somePoints.push_back(Vec2(0, 1));
	somePoints.push_back(Vec2(1.8, 0.8));
	somePoints.push_back(Vec2(1, -1));
	somePoints.push_back(Vec2(-1, -1));
	shapeTemplates.push_back(new ConvexPolygon(Vec2(0), 1, somePoints, templateColour));
	ConvexPolygon* poly = (ConvexPolygon*)shapeTemplates.back();
	poly->RecalculateCentre();

	// "Plain" Plane
	shapeTemplates.push_back(new Plane(Vec2(0), 1, templateColour));
}

void Spawner::Update(float delta, Vec2 cursorPos)
{
	oldCursorPos = this->cursorPos;
	this->cursorPos = cursorPos;

	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		// Update our tool
		if (selectedTool < 3) // Circle, Box, Poly
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
			// Cutter has no behaviour in update. It it handled entirely in LeftClick and LeftRelease
		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (selectedTool == (int)SPAWNER_TOOL::SPAWN_CIRCLE)
		{
			// Use cursor pos and circles current position to get a distance which will now be the radius.
			Circle* circle = (Circle*)shapeTemplates[(int)SHAPE::CIRCLE];
			if (shapeTemplates[selectedTool]->m_position != cursorPos)
				circle->SetRadius(glm::distance(shapeTemplates[selectedTool]->m_position, cursorPos));

			if (circle->GetRadius() <= 0.3f)
				circle->SetRadius(0.3f);

		}
		else if (selectedTool == (int)SPAWNER_TOOL::SPAWN_AABB) // This is actually a poly now!!
		{
			// Use cursor pos and and Boxes position to get a half width and height for our box spawning.
			ConvexPolygon* boxPoly = (ConvexPolygon*)shapeTemplates[(int)SHAPE::AABB];
			float halfWidth = glm::abs(cursorPos.x - boxPoly->m_position.x);
			float halfHeight = glm::abs(cursorPos.y - boxPoly->m_position.y);

			halfWidth = glm::max(halfWidth, 0.5f);
			halfHeight = glm::max(halfHeight, 0.5f);

			// Update all points
			std::vector<Vec2> points;
			points.push_back({ halfWidth, halfHeight });
			points.push_back({ halfWidth, -halfHeight });
			points.push_back({ -halfWidth, -halfHeight });
			points.push_back({ -halfWidth, halfHeight });
			boxPoly->SetPoints(points);

		}
		else if (selectedTool == (int)SPAWNER_TOOL::SPAWN_CONVEX_POLY)
		{
			// Tell our slightly more complex poly building process to update.
			DoPolygonConstructionUpdate(delta, cursorPos);
		}
		else if (selectedTool == (int)SPAWNER_TOOL::SPAWN_PLANE)
		{
			// use cursor pos to define a distance and normal for our plane template
			Plane* plane = (Plane*)shapeTemplates[(int)SHAPE::PLANE];
			plane->m_normal = -glm::normalize(cursorPos);
			plane->m_distance = -glm::length(cursorPos);
		}
		break;
	}
	case SPAWNER_STATE::LAUNCH:
	{
		// No action required. All handled in Draw and OnLeftRelease
		break;
	}
	case SPAWNER_STATE::GRAB:
	{
		// Move the grabbed object towards the cursor
		if (grabbed->m_position != cursorPos)
		{
			Vec2 shapeToCursor = cursorPos - grabbed->m_position;
			Vec2 shapeToCursorNormalized = glm::normalize(shapeToCursor);
			grabbed->m_velocity = shapeToCursor * 10.0f;
		}
	}
	}
}

void Spawner::Draw(LineRenderer& lines) const
{
	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		if(selectedTool < 4)
			shapeTemplates[selectedTool]->Draw(lines);
		else if (selectedTool == (int)SPAWNER_TOOL::LINE_CUTTER)
		{
			// Super cool line cutter visual. Looks kind of like a laser pointer.
			lines.DrawLineSegment(cursorPos, oldCursorPos, {1, 0, 0});
		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (selectedTool == (int)SPAWNER_TOOL::SPAWN_CONVEX_POLY)
			DoPolygonConstructionDraw(lines);
		else if (selectedTool < 3)
			shapeTemplates[selectedTool]->Draw(lines);
		break;
	}
	case SPAWNER_STATE::LAUNCH:
	{
		// Draw the object we're spawning and a line from it to our cursor to indicate the velocity we're about to give it when we release.
		spawn->Draw(lines);
		lines.DrawLineSegment(spawn->m_position, cursorPos);
		break;
	}
	case SPAWNER_STATE::GRAB:
	{
		grabbed->Draw(lines); // This will draw it white because we overrode the colour when we grabbed it, and will assign it back when we release.
		break;
	}
	case SPAWNER_STATE::CUT:
	{
		// Draw a line from where our cut will start to the cursor.
		lines.DrawLineSegment(cursorDownPos, cursorPos, { 1,0,0 });
		break;
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
			if (shape->PointInShape(cursorPos) && shape->GetShape() != SHAPE::PLANE)
			{
				grabbed = shape;
				// Store shapes assigned colour and override it to be white to highlight that we've grabbed it.
				grabbedColour = shape->m_colour;
				shape->m_colour = { 1,1,1 };
				break;
			}
		}

		if (grabbed != nullptr)	// Grab the object
		{
			state = SPAWNER_STATE::GRAB;
			grabbedInverseMass = grabbed->GetInverseMass();
			grabbed->MakeKinematic();
		}
		else // Nothing to grab, have selected tool perform its action.
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
					spawn = circle;
					break;
				}
				case SHAPE::AABB:
				{
					AABB* aabb = new AABB(*dynamic_cast<AABB*>(shapeTemplates[selectedTool]));
					spawn = aabb;
					break;
				}
				case SHAPE::CONVEX_POLY:
				{
					ConvexPolygon* poly = new ConvexPolygon(*(ConvexPolygon*)shapeTemplates[selectedTool]);
					spawn = poly;
					break;
				}
				case SHAPE::PLANE:
				{
					Plane* plane = new Plane(*static_cast<Plane*>(shapeTemplates[selectedTool]));
					spawn = plane;
					break;
				}
				}
			}
			else if (selectedTool == (int)SPAWNER_TOOL::LINE_CUTTER)
			{
				// begin cut state.
				cursorDownPos = cursorPos;
				state = SPAWNER_STATE::CUT;
			}
		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (selectedTool == (int)SPAWNER_TOOL::SPAWN_CONVEX_POLY) // Run poly builder logic
			DoPolygonConstructionLeftClick();
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
		state = SPAWNER_STATE::IDLE;

		// If the thing we grabbed wasn't originally kinematic then we need to set its mass back.
		if (grabbedInverseMass != 0.0f)
			grabbed->SetMass(1.0f / grabbedInverseMass);

		grabbed->m_colour = grabbedColour;
		grabbed = nullptr;
		break;
	}
	case SPAWNER_STATE::CUT:
	{
		state = SPAWNER_STATE::IDLE;

		// make line from cursorDownPos to cursorPos and calculate intersection across all current polys
		int maxShapesToCheck = shapes->size();
		for (int i = 0; i < maxShapesToCheck; i++)
		{
			Shape* shape = shapes->at(i);
			if (shape->LineIntersects(cursorDownPos, cursorPos))
				shape->Slice(cursorDownPos, cursorPos, shapes);
		}
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
		state = SPAWNER_STATE::IDLE;
		
		// Delete grabbed object
		grabbed->MarkForDeletion();
		grabbed = nullptr;
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
		if (selectedTool != 2) // simple buildables
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
		// Handle up and down scroll through different tools.
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
	// We're going to build a potentialVert position by taking the cursorPos and making sure its not in a place that would break the rules of a convex polygon.
	potentialVert = cursorPos;
	if(spawningVerts.size() > 0)
	{
		// Set up the various datapoints we're going to need.
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

		// Check for attempt to make concave polygon in counter clockwise direction
		dot = glm::dot(lastVertToPotentialPos, lastEdgeNormalised);
		dotPerpendicular = glm::dot(lastVertToPotentialPos, lastEdgePerpendicular);
		if (dotPerpendicular < 0)
		{
			potentialVert = fromPos + (lastEdgeNormalised * glm::max(0.0f,dot));
			potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
		}
			
		// Check for attempt to make concave polygon in clockwise direction
		dotToHome = glm::dot(lastVertToPotentialPos, toHome);
		dotPerpendicularToHome = glm::dot(lastVertToPotentialPos, toHomePerpendicular);
		if (dotPerpendicularToHome < 0 && spawningVerts.size() > 2)
		{
			potentialVert = fromPos + (toHome * glm::max(0.0f, dotToHome));
			potentialVertdistanceToHome = glm::distance(potentialVert, spawningVerts[0]);
		}

		// Check for attempt to make edge going past starting vector
		firstVertToPotentialPos = potentialVert - spawningVerts[0];
		dotfirstEdge = glm::dot(firstVertToPotentialPos, firstEdgeNormalized);
		dotfirstEdgePerpendicular = glm::dot(firstVertToPotentialPos, firstEdgePerpendicular);
		if (spawningVerts.size() > 2 && dotfirstEdgePerpendicular < 0.0f)
		{

			// Check if our poly is closing in on itself so that we can define a max distance along this vector.
			float maxDistance = FLT_MAX;

			Vec2 v1 = spawningVerts[0] + (-firstEdgeNormalized * 50.0f);
			Vec2 v2 = spawningVerts[spawningVerts.size() - 1] + (lastEdgeNormalised * 50.0f);

			float dot = glm::dot(glm::normalize(v1), glm::normalize(v2));
			if (dot > 0.0f)
			{
				Vec2 A, B, C, D;
				A = spawningVerts[0];
				B = v1;
				C = spawningVerts[spawningVerts.size() - 1];
				D = v2;

				Vec2 maxPoint;
				LineLineIntersection(A, B, C, D, &maxPoint);

				float dotto = glm::dot(glm::normalize(spawningVerts[0] - maxPoint), firstEdgeNormalized);
				if (dotto > 0.99f)
					maxDistance = glm::distance(spawningVerts[0], maxPoint);
			}

			potentialVert = spawningVerts[0] + (firstEdgeNormalized * glm::min(0.0f, glm::max(dotfirstEdge, -maxDistance)));
		}

		// Check for closing the loop
		if (glm::distance(cursorPos, spawningVerts[0]) < 0.2f)
			potentialVert = spawningVerts[0];
	}
}

void Spawner::DoPolygonConstructionDraw(LineRenderer& lines) const
{
	// Draw current poly progress
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

void Spawner::DoPolygonConstructionLeftClick()
{
	// Test if we're creating a new polygon or adding to one currently in construction
	if (spawningVerts.size() > 0)
	{
		// check if we're close to the home point and complete the loop if we are.
		if (glm::distance(potentialVert, spawningVerts.front()) < 0.2f)
		{
			state = SPAWNER_STATE::IDLE;
			// Create the shape
			ConvexPolygon* poly = new ConvexPolygon({ 0,0 }, 1, spawningVerts, templateColour);
			spawningVerts.clear();
			delete shapeTemplates[selectedTool];
			shapeTemplates[selectedTool] = poly;

		}
		else // otherwise test that its not too close to another vert (annoying) and if not, push it on to the list of points.
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