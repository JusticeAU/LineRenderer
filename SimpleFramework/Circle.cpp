#include "Circle.h"
#include "CollisionData.h"
#include "LineRenderer.h"

void Circle::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);
	lines.DrawCircle(m_position, m_radius);
}