#include "Application.hpp"

#include <iostream>
#include <cmath>

#include "States/MandelbrotState.hpp"

namespace ng {

    // constructor / destructor
    Application::Application() :
        m_window(sf::VideoMode(800, 800), "SFML", sf::Style::Close) {

        // window
        m_window.setFramerateLimit(60);
        m_window.setVerticalSyncEnabled(true);

        // font
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("Application::Application: failed to load font");

        // text
        m_statisticsText.setFont(m_font);
        m_statisticsText.setPosition(std::floor(5.f), std::floor(5.f));
        m_statisticsText.setCharacterSize(20);
        m_statisticsText.setFillColor(sf::Color::White);

        // states
        m_states.emplace(std::make_unique<MandelbrotState>());
    }

    // public methods
    void Application::run() {
        while (m_window.isOpen()) {
            updateFrameTime();
            pollEvent();
            update();
            render();
        }
    }

    // member methods
    void Application::updateFrameTime() {
        m_lastFrameTime = m_frameTime;
        m_frameTime = m_clock.restart();
    }

    void Application::pollEvent() {
        sf::Event event{};

        while (m_window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    m_window.close();
                    break;
                default:
                    break;
            }
        }
    }

    void Application::update() {
        if (!m_states.empty())
            m_states.top()->update(m_frameTime.asSeconds());

        m_statisticsText.setString(
            "FPS: " + std::to_string((1.f / m_frameTime.asSeconds())) + "\n" +
            "time: " + std::to_string(m_frameTime.asSeconds())
        );
    }

    void Application::render() {
        m_window.clear();

        if (!m_states.empty())
            m_states.top()->render(m_window);

        m_window.draw(m_statisticsText);

        m_window.display();
    }

} // namespace ng