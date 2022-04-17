#include "ContentManager.hpp"

#include "engine/core/AttackInfo.hpp"
#include "engine/core/GraphicsInfo.hpp"
#include "engine/core/utility/Algorithm.hpp"
#include <memory>

namespace fs = std::filesystem;

namespace cppRogue {

bool loadItemData(collectable::ItemInfo& infos, const nlohmann::json& itemNode);

bool ContentManager::load(const std::string& path)
{
    auto rootNodeOp = json::load(path);
    if (!rootNodeOp.has_value()) { return false; }
    nlohmann::json rootNode = rootNodeOp.value();

    // All data loading will be relative to this path
    m_rootPath = fs::path{path}.parent_path();
    bool everythingOk = true;

    // Reserve enough space on each collection to avoid sparse memory
    m_textures.reserve(50);
    m_items.reserve(50);
    m_races.reserve(50);
    m_breeds.reserve(50);

    // Load items
    for (const auto& itemsNode : rootNode["items"])
    {
        everythingOk &= loadItems(itemsNode.get<std::string>());
    }

    // Load hero races
    for (const auto& racesNode : rootNode["races"])
    {
        everythingOk &= loadRaces(racesNode.get<std::string>());
    }

    // Load breeds
    for (const auto& breedsNode : rootNode["breeds"])
    {
        everythingOk &= loadBreeds(breedsNode.get<std::string>());
    }

    // Load hero
    everythingOk &= loadHero(rootNode["hero"].get<std::string>());

    return everythingOk;
}

bool ContentManager::loadHero(const std::string& relativePath)
{
    auto rootNodeOp = json::load((m_rootPath / relativePath).generic_string());
    if (!rootNodeOp.has_value()) { return false; }
    nlohmann::json rootNode = rootNodeOp.value();

    // Load race
    const auto raceSearchResult = find(m_races, rootNode["race"].get<std::string>());
    if (!raceSearchResult)
    {
        // An error occurs stop there
        return false;
    }

    // Load graphics data
    const auto graphicsNode = rootNode["graphics"];
    const auto fullTexturePath = m_rootPath / graphicsNode["texture"].get<std::string>();
    auto texture = texture::load(fullTexturePath.generic_string());
    if (!texture.has_value()) { return false; }
    m_textures.emplace_back(std::make_shared<sf::Texture>(std::move(texture.value())));

    const auto boundsNode = graphicsNode["bounds"];
    GraphicsInfo graphics{*m_textures.back()};
    graphics.setBounds(
        {boundsNode["x"], boundsNode["y"], boundsNode["width"], boundsNode["height"]});

    // Create Hero information
    auto name = rootNode["name"].get<std::string>();
    auto heroData = entity::HeroInfo{name, *raceSearchResult, std::move(graphics)};

    // Add gold
    int gold = rootNode["gold"].get<int>();
    heroData.gatherGold(gold);

    // Add equipment
    for (const auto& equipmentNode : rootNode["equipments"])
    {
        auto equipementSlot = equipmentNode["slot"].get<collectable::Slot>();
        auto equipmentId = equipmentNode["name"].get<std::string>();
        const auto equipmentSearchResult = find(m_items, equipmentId);
        if (!equipmentSearchResult)
        {
            // An error occurs stop there
            return false;
        }

        // Copy and equip it
        auto addResult = heroData.equipment().tryAdd(*equipmentSearchResult, equipementSlot);
        if (!addResult.isAdded())
        {
            // An error occurs stop there
            return false;
        }
    }

    // Create Hero
    sf::Vector2i initialPosition{};
    const auto positionNode = rootNode["position"];
    initialPosition.x = positionNode["x"].get<int>();
    initialPosition.y = positionNode["y"].get<int>();
    m_hero = std::make_shared<entity::Hero>(std::move(initialPosition), std::move(heroData));

    return m_hero != nullptr;
}

bool ContentManager::loadRaces(const std::string& relativePath)
{
    auto rootNodeOp = json::load((m_rootPath / relativePath).generic_string());
    if (!rootNodeOp.has_value()) { return false; }
    nlohmann::json rootNode = rootNodeOp.value();

    for (const auto& raceNode : rootNode["races"])
    {
        auto name = raceNode["name"].get<std::string>();
        auto description = raceNode["description"].get<std::string>();

        std::unordered_map<entity::Statistic, int> stats{};
        for (const auto& statNode : raceNode["statistics"])
        {
            auto statKind = statNode["name"].get<entity::Statistic>();
            auto statValue = statNode["value"].get<int>();
            stats[statKind] = statValue;
        }

        auto race = std::make_shared<entity::Race>(name, description, stats);
        m_races.emplace_back(std::move(race));
    }

    return true;
}

bool ContentManager::loadItems(const std::string& relativePath)
{
    auto rootNodeOp = json::load((m_rootPath / relativePath).generic_string());
    if (!rootNodeOp.has_value()) { return false; }
    nlohmann::json rootNode = rootNodeOp.value();

    for (const auto& itemNode : rootNode["items"])
    {
        // Parse item data
        collectable::ItemInfo infos{};
        if (loadItemData(infos, itemNode))
        {
            // Create item with infos
            m_items.emplace_back(std::make_shared<collectable::Item>(infos));
        }
    }

    return true;
}

bool ContentManager::loadBreeds(const std::string& relativePath)
{
    // @TODO
    // @TODO Remplir "m_breeds" à partir du JSON
    // @TODO

    return false;
}

bool loadItemData(collectable::ItemInfo& infos, const nlohmann::json& itemNode)
{
    infos.name = itemNode["name"].get<std::string>();
    infos.description = itemNode["description"].get<std::string>();

    // Statistics
    auto statisticsNode = itemNode["statistics"];
    infos.armor = statisticsNode["armor"].get<int>();
    infos.price = statisticsNode["price"].get<int>();
    infos.weight = statisticsNode["weight"].get<int>();
    infos.heft = statisticsNode["heft"].get<int>();
    infos.fuel = statisticsNode["fuel"].get<int>();

    // Slots (optionnal)
    if (itemNode.contains("slots"))
    {
        for (const auto& slotNode : itemNode["slots"])
        {
            // Case insensitive
            infos.slots += slotNode.get<collectable::Slot>();
        }
    }

    // Attack (optionnal)
    if (itemNode.contains("attack"))
    {
        auto attackNode = itemNode["attack"];

        int damage = attackNode["damage"].get<int>();
        // Attack name comes from the weapon name
        infos.attack = AttackInfo(infos.name, damage);
    }

    return true;
}

} // namespace cppRogue
