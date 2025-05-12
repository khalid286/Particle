#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;

	//vector for Particles
	vector<Particle> m_particles;

	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();
	Vector2i m_lastParticlePosition = Vector2i(-1, -1);
	int m_particleCount = 0;
	bool m_showCount = false;
	Text m_particleCountText;
	Font m_font;


public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};
