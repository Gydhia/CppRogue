#ifndef CPPROGUE_GAMEARENA_HPP
#define CPPROGUE_GAMEARENA_HPP

#include "engine/entity/Entity.hpp"
#include "engine/environment/Map.hpp"

#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <memory>
#include <vector>

// @TODO
// @TODO Implémenter l'inteface minimale de GameArena
// @TODO Attention aux différences de référentiel entre le monde (coordonnées) et la carte (indices des tuiles de "Map")
// @TODO


// @TODO
// @TODO Permettre de pouvoir récupérer et définir une référence / pointeur vers "GameArena" pour chaque "Entity"
// @TODO

namespace cppRogue {

// Forward declaration
namespace entity {
class Entity;
class Hero;
class Monster;
} // namespace entity

/**
 * @brief      Manager controlling entities and environment of the game.
 */
class GameArena : public sf::Drawable
{
  public:
    GameArena();
    ~GameArena() override;

    // Game arena is not copyable ...
    GameArena(const GameArena&) noexcept = delete;
    GameArena& operator=(const GameArena&) = delete;
    // ... but still movable
    GameArena(GameArena&&) noexcept = default;

    /* Load data */
    bool loadTilesInfo(const std::string& path);
    bool loadMap(const std::string& relativePath, const std::string& dataPath);

    // @TODO
    // @TODO Minimal interface required to be implemented by GameArena
    [[nodiscard]] bool addHero(std::shared_ptr<entity::Hero> hero);
    [[nodiscard]] bool addMonster(std::shared_ptr<entity::Monster> monster);
    [[nodiscard]] bool canEnter(const entity::Entity& entity, const sf::Vector2i& position) const;
    [[nodiscard]] entity::Entity* entityAt(const sf::Vector2i& position) const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    // @TODO Minimal interface required to be implemented by GameArena
    // @TODO

  private:
    // Map
    environment::Map m_map;
    sf::Vector2i m_mapSize;
    std::vector<environment::TileInfo> m_tilesInfo;

    // @TODO
    // @TODO Ajouter ici les membres privées (methodes et attributs)
    // @TODO
};

} // namespace cppRogue

#endif
