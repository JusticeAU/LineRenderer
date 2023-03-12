#include "Toolbox.h"
#include "Tool.h"

void Toolbox::Update(float delta)
{
	m_tools[m_toolIndex]->Update(delta);
}

void Toolbox::Draw(LineRenderer& lines)
{
	m_tools[m_toolIndex]->Draw(lines);
}

void Toolbox::OnMouseScroll(double delta)
{
	if (m_tools[m_toolIndex]->IsIdle())
	{
		// Handle up and down scroll through different tools.
		if (glm::sign<double>(delta) == 1)
		{
			m_toolIndex += 1;
			if (m_toolIndex == m_tools.size())
				m_toolIndex = 0;
		}
		else if (glm::sign<double>(delta) == -1)
		{
			m_toolIndex -= 1;
			if (m_toolIndex == -1)
				m_toolIndex = m_tools.size() - 1;
		}
	}
}

void Toolbox::OnLeftClick()
{
	m_tools[m_toolIndex]->OnLeftClick();
}

void Toolbox::OnLeftUp()
{
	m_tools[m_toolIndex]->OnLeftUp();
}

void Toolbox::OnRightClick()
{
	m_tools[m_toolIndex]->OnRightClick();
}

void Toolbox::OnRightUp()
{
	m_tools[m_toolIndex]->OnRightUp();
}

void Toolbox::OnMiddleClick()
{
	m_tools[m_toolIndex]->OnMiddleClick();
}

void Toolbox::OnMiddleUp()
{
	m_tools[m_toolIndex]->OnMiddleUp();
}
