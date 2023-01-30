#pragma once

#include "ApplicationHarness.h"


class Projectile : public Application
{
public:
	Projectile();

	virtual void Update(float delta) override;

	void OnLeftClick() override;

	float totalTime = 0.0f;
	Vec2 pos = { 0,0 };
	Vec2 vel = { 0,0 };
	Vec2 acc = { 0, -9.81f };

	Vec2 leftDownPos;
	bool down = false;
};