#pragma once
#include "Maths.h"
#include "LineRenderer.h"

class Application
{
public:
	virtual void Update(float delta) = 0;

	bool leftMouseDown;
	bool rightMouseDown;
	bool middleMouseDown;
	Vec2 cursorPos;
	LineRenderer* lines = nullptr;

	virtual void OnLeftClick() {}
	virtual void OnLeftRelease() {}
	virtual void OnRightClick() {}
	virtual void OnRightRelease() {}
	virtual void OnMiddleClick() {}
	virtual void OnMiddleRelease() {}
	virtual void OnMouseScroll(double delta) {}

};