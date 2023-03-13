#pragma once
#include "Tool.h"
#include "Circle.h"

class Toolbox;

class SpawnerTool : public Tool
{
public:
	SpawnerTool(Toolbox* toolbox) : Tool(toolbox) {}
	virtual void Update(float delta) override;
	virtual void Draw(LineRenderer& lines)override;

	virtual void OnLeftClick();
	virtual void OnLeftUp();
	virtual void OnRightClick();
	virtual void OnRightUp();

protected:
	Shape* m_shape;
	Vec2 m_cursorDownPos;
};