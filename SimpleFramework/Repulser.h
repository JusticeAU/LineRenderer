#pragma once
#include "Maths.h"
#include "Particle.h"
#include <vector>

class Repulser
{
public:
	Repulser(Vec2 position, float radius, float force)
		: m_position(position), m_radius(radius), m_force(force) {}

	Vec2 m_position;
	float m_radius;
	float m_force;

	void Update(float delta, std::vector<Particle>& particles)
	{
		//for each particle in list
		for (Particle& p : particles)
		{
			if (PointIsInside(p.m_position))
			{
				Vec2 direction = glm::normalize(p.m_position - m_position);
				p.Applyforce(direction * m_force * delta);
			}
		}
	}

	bool PointIsInside(Vec2 point)
	{
		float distance = glm::distance(m_position, point);
		return distance < m_radius;
	}
};