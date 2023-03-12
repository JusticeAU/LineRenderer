#pragma once
#include "Shape.h"
#include "AABB.h"
#include <vector>
#include <iostream>

class ConvexPolygon : public Shape
{
public:
	ConvexPolygon(Vec2 position, float inverseMass, std::vector<Vec2> points, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE::CONVEX_POLY, position, inverseMass, colour), m_points(points) {
		RecalculateCentre();
		RecalculateAABB();
		CalculateMassFromArea();
		//CalculateMomentOfInertia();
	}
protected:
	std::vector<Vec2> m_points;

public:
	AABB aabb;

	void Rotate(float degrees) override;
	void Update(float deltaTime) override;
	void Draw(LineRenderer& lines) const override;

	void CalculateMassFromArea() override;
	void CalculateMomentOfInertia() override;
	bool PointInShape(Vec2 point) const;
	bool LineIntersects(Vec2 lineFrom, Vec2 lineTo) const override;
	void Slice(Vec2 lineFrom, Vec2 lineTo, std::vector<Shape*>* shapes) override;

	int GetVertexCount() const { return m_points.size(); }
	Vec2 GetVertexDirection(int vertIndex) const;
	Vec2 GetSurfaceNormal(int vertIndex) const;
	Vec2 GetSurfaceMidPoint(int vertIndex) const;
	Vec2 GetVertexInWorldspace(int vertIndex) const;

	void SetPoints(std::vector<Vec2> points);

	void RecalculateCentre();
	void RecalculateAABB();
	void CalculateMomentOfInertia(LineRenderer& lines);
};