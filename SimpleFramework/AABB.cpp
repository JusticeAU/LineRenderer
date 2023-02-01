#include "AABB.h"
#include "LineRenderer.h"

void AABB::Draw(LineRenderer& lines) const
{
	float minX = m_position.x - m_halfWidth;
	float maxX = m_position.x + m_halfWidth;
	float minY = m_position.y - m_halfHeight;
	float maxY = m_position.y + m_halfHeight;

	lines.SetColour(m_colour);
	lines.AddPointToLine({ minX, minY });
	lines.AddPointToLine({ minX, maxY });
	lines.AddPointToLine({ maxX, maxY });
	lines.AddPointToLine({ maxX, minY });
	lines.FinishLineLoop();

}