#include "Circle.h"
#include "CollisionData.h"
#include "LineRenderer.h"
#include "glm.hpp"
#include "ConvexPolygon.h"

void Circle::CalculateMassFromArea()
{
	float mass = glm::pi<float>() * (m_radius * m_radius);
	m_inverseMass = 1.0f / mass;
}

void Circle::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);
	lines.DrawCircle(m_position, m_radius, 18);
}

// Returns true if the position supplied is inside the circle
bool Circle::PointInShape(Vec2 point) const
{
	return glm::distance(m_position, point) < m_radius;
}

// returns true if a line from a to b pass through the circle completely (has two points of intersection)
bool Circle::LineIntersects(Vec2 a, Vec2 b) const
{
	Vec2 first, second;
	float lengthAB = glm::distance(a, b);

	// Compute direction vector from A to B
	Vec2 D = (b - a) / lengthAB;

	// Compute the distance between the points A and E, where E is the point of AB closest to circle centre
	float t = D.x * (m_position.x - a.x) + D.y * (m_position.y - a.y);

	// Compute the coordiantes of point E
	Vec2 E = {
		t * D.x + a.x,
		t * D.y + a.y };

	// Compute distance between E and C
	float lengthEC = glm::distance(E, m_position);

	// Test if the line intersects the circle
	if (lengthEC < m_radius)
	{
		// Compute distance from t to circle intersection point
		float dt = glm::sqrt((m_radius * m_radius) - (lengthEC * lengthEC));

		// Compute first intersection point
		first = {
			(t - dt) * D.x + a.x,
			(t - dt) * D.y + a.y
		};

		// Compute second intersection point
		second = {
			(t + dt) * D.x + a.x,
			(t + dt) * D.y + a.y
		};

		// if second collision point is further away from A than B is, then the line didnt slice through
		float lengthASecond = glm::length(second - a);
		if (lengthASecond > lengthAB)
			return false;

		return true;
	}
	else // line doesnt touch circle
		return false;
}
// Converts the Circle to a Polygon and calls the Polygon Slice function on it.
// If the line from a to B does not intersect the circle, this will still convert the Circle to a Polygon.
void Circle::Slice(Vec2 a, Vec2 b, std::vector<Shape*>* shapes)
{
	// Create a circle polygon based on current circle
	const int segmentCount = 18;
	float cosAngle = cos(2 * 3.14159f / segmentCount);
	float sinAngle = sin(2 * 3.14159f / segmentCount);

	glm::mat2 rotMat = { {cosAngle, -sinAngle},{sinAngle, cosAngle} };

	Vec2 plotPoint(0, m_radius);

	std::vector<Vec2> points;
	for (int i = 0; i <= segmentCount; i++)
	{
		plotPoint = rotMat * plotPoint;
		points.push_back(plotPoint);
	}
	// Construct the Polygon instances from the points, mark this circle for deletion and push the new Polygon on to the shapes vector.
	ConvexPolygon* poly = new ConvexPolygon(m_position, m_inverseMass, points, m_colour);
	poly->m_velocity = m_velocity;
	shapes->push_back(poly);
	poly->Slice(a, b, shapes);
	m_toBeDeleted = true;
}

Shape* Circle::Clone()
{
	Circle* clone = new Circle(m_position, m_radius, m_inverseMass, m_colour);
	return (Shape*)clone;
}