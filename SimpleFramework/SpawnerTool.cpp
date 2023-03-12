#include "SpawnerTool.h"
#include "Circle.h"
#include "Toolbox.h"
#include "LineRenderer.h"

SpawnerTool::SpawnerTool(Toolbox* toolbox) : Tool(toolbox)
{
	m_shape = new Circle({ 0,0 }, 1.0f, 1.0f);
}

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
	Circle* spawn = new Circle(*(Circle*)m_shape);

	// Random Colour
	float r = static_cast<float>(rand()) / (float)(RAND_MAX);
	float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	spawn->m_colour = { r,g,b };
	
	spawn->m_velocity = (m_cursorDownPos - m_toolbox->m_cursorPos) * 5.0f;
	
	m_toolbox->SpawnShape(spawn);
}

void SpawnerTool::OnRightClick()
{
}

void SpawnerTool::OnRightUp()
{
}
