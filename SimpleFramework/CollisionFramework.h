#include "Application.h"
#include "Circle.h"

class CollisionFramework : public Application
{
public:
	CollisionFramework();

	Circle* m_CircleA = new Circle({-5,0}, 2.0f, {1,0,0});
	Circle* m_CircleB = new Circle({ 5,0 }, 1.5f, {0,1,0});

	void Update(float delta) override;
};