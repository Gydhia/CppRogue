#include "CppRogue.hpp"

#include "ContentManager.hpp"
#include "engine/core/GameArena.hpp"
#include "engine/core/utility/IO.hpp"
#include "engine/entity/monster/Monster.hpp"

// SFML
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
// ImGui
#include "imgui.h"
// ImGui-SFML
#include "imgui-SFML.h"

#include <iostream>

// @TODO
// @TODO Construire un monstre de chaque "Breed" (charg�e depuis le JSON)
// @TODO Afficher chaque monstre sur la carte
// @TODO


// @TODO
// @TODO Refactoring de l'affichage de la carte et des entit�s 
// @TODO "GameArena" est aussi responsable de l'affichage des entit�s
// @TODO


// @TODO
// @TODO Permettre de faire avancer notre h�ro avec les touches du clavier et l'action "Walk"
// @TODO Voir la documentation de SFML pour "sf::Keyboard" dans les 4 directions cardinales
// @TODO
// @TODO V�rifier que notre h�ro :
//   - ne peux pas sortir de la zone de jeu
//   - ne peux pas passer � travers les murs ou aller dans l'eau
// @TODO

// @TODO
// @TODO Permettre de modifier la motilit� du h�ro 
//   - PageUp pour activer / d�sactiver la capacit� de voler
//   - PageDown pour activer / d�sactiver la capacit� de nager
// @TODO
// @TODO V�rifier que la modification des capacit�s impacte la mobilit� du h�ro
//   - doit pouvoir passer au dessus des murs et de l'eau en volant
//   - doit pouvoir aller dans l'eau en nageant
// @TODO


namespace cppRogue {

void run_game()
{
    // Setup windows size based on map size
    sf::RenderWindow window{sf::VideoMode(36 * 32, 28 * 32), "CppRogue"};
    window.setFramerateLimit(30);
    ImGui::SFML::Init(window);

    // Load game content
    ContentManager gameData{};
    if (!gameData.load("data/content.json"))
    {
        std::cerr << "Game content loading failed !" << '\n';
        return;
    }

    // Setup game arena
    GameArena arena{};
    bool isloaded = arena.loadTilesInfo("data/tilesinfo.json");
    isloaded &= arena.loadMap("Tiled/demoMap.json", "data");
    if (!isloaded)
    {
        std::cerr << "Game arena loading failed !" << '\n';
        return;
    }

    // Get our hero
    auto hero = gameData.hero();
    arena.addHero(hero);

    int index = 0;
    for (auto&& breed : gameData.breeds())
    {
        sf::Vector2i newPos{rand() % 36 * 32, rand() % 28 * 32};
        entity::Monster monster{newPos, *breed};
        monster.onMove(newPos, newPos);
        arena.addMonster(std::make_shared<entity::Monster>(monster));

        index++;
    }

    // @TODO
    // @TODO Construire un monstre de chaque "Breed"
    // @TODO

    // Pattern - Game loop
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        // Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) { window.close(); }
        }

        // Update our mighty hero
        auto newPos = hero->isWaitingForInputs();
 
        // Draw
        window.clear();
        window.draw(arena);

        window.display();
    }

    ImGui::SFML::Shutdown();
}

} // namespace cppRogue
