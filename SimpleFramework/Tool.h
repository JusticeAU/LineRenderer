#pragma once

class LineRenderer;
class Toolbox;

class Tool
{
public:
	Tool(Toolbox* toolbox) : m_toolbox(toolbox) {};
	virtual void Update(float delta) = 0;
	virtual void Draw(LineRenderer& lines) = 0;
	
	virtual void OnLeftClick() {};
	virtual void OnLeftUp() {};
	virtual void OnRightClick() {};
	virtual void OnRightUp() {};
	virtual void OnMiddleClick() {};
	virtual void OnMiddleUp() {};

	virtual bool IsIdle() { return m_idle; }
protected:
	Toolbox* m_toolbox;
	bool m_idle = true;
};