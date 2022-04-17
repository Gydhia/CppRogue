#include "AppTester.hpp"

// SFML
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

// ImGui
#include "imgui.h"

// ImGui-SFML
#include "imgui-SFML.h"

// nlohmann_json
#include "nlohmann/json.hpp"

#include <cassert>
#include <iostream>

void testJson()
{
    // Parse a string literal
    nlohmann::json j = R"({"happy": true, "pi": 3.141 })"_json;

    // Parse a raw string literal
    auto j2 = R"(
      {
        "happy": true,
        "pi": 3.141
      }
    )"_json;
    assert(j == j2);

    // Parse an initializer list
    nlohmann::json j3 = {{"pi", 3.141},
                         {"happy", true},
                         {"name", "Niels"},
                         {"nothing", nullptr},
                         {"answer", {{"everything", 42}}},
                         {"list", {1, 0, 2}},
                         {"object", {{"currency", "USD"}, {"value", 42.99}}}};

    std::cout << "JSON library correctly setup\n";
}

void testSfml()
{
    sf::RenderWindow window{sf::VideoMode(1280, 720), "ImGui + SFML"};
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape{100.F};
    shape.setFillColor(sf::Color::Green);

    std::cout << "SFML library correctly setup\n";
    std::cout << "Imgui library correctly setup\n";

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) { window.close(); }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        static bool show{true};
        ImGui::ShowDemoWindow(&show);

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
