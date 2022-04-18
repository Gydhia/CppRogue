#include "GameArena.hpp"

#include "engine/action/Action.hpp"
#include "engine/core/utility/Algorithm.hpp"
#include "engine/core/utility/IO.hpp"
#include "engine/entity/hero/Hero.hpp"
#include "engine/entity/monster/Monster.hpp"

#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <memory>

namespace cppRogue {

// Required for forward declaration to work with smart pointers
GameArena::GameArena() = default;
GameArena::~GameArena() = default;

bool GameArena::loadTilesInfo(const std::string& path)
{
    auto rootNodeOp = json::load(path);
    if (!rootNodeOp.has_value()) { return false; }
    nlohmann::json rootNode = rootNodeOp.value();

    auto typesNode = rootNode["types"];
    if (typesNode.empty())
    {
        std::cerr << "At least one tile type information must be defined in " << path << "\n";
        return false;
    }

    m_tilesInfo.reserve(typesNode.size());
    for (auto&& typeNode : typesNode)
    {
        const int id = typeNode["id"].get<int>();
        auto name = algorithm::lower(typeNode["name"].get<std::string>());

        auto tileType = environment::TileInfo{id, name};
        auto searchResult = std::find(m_tilesInfo.begin(), m_tilesInfo.end(), tileType);
        if (searchResult != m_tilesInfo.end())
        {
            std::cerr << "Tiles type cannot share a common ID. Duplicated ID is " << tileType.id
                      << "\n";
            return false;
        }

        tileType.cost = typeNode["cost"].get<int>();
        for (auto&& motilityNode : typeNode["motilities"])
        {
            tileType.motilityFlags += motilityNode.get<Motility>();
        }
        m_tilesInfo.emplace_back(tileType);
    }

    return true;
};

bool GameArena::loadMap(const std::string& relativePath, const std::string& dataPath)
{
    if (m_tilesInfo.empty())
    {
        std::cerr << "Tiles information (TileInfo) not found: " << '\n';
        return false;
    }

    bool isloaded = m_map.load(relativePath, dataPath, m_tilesInfo);

    if (isloaded)
    {
        // Keep local map size for fast lookup
        m_mapSize = sf::Vector2i(m_map.width(), m_map.height());
    }

    return isloaded;
};

bool GameArena::addHero(std::shared_ptr<entity::Hero> hero)
{
    if (!m_hero)
    {
        m_hero = hero;
        m_hero->setGameArena(this);
    }

    return false;
}

bool GameArena::addMonster(std::shared_ptr<entity::Monster> monster)
{
    m_monsters.emplace_back(monster);
    monster->setGameArena(this);

    return false;
}

bool GameArena::canEnter(const entity::Entity& entity, const sf::Vector2i& position) const
{
    environment::MapCoords mapCoord{position.x, position.y};

    if (this->m_map.inBounds(mapCoord))
    {
        if (this->m_map.at(mapCoord).canEnter(entity.motilities())) { return true; }
    }

    return false;
}

entity::Entity* GameArena::entityAt(const sf::Vector2i& position) const
{
    for (auto& monster : m_monsters) { 
        if (monster->position() == position) 
            return monster.get();
    }
    if (m_hero->position() == position) 
        return m_hero.get();

    return nullptr;
}

//
// Drawable INTERFACE
//
void GameArena::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Draw the whole map
    m_map.draw(target, states);

    // @TODO
    // @TODO Afficher les différentes entités (Héro et Monstres)
    // @TODO
}

} // namespace cppRogue
