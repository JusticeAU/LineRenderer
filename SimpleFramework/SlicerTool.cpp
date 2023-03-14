#include "SlicerTool.h"
#include "Toolbox.h"
#include "LineRenderer.h"

void SlicerTool::Update(float delta)
{
	if (m_idle)
	{
		for (int i = 0; i < CURSOR_BUFFER-1; i++)
		{
			m_oldCursorPositions[i] = m_oldCursorPositions[i + 1];
		}
		m_oldCursorPositions[CURSOR_BUFFER-1] = m_toolbox->m_cursorPos;
	}
	else
	{
		m_oldCursorPositions[CURSOR_BUFFER - 1] = m_toolbox->m_cursorPos;
	}
}

void SlicerTool::Draw(LineRenderer& lines)
{
	lines.DrawLineSegment(m_oldCursorPositions[0], m_oldCursorPositions[CURSOR_BUFFER - 1], {1,0,0});
}

void SlicerTool::OnLeftClick()
{
	m_idle = false;
}

void SlicerTool::OnLeftUp()
{
	m_idle = true;
}