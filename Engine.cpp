#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace sf;
using namespace std;

// Constructor to initialize the engine
Engine::Engine() {
    // Create the render window with a custom resolution or desktop mode
    m_Window.create(VideoMode::getDesktopMode(), "Particle Generator", Style::Fullscreen);
}

// Main run function
void Engine::run() {
    Clock clock;

    // Run unit tests
    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete. Starting engine..." << endl;

    while (m_Window.isOpen()) {
        float dtAsSeconds = clock.restart().asSeconds();

        input();
        update(dtAsSeconds);
        draw();
    }
}

// Handle user input
void Engine::input() {
    Event event;
    while (m_Window.pollEvent(event)) {
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
            m_Window.close();
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(m_Window);
            for (int i = 0; i < 5; i++) {
                int numPoints = rand() % 26 + 25;
                m_particles.emplace_back(m_Window, numPoints, mousePos);
            }
        }
    }
}

// Update particles
void Engine::update(float dtAsSeconds) {
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        if (it->getTTL() > 0.0f) {
            it->update(dtAsSeconds);
            ++it;
        }
        else {
            it = m_particles.erase(it);
        }
    }
}

// Draw particles
void Engine::draw() {
    m_Window.clear();

    for (const auto& particle : m_particles) {
        m_Window.draw(particle);
    }

    m_Window.display();
}