#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"

#include <iostream>

CollisionFramework::CollisionFramework()
{
	shapeTemplates.push_back(new Circle(cursorPos, 1, 0, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new Plane(cursorPos, 1, { 0.2f,0.2f,0.2f }));
	shapeTemplates.push_back(new AABB(cursorPos, 1, 1.0f, 0, { 0.2f,0.2f,0.2f }));

	Circle* circle1 = new Circle({ -3, 0 }, 1, 1.0f);
	circle1->m_velocity = { 1,0 };
	Circle* circle2 = new Circle({ 3, 0 }, 1, 0);

	shapes.push_back(circle1);
	shapes.push_back(circle2);
	//shapes.push_back(new Plane({ 0,1 }, 10, { 1,1,1 }));
	//shapes.push_back(new Plane({ 0,-1 }, 10, { 1,1,1 }));
	//shapes.push_back(new Plane({ 1,0 }, 10, { 1,1,1 }));
	//shapes.push_back(new Plane({ -1,0 }, 10, { 1,1,1 }));



}

void CollisionFramework::Update(float delta)
{
	// Update all primitives
	for (auto& shape : shapes)
		shape->Update(delta);


	// Perform collision tests and resolution 1 pass against all shapes.
	for (int i = 0; i < MAX_COLLISION_PASSES; i++)
	{
		std::vector<CollisionData> collisions;
		for (int a = 0; a+1 < shapes.size(); a++)
		{
			for (int b = a + 1; b < shapes.size(); b++)
			{
				CollisionData col = TestCollisions(shapes[a], shapes[b]);
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
				col.Resolve();
			}
		}
	}

	//Draw
	for (auto s : shapes)
	{
		s->Draw(*lines);
	}

	// Handle Template Spawning
	if (spawn != nullptr)
	{
		if (spawn->GetShape() == SHAPE::CIRCLE)
		{
			Circle* circle = (Circle*)spawn;
			circle->m_radius = glm::distance(spawn->m_position, cursorPos);
			if (circle->m_radius == 0.0f)
				circle->m_radius = 1.0f;
			
		}
		else if (spawn->GetShape() == SHAPE::AABB)
		{
			AABB* aabb = (AABB*)spawn;
			aabb->m_halfHeight = glm::abs(spawn->m_position.y - cursorPos.y);
			if (aabb->m_halfHeight == 0.0f)
				aabb->m_halfHeight = 0.5f;
			aabb->m_halfWidth = glm::abs(spawn->m_position.x - cursorPos.x);
			if (aabb->m_halfWidth == 0.0f)
				aabb->m_halfWidth = 0.5f;
		}
		else if (spawn->GetShape() == SHAPE::PLANE)
		{
			Plane* plane = static_cast<Plane*>(spawn);
			plane->m_normal = glm::normalize(cursorPos);
			plane->m_distance = glm::length(cursorPos);
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
		if (shapeTemplates[templateIndex]->GetShape() == SHAPE::PLANE)
		{
			Plane* plane = static_cast<Plane*>(shapeTemplates[templateIndex]);
			plane->m_normal = glm::normalize(cursorPos);
			plane->m_distance = glm::length(cursorPos);
		}
		else
		{
			shapeTemplates[templateIndex]->m_position = cursorPos;
		}
		shapeTemplates[templateIndex]->Draw(*lines);
	}
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
		aabb->m_position = cursorPos;
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
		spawn->m_velocity = { ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 10), ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 10) };
		spawn->m_inverseMass = 1.0f;
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
