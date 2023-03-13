#pragma once
#include "SpawnerTool.h"
#include "Plane.h"

class LineRenderer;

class PlaneDrawerTool : public Tool
{
public:
	PlaneDrawerTool(Toolbox* toolbox);
	void Update(float delta) override;
	void Draw(LineRenderer& lines) override;

	void OnLeftClick() override;
protected:
	Plane* m_plane = nullptr;
};