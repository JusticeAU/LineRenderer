#pragma once
#include "Maths.h"
#include <vector>

class Shape;
class LineRenderer;
class ConvexPolygon;

enum class SPAWNER_STATE
{
	IDLE,
	BUILD,
	LAUNCH,
	GRAB,
	CUT
};

// SPAWN_ indexes should align with with the shapeTemplates vector and the SHAPES enum
enum class SPAWNER_TOOL
{
	SPAWN_CIRCLE,
	SPAWN_AABB,
	SPAWN_CONVEX_POLY,
	SPAWN_PLANE,
	LINE_CUTTER,

	COUNT
};

class Spawner
{
public:
	Spawner(std::vector<Shape*>* shapes);

	void Update(float delta, Vec2 cursorPos);
	void Draw(LineRenderer& lines);
	
	void OnLeftClick(Vec2 cursorPos);
	void OnLeftRelease();
	void OnRightClick();
	void OnRightRelease();
	void OnMouseScroll(double delta);

	void DoPolygonConstructionUpdate(float delta, Vec2 cursorPos);
	void DoPolygonConstructionDraw(LineRenderer& lines);

	static bool LineIntersection(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Vec2* i = nullptr)
	{

		// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		Vec2 s1(0);
		Vec2 s2(0);
		s1.x = p1.x - p0.x;     s1.y = p1.y - p0.y;
		s2.x = p3.x - p2.x;     s2.y = p3.y - p2.y;

		float s, t;
		s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
		t = (s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			// Collision detected
			if (i != nullptr)
			{
				i->x = p0.x + (t * s1.x);
				i->y = p0.y + (t * s1.y);

			}
			return true;
		}

		return false; // No collision
	}

protected:
	Vec3 templateColour = { 0.2f,0.2f,0.2f };
	std::vector<Shape*>* shapes = nullptr;
	std::vector<Shape*> shapeTemplates;
	int selectedTool = 0;
	Shape* spawn = nullptr;
	float timeSinceStart = 0.0f;
	SPAWNER_STATE state = SPAWNER_STATE::IDLE;;

	Vec2 cursorPos = Vec2(0);
	Vec2 cursorDownPos = Vec2(0);

	// Poly Spawner shit
	std::vector<Vec2> spawningVerts;
	Vec2 potentialVert;

	// Grabbeed Shape
	Shape* grabbed = nullptr;
	float grabbedInverseMass = 0.0f;

	// Cutter test
	ConvexPolygon* testPoly = nullptr;
};