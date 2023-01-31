#include "Application.h"
#include "Circle.h"

class CircleCollision : public Application
{
public:
	CircleCollision();

	Circle* m_CircleA = new Circle({-5,0}, 2.0f);
	Circle* m_CircleB = new Circle({ 5,0 }, 1.5f);

	void Update(float delta) override;
};