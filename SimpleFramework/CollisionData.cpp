#include "CollisionData.h"
#include "Circle.h"
#include "LineRenderer.h"

void CollisionData::Resolve()
{
	if (IsCollision() != true)
		return;

	shapeA->m_position -= normal * depth / 2.0f;
	shapeB->m_position += normal * depth / 2.0f;

}

void CollisionData::DebugDrawCircleInfo(LineRenderer& lines) const
{
	if (IsCollision())
	{
		lines.SetColour({ 1,0,0 });
		lines.DrawCross(worldPosition - normal * depth / 2.0f, 0.1);
		lines.SetColour({ 0.5,0.5,0 });
		lines.DrawCross(worldPosition, 0.1);
		lines.SetColour({ 0,1,0 });
		lines.DrawCross(worldPosition + normal * depth / 2.0f, 0.1);
		lines.SetColour({ 1,1,1 });
		lines.DrawCircle(worldPosition, depth / 2);
	}
}

void CollisionData::DebugDrawPlaneInfo(LineRenderer& lines) const
{
	lines.SetColour({ 0, 1, 0 });
	lines.DrawLineSegment({ 0,0 }, normal*depth);
}