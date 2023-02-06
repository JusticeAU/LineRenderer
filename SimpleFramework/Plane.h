#include "Shape.h"
#include "Maths.h"
class Plane : public Shape
{
	Vec2 m_normal;
	float m_distance;
public:
	Plane(Vec2 position, Vec2 normal, float distance, Vec3 colour);


	void Draw(LineRenderer& lines) const override;
};