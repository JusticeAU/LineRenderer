#include "CollisionFramework.h"
#include "CollisionData.h"

CollisionFramework::CollisionFramework()
{

}

void CollisionFramework::Update(float delta)
{

	if (leftMouseDown)
		m_CircleA->m_position = cursorPos;
	else if (rightMouseDown)
		m_CircleB->m_position = cursorPos;

	CollisionData col = m_CircleA->CollisionWithCircle(m_CircleB);
	col.Resolve();
	col.DebugDraw(*lines);

	//Draw
	m_CircleA->Draw(*lines);
	m_CircleB->Draw(*lines);

}