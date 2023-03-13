#pragma once
#include "SpawnerTool.h"

class CircleSpawnerTool : public SpawnerTool
{
public:
	CircleSpawnerTool(Toolbox* toolbox) : SpawnerTool(toolbox)
	{
		m_shape = new Circle({ 0,0 }, 1.0f, 1.0f);
	}
};