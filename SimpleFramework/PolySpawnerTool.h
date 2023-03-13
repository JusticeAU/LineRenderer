#pragma once
#include "SpawnerTool.h"
#include "ConvexPolygon.h"
#include <vector>

class PolySpawnerTool : public SpawnerTool
{
public:
	PolySpawnerTool(Toolbox* toolbox) : SpawnerTool(toolbox)
	{

		std::vector<Vec2> boxPoints;
		boxPoints.push_back({ 0,1 });
		boxPoints.push_back({ 1,0 });
		boxPoints.push_back({ 0,-1 });
		boxPoints.push_back({ -1,0 });

		m_shape = new ConvexPolygon(Vec2(0), 1, boxPoints, { 1,1,1 });
	}
};