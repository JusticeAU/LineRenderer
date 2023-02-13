#include "Spawner.h"
#include "Circle.h"
#include "AABB.h"
#include "Plane.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"

#include "LineRenderer.h"
#include "glm.hpp"

Spawner::Spawner(std::vector<Shape*>* shapes)
{
	this->shapes = shapes;

	// Set up template shapes
	shapeTemplates.push_back(new Circle(Vec2(0), 1, 0, {0.2f,0.2f,0.2f}));
	shapeTemplates.push_back(new AABB(Vec2(0), 1, 1.0f, 0, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new Plane(Vec2(0), 1, { 0.2f,0.2f,0.2f }));
}

void Spawner::Update(float delta, Vec2 cursorPos)
{
	timeSinceStart += delta;

	// Handle Template Spawning
	if (spawn != nullptr)
	{
		if (spawn->GetShape() == SHAPE::CIRCLE)
		{
			Circle* circle = (Circle*)spawn;
			circle->m_radius = glm::distance(spawn->m_position, cursorPos);
			if (circle->m_radius <= 0.3f)
				circle->m_radius = 0.3f;

		}
		else if (spawn->GetShape() == SHAPE::AABB)
		{
			AABB* aabb = (AABB*)spawn;
			aabb->m_halfWidth = glm::abs(cursorPos.x - spawnStartPos.x) * 0.5f;
			aabb->m_halfHeight = glm::abs(cursorPos.y - spawnStartPos.y) * 0.5f;

			Vec2 offset = { aabb->m_halfWidth, aabb->m_halfHeight };
			aabb->m_position = (spawnStartPos + cursorPos) * 0.5f;
		}
		else if (spawn->GetShape() == SHAPE::PLANE)
		{
			Plane* plane = static_cast<Plane*>(spawn);
			plane->m_normal = -glm::normalize(cursorPos);
			plane->m_distance = -glm::length(cursorPos);
		}

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
	else
	{
		if (shapeTemplates[templateIndex]->GetShape() == SHAPE::CIRCLE)
		{
			Circle* circle = static_cast<Circle*>(shapeTemplates[templateIndex]);
			circle->m_radius = 0.3 + glm::sin(timeSinceStart * 5) * 0.1f;
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
			aabb->m_halfHeight = 0.3f + glm::sin(timeSinceStart * 5) * 0.1f;
			aabb->m_halfWidth = 0.3f + glm::sin(timeSinceStart * 5) * 0.1f;

			Vec2 offset = { aabb->m_halfWidth, -aabb->m_halfHeight };
			shapeTemplates[templateIndex]->m_position = cursorPos + offset;
		}
		else
		{
			shapeTemplates[templateIndex]->m_position = cursorPos;
		}
	}
}

void Spawner::Draw(LineRenderer& lines)
{
	if(spawn != nullptr)
		spawn->Draw(lines);
	else
		shapeTemplates[templateIndex]->Draw(lines);
}

void Spawner::OnLeftClick(Vec2 cursorPos)
{
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
	}
}

void Spawner::OnLeftRelease()
{
	if (spawn != nullptr)
	{
		float r = static_cast<float>(rand()) / (float)(RAND_MAX);
		float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		spawn->m_colour = { r,g,b };
		if (spawn->GetShape() != SHAPE::PLANE)
		{
			spawn->m_velocity = { ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 10), ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 10) };
		}
		spawn->CalculateMassFromArea();
		shapes->push_back(spawn);
	}

	spawn = nullptr;
}

void Spawner::OnRightClick()
{

}

void Spawner::OnRightRelease()
{
	templateIndex += 1;
	if (templateIndex == (int)SHAPE::CONVEX_POLY) // hack whilst implement polys
		templateIndex = 0;
}