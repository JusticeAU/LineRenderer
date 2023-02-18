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
	GRAB
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
	int templateIndex = 0;
	Shape* spawn = nullptr;
	Vec2 spawnStartPos = Vec2(0);
	float timeSinceStart = 0.0f;
	SPAWNER_STATE state = SPAWNER_STATE::IDLE;

	Vec2 cursorPos;

	// Poly Spawner shit
	std::vector<Vec2> spawningVerts;
	Vec2 potentialVert;
};