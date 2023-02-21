#pragma once
#include "Shape.h"
#include "AABB.h"
#include <vector>

class ConvexPolygon : public Shape
{
public:
	ConvexPolygon(Vec2 position, float inverseMass, std::vector<Vec2> points, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE::CONVEX_POLY, position, inverseMass, colour), m_points(points) {
		RecalculateCentroid();
		UpdateAABB();
	}

	std::vector<Vec2> m_points;
	AABB aabb;

	void Update(float deltaTime) override;
	void Draw(LineRenderer& lines) const override;

	void CalculateMassFromArea() override;
	bool PointInShape(Vec2 point);
	bool LineIntersects(Vec2 lineFrom, Vec2 lineTo) override;
	void Slice(Vec2 lineFrom, Vec2 lineTo, std::vector<Shape*>* shapes) override;

	Vec2 GetVertexDirection(int vertIndex) const;
	Vec2 GetSurfaceNormal(int vertIndex) const;
	Vec2 GetSurfaceMidPoint(int vertIndex) const;
	Vec2 GetVertexInWorldspace(int vertIndex) const;

	void RecalculateCentroid();
	void UpdateAABB();

protected:
};