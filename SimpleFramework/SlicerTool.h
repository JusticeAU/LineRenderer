#pragma once
#include "Tool.h"
#include "Maths.h"

const int CURSOR_BUFFER = 5;
class SlicerTool : public Tool
{
public:
	SlicerTool(Toolbox* toolbox) : Tool(toolbox) {}
	void Update(float delta) override;
	void Draw(LineRenderer& lines) override;

	void OnLeftClick() override;
	void OnLeftUp() override;
protected:
	Vec2 m_cursorPosPrevious;
	Vec2 m_oldCursorPositions[CURSOR_BUFFER] = { Vec2(0),Vec2(0),Vec2(0),Vec2(0),Vec2(0) };
};