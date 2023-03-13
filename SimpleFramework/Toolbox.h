#pragma once
#include "Maths.h"
#include <vector>
#include "CircleSpawnerTool.h"
#include "BoxSpawnerTool.h"
#include "PolySpawnerTool.h"
#include "PlaneDrawerTool.h"


class Shape;
class Tool;
class LineRenderer;

class Toolbox
{
public:
	Toolbox(std::vector<Shape*>* shapes, Vec2& cursorPos) : m_shapes(shapes), m_cursorPos(cursorPos)
	{
		m_tools.push_back(new CircleSpawnerTool(this));
		m_tools.push_back(new BoxSpawnerTool(this));
		m_tools.push_back(new PolySpawnerTool(this));
		m_tools.push_back(new PlaneDrawerTool(this));
	}

	void Update(float delta);
	void Draw(LineRenderer& lines);

	void OnMouseScroll(double delta);

	void OnLeftClick();
	void OnLeftUp();
	void OnRightClick();
	void OnRightUp();
	void OnMiddleClick();
	void OnMiddleUp();

	void AddTool(Tool* tool) { m_tools.push_back(tool); }
	void SpawnShape(Shape* shape) { m_shapes->push_back(shape); }
	
	Vec2& m_cursorPos;
protected:
	std::vector<Shape*>* m_shapes;
	std::vector<Tool*> m_tools;
	int m_toolIndex = 0;
};