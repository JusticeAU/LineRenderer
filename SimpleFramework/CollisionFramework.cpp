#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{

	// Set up template shapes
	shapeTemplates.push_back(new Circle(cursorPos, 1, 0, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new AABB(cursorPos, 1, 1.0f, 0, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new Plane(cursorPos, 1, { 0.2f,0.2f,0.2f }));


	// Set up world border planes
	shapes.push_back(new Plane({ 0,1 }, -10, { 1,1,1 }));
	//shapes.push_back(new Plane({ 0,-1 }, -10, { 1,1,1 }));
	//shapes.push_back(new Plane({ 1,0 }, -10, { 1,1,1 }));
	//shapes.push_back(new Plane({ -1,0 }, -10, { 1,1,1 }));

}

void CollisionFramework::Update(float delta)
{
	timeSinceStart += delta;

	// Update all primitives
	for (auto& shape : shapes)
		shape->Update(delta);

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


	// Perform collision tests and resolution 1 pass against all shapes.
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

	ProcessObjectSpawner();
}

void CollisionFramework::OnLeftClick()
{
	std::cout << "left down" << std::endl;

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

void CollisionFramework::OnLeftRelease()
{
	std::cout << "left release" << std::endl;
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
		shapes.push_back(spawn);
	}

	spawn = nullptr;
}

void CollisionFramework::OnRightClick()
{
	std::cout << "right click" << std::endl;
}

void CollisionFramework::OnRightRelease()
{
	std::cout << "right release" << std::endl;
	templateIndex += 1;
	if (templateIndex == (int)SHAPE::COUNT)
		templateIndex = 0;
}

void CollisionFramework::ProcessObjectSpawner()
{
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
		for (int i = 0; i < MAX_COLLISION_PASSES; i++)
		{
			std::vector<CollisionData> collisions;
			for (int b = 0; b < shapes.size(); b++)
			{
				CollisionData col = TestCollisions(spawn, shapes[b]);
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
		spawn->Draw(*lines);
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
		shapeTemplates[templateIndex]->Draw(*lines);
	}
}