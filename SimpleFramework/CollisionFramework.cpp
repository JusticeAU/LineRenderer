#include "CollisionFramework.h"
#include "CollisionData.h"
#include "CollisionFunctions.h"
#include "Spawner.h"

CollisionFramework::CollisionFramework()
{
	// Create our spawner tools and give it a reference to the vector of world shapes.
	spawner = new Spawner(&shapes);

	// Set up world border planes
	shapes.push_back(new Plane({ 0,1 }, -9, { 1,1,1 }));
	shapes.push_back(new Plane({ 1,0 }, -16, { 1,1,1 }));
	shapes.push_back(new Plane({ -1,0 }, -16, { 1,1,1 }));

}

void CollisionFramework::Update(float delta)
{
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
			shape->ApplyImpulse(toCursor * (1.0f / shape->GetInverseMass()));
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
				col.Resolve();
		}
	}

	// Draw all shapes in simulation.
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