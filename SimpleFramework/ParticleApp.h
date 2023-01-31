#pragma once
#include "Application.h"
#include "Particle.h"
#include "Repulser.h"
#include <vector>
#include <random>

class ParticleApp : public Application
{
public:
	ParticleApp();

	void Update(float deltatime) override;

	std::vector<Particle> m_particles;
	std::vector<Repulser> m_repulsers;
	bool rightIsDown = false;
};

