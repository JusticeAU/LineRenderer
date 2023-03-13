#include "SpawnerTool.h"
#include "Circle.h"
#include "Toolbox.h"
#include "LineRenderer.h"

void SpawnerTool::Update(float delta)
{
	if(m_idle)
		m_shape->m_position = m_toolbox->m_cursorPos;
}

void SpawnerTool::Draw(LineRenderer& lines)
{
	m_shape->Draw(lines);
	if (!m_idle)
	{
		lines.DrawLineSegment(m_cursorDownPos, m_toolbox->m_cursorPos);
	}
}

void SpawnerTool::OnLeftClick()
{
	m_idle = false;
	m_cursorDownPos = m_toolbox->m_cursorPos;
}

void SpawnerTool::OnLeftUp()
{
	m_idle = true;
	
	// Clone the template
	Shape* spawn = m_shape->Clone();

	// Random Colour
	float r = static_cast<float>(rand()) / (float)(RAND_MAX);
	float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	spawn->m_colour = { r,g,b };
	
	// Set velocity of new shape based on cursor slingshot.
	spawn->m_velocity = (m_cursorDownPos - m_toolbox->m_cursorPos) * 5.0f;
	
	// add to physics simulation.
	m_toolbox->SpawnShape(spawn);
}

void SpawnerTool::OnRightClick()
{
}

void SpawnerTool::OnRightUp()
{
}
