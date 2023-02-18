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
	shapeTemplates.push_back(new Circle(Vec2(0), 1, 0, {0.2f,0.2f,0.2f}));
	shapeTemplates.push_back(new AABB(Vec2(0), 1, 1.0f, 0, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new Plane(Vec2(0), 1, { 0.2f,0.2f,0.2f }));

	// set up hard coded poly for now:
	std::vector<Vec2> somePoints = std::vector<Vec2>();
	somePoints.push_back(Vec2(0, 1));
	somePoints.push_back(Vec2(1.8, 0.8));

	somePoints.push_back(Vec2(1, -1));
	somePoints.push_back(Vec2(-1, -1));
	shapeTemplates.push_back(new ConvexPolygon(Vec2(0), 1, somePoints, { 0.2f,0.2f,0.2f }));
}

void Spawner::Update(float delta, Vec2 cursorPos)
{
	timeSinceStart += delta;
	this->cursorPos = cursorPos;

	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		if (shapeTemplates[templateIndex]->GetShape() == SHAPE::CIRCLE)
		{
			Circle* circle = static_cast<Circle*>(shapeTemplates[templateIndex]);
			shapeTemplates[templateIndex]->m_position = cursorPos;
		}
		else if (shapeTemplates[templateIndex]->GetShape() == SHAPE::PLANE)
		{
			Plane* plane = static_cast<Plane*>(shapeTemplates[templateIndex]);
			plane->m_normal = -glm::normalize(cursorPos);
			plane->m_distance = -glm::length(cursorPos);
		}
		else if (shapeTemplates[templateIndex]->GetShape() == SHAPE::AABB)
		{
			AABB* aabb = (AABB*)shapeTemplates[templateIndex];
			Vec2 offset = { aabb->m_halfWidth, -aabb->m_halfHeight };
			shapeTemplates[templateIndex]->m_position = cursorPos + offset;
		}
		else
		{
			shapeTemplates[templateIndex]->m_position = cursorPos;
		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{
		if (templateIndex == (int)SHAPE::CIRCLE)
		{
			Circle* circle = (Circle*)shapeTemplates[templateIndex];
			if (shapeTemplates[templateIndex]->m_position != cursorPos)
				circle->m_radius = glm::distance(shapeTemplates[templateIndex]->m_position, cursorPos);

			if (circle->m_radius <= 0.3f)
				circle->m_radius = 0.3f;

		}
		else if (templateIndex == (int)SHAPE::AABB)
		{
			AABB* aabb = (AABB*)shapeTemplates[templateIndex];
			aabb->m_halfWidth = glm::abs(cursorPos.x - spawnStartPos.x) * 0.5f;
			aabb->m_halfHeight = glm::abs(cursorPos.y - spawnStartPos.y) * 0.5f;

			Vec2 offset = { aabb->m_halfWidth, aabb->m_halfHeight };
			aabb->m_position = (spawnStartPos + cursorPos) * 0.5f;
		}
		else if (templateIndex == (int)SHAPE::PLANE)
		{
			Plane* plane = (Plane*)shapeTemplates[templateIndex];
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
		break;
	}
	}

	// Handle Template Spawning
	if (spawn != nullptr)
	{
		

		// run collision against objects in scene
		for (int i = 0; i < 3; i++)
		{
			std::vector<CollisionData> collisions;
			for (int b = 0; b < shapes->size(); b++)
			{
				Shape* other = shapes->at(b);
				CollisionData col = TestCollisions(spawn, other);
				if (col.IsCollision())
					collisions.push_back(col);
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
}

void Spawner::Draw(LineRenderer& lines)
{
	if (spawn != nullptr)
	{
		// Draw Slingshot
		lines.DrawLineSegment(spawn->m_position, cursorPos);
		spawn->Draw(lines);
	}
	else
		shapeTemplates[templateIndex]->Draw(lines);
}

void Spawner::OnLeftClick(Vec2 cursorPos)
{
	switch (state)
	{
	case SPAWNER_STATE::IDLE:
	{
		// Check if were selecting an object
		if (false)
		{
			// grab object logic
		}
		else
		{
			// Spawn and Launch object logic
			state = SPAWNER_STATE::LAUNCH;
			// Spawn current template
			switch (shapeTemplates[templateIndex]->GetShape())
			{
			case SHAPE::CIRCLE:
			{
				Circle* circle = new Circle(*(Circle*)(shapeTemplates[templateIndex]));
				circle->m_position = cursorPos;
				spawn = circle;
				break;
			}
			case SHAPE::AABB:
			{
				AABB* aabb = new AABB(*dynamic_cast<AABB*>(shapeTemplates[templateIndex]));
				spawnStartPos = cursorPos;
				Vec2 offset = { aabb->m_halfWidth, -aabb->m_halfHeight };
				aabb->m_position = cursorPos + offset;
				spawn = aabb;
				break;
			}
			case SHAPE::PLANE:
			{
				Plane* plane = new Plane(*static_cast<Plane*>(shapeTemplates[templateIndex]));
				spawn = plane;
				break;
			}
			case SHAPE::CONVEX_POLY:
			{
				ConvexPolygon* poly = new ConvexPolygon(*(ConvexPolygon*)shapeTemplates[templateIndex]);
				spawn = poly;
				break;
			}
			}
		}
		break;
	}
	case SPAWNER_STATE::BUILD:
	{

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
		break;
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

	}
	default:
		break;
	}
}

void Spawner::OnRightRelease()
{
	
}

void Spawner::OnMouseScroll(double delta)
{
	if (state == SPAWNER_STATE::IDLE)
	{
		if (glm::sign<double>(delta) == 1)
		{
			templateIndex += 1;
			if (templateIndex == (int)SHAPE::COUNT)
				templateIndex = 0;
		}
		else if (glm::sign<double>(delta) == -1)
		{
			templateIndex -= 1;
			if (templateIndex == -1)
				templateIndex = (int)SHAPE::COUNT-1;
		}
	}

}