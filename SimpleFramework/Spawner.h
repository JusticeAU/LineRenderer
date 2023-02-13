#pragma once
#include "Maths.h"
#include <vector>

class Shape;
class LineRenderer;

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

protected:
	std::vector<Shape*>* shapes = nullptr;
	std::vector<Shape*> shapeTemplates;
	int templateIndex = 0;
	//bool spawnStarted = false;
	Shape* spawn = nullptr;
	Vec2 spawnStartPos = Vec2(0);
	float timeSinceStart = 0.0f;
};