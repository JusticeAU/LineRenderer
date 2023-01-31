#include "ParticleApp.h"
#include <algorithm>
const int PARTICLE_SPAWN_COUNT = 4;

ParticleApp::ParticleApp()
{
	/// eyy constructo

	
}

void ParticleApp::Update(float deltaTime)
{
	// Eyyy we're updating
	// Update physics
	if (leftMouseDown)
	{
		for (int i = 0; i < PARTICLE_SPAWN_COUNT; i++)
		{
			float x = static_cast<float>((rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 2;
			float y = static_cast<float>((rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 2;

			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);


			m_particles.push_back(Particle(cursorPos, {x,y}, 0.1f, {r,g,b}));

		}
	}
	if (rightMouseDown && !rightIsDown)
	{
		rightIsDown = true;
		// check if we're above a repulser and if so remove it, otherwise add one to the world.
		Vec2 pos = cursorPos;
		std::vector<Repulser>::iterator repulser = std::find_if(m_repulsers.begin(), m_repulsers.end(), [&pos]( Repulser& r) { return r.PointIsInside(pos); });
		if (repulser != m_repulsers.end())
		{
			m_repulsers.erase(repulser);
		}
		else
			m_repulsers.push_back(Repulser(cursorPos, 1, 1));
	}

	if (!rightMouseDown)
		rightIsDown = false;


	// Draw but also update in here for now
	for(Particle& p : m_particles)
	{
		p.Update(deltaTime);
		lines->SetColour(p.m_colour);
		//lines->DrawCircle(p.m_position, p.m_radius, 4);
		lines->DrawCross(p.m_position, p.m_radius);
	}

	// Clear out any old particles
	std::vector<Particle>::iterator toRemove = std::remove_if(m_particles.begin(), m_particles.end(), [](Particle& p) { return p.m_lifeTime <= 0; });
	m_particles.erase(toRemove, m_particles.end());

	// draw repulsers
	lines->SetColour({ 1,1,1 });
	for (Repulser& r : m_repulsers)
	{
		r.Update(deltaTime, m_particles);
		lines->DrawCircle(r.m_position, r.m_radius);
	}
}
