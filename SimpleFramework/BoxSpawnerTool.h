#include "SpawnerTool.h"
#include "ConvexPolygon.h"
#include <vector>

class BoxSpawnerTool : public SpawnerTool
{
public:
	BoxSpawnerTool(Toolbox* toolbox) : SpawnerTool(toolbox)
	{

		std::vector<Vec2> boxPoints;
		boxPoints.push_back({ 1,1 });
		boxPoints.push_back({ 1,-1 });
		boxPoints.push_back({ -1,-1 });
		boxPoints.push_back({ -1,1 });

		m_shape = new ConvexPolygon(Vec2(0), 1, boxPoints, { 1,1,1 });
	}
};