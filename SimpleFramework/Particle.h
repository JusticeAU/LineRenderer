#pragma once
#include "Maths.h"

class Particle
{
public:
	Particle(Vec2 position, Vec2 velocity, float size, Vec3 colour = { 1, 1, 1 })
	{
		m_position = position;
		m_velocity = velocity;
		m_radius = size;
		m_colour = colour;
		m_lifeTime = 10.0f;
	}

public:
	Vec2 m_position = { 0,0 };
	Vec2 m_velocity = { 0,0 };
	float m_radius = 0;
	Vec3 m_colour = { 1, 1, 1 };
	float m_lifeTime = 0.0f;
private:
	Vec2 m_gravity = { 0, -9.81 };
	//Vec2 m_gravity = { 0, 0 };

public:

	void Update(float deltaTime)
	{
		m_velocity += m_gravity * deltaTime;
		m_position += m_velocity * deltaTime;
		m_lifeTime -= deltaTime;
	}

	void Applyforce(Vec2 force)
	{
		m_velocity += force;
	}
};