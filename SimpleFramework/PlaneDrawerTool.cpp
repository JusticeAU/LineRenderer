#include "PlaneDrawerTool.h"
#include "Toolbox.h"

PlaneDrawerTool::PlaneDrawerTool(Toolbox* toolbox) : Tool(toolbox)
{
	m_plane = new Plane(Vec2(0), 0.0f, { 1,1,1 });
}

void PlaneDrawerTool::Update(float delta)
{
	m_plane->m_distance = -glm::length(m_toolbox->m_cursorPos);
	m_plane->m_normal = -glm::normalize(m_toolbox->m_cursorPos);
	
}

void PlaneDrawerTool::Draw(LineRenderer& lines)
{
	m_plane->Draw(lines);
}


void PlaneDrawerTool::OnLeftClick()
{
	m_toolbox->SpawnShape(m_plane->Clone());
}