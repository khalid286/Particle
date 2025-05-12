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
    m_Window.create(VideoMode::getDesktopMode(), "Particle Generator", Style::Default);
    //Load the font
    if (!m_font.loadFromFile("Font.ttf")) {
        cout << "Error loading font!" << endl;
    }
    m_particleCountText.setFont(m_font);
    m_particleCountText.setCharacterSize(100);
    m_particleCountText.setFillColor(Color::White);
    m_particleCountText.setPosition(200, 200);

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

        // Initial click event to generate a particle
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            m_showCount = false;  // Hide the particle count when clicking
            m_particleCount = 0;  // Reset particle count

            Vector2i mousePos = Mouse::getPosition(m_Window);

            for (int i = 0; i < 5; i++) {
                int numPoints = rand() % 26 + 25;
                m_particles.emplace_back(m_Window, numPoints, mousePos);
                m_particleCount++;
            }

            m_lastParticlePosition = mousePos;
        }

        // Mouse released event
        if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
            m_showCount = true;  // Show the particle count when releasing the mouse
            m_lastParticlePosition = Vector2i(-1, -1);
        }
    }

    // Continuous particle generation while holding down the left mouse button
    if (Mouse::isButtonPressed(Mouse::Left)) {
        Vector2i mousePos = Mouse::getPosition(m_Window);

        // Check for sufficient movement to prevent excessive particle creation
        if (m_lastParticlePosition == Vector2i(-1, -1) ||
            abs(mousePos.x - m_lastParticlePosition.x) > 5 ||
            abs(mousePos.y - m_lastParticlePosition.y) > 5) {

            for (int i = 0; i < 3; i++) {
                int numPoints = rand() % 26 + 25;
                m_particles.emplace_back(m_Window, numPoints, mousePos);
                m_particleCount++;
            }

            m_lastParticlePosition = mousePos;
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

    // Display particle count only when mouse is released
    if (m_showCount) {
        m_particleCountText.setString("Particles Created: " + to_string(m_particleCount));
        m_Window.draw(m_particleCountText);
    }

    m_Window.display();
}
