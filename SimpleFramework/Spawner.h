#pragma once
#include "Maths.h"
#include <vector>

class Shape;
class LineRenderer;

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

	Vec2 LineIntersection(Vec2 A, Vec2 B, Vec2 C, Vec2 D);

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
};