#pragma once

#include "Application.h"


class Example : public Application
{
public:
	Example();

	virtual void Update(float delta) override;

	void OnLeftClick() override;

	float totalTime = 0.0f;
};