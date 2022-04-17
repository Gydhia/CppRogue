#include "Map.hpp"

#include "engine/core/GraphicsInfo.hpp"
#include "engine/core/utility/Algorithm.hpp"
#include "engine/core/utility/Debug.hpp"
#include "engine/core/utility/IO.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

namespace cppRogue::environment {

namespace fs = std::filesystem;

/* Map */

std::optional<Tileset> loadTileset(nlohmann::json tilesetNode, const fs::path& parentPath)
{
    auto imagePath = tilesetNode["image"].get<std::string>();
    auto imageFullpath = fs::canonical(parentPath / imagePath);
    const auto optImg = texture::load(imageFullpath.generic_string());
    if (!optImg.has_value())
    {
        const auto message = "Can't open texture at " + imageFullpath.generic_string();
        std::cerr << message << '\n';
        return std::nullopt;
    }

    int tileCount = tilesetNode["tilecount"].get<int>();
    Tileset tileset{tileCount, optImg.value()};
    tileset.name = tilesetNode["name"].get<std::string>();
    tileset.imageHeight = tilesetNode["imageheight"].get<int>();
    tileset.imageWidth = tilesetNode["imagewidth"].get<int>();
    tileset.nbColumns = tilesetNode["columns"].get<int>();
    tileset.tileHeight = tilesetNode["tileheight"].get<int>();
    tileset.tileWidth = tilesetNode["tilewidth"].get<int>();
    tileset.margin = tilesetNode["margin"].get<int>();
    tileset.spacing = tilesetNode["spacing"].get<int>();

    // Update tile info ids if required
    for (auto&& tileNode : tilesetNode["tiles"])
    {
        const int localId = tileNode["id"].get<int>();

        for (auto&& propertyNode : tileNode["properties"])
        {
            auto name = algorithm::lower(propertyNode["name"].get<std::string>());
            if (name == "tileinfo")
            {
                // Only tileinfo property is currently used
                const int tileInfoId = propertyNode["value"].get<int>();
                tileset.setInfoId(localId, tileInfoId);
            }
        }
    }

    return tileset;
}

bool Map::load(const std::string& relativePath, const std::string& dataPath,
               const std::vector<TileInfo>& tilesInfo)
{
    const auto tiledMapPath = fs::canonical(fs::path(dataPath) / relativePath);
    auto rootNodeOp = json::load(tiledMapPath.generic_string());
    if (!rootNodeOp.has_value())
    {
        std::cerr << "Can't open map file: " << m_name << "\n";
        return false;
    }
    // Start parsing
    nlohmann::json rootNode = rootNodeOp.value();

    // Only support the tested version
    float version = std::stof(rootNode["version"].get<std::string>());
    if (version < 1.6F || version > 1.7F)
    {
        std::cerr << "Parsing not tested with version lower than 1.6 or larger than 1.7"
                  << "\n";
        return false;
    }

    m_dimension = {};
    m_dimension.tileHeight = rootNode["tileheight"].get<int>();
    m_dimension.tileWidth = rootNode["tilewidth"].get<int>();
    m_dimension.columCount = rootNode["width"].get<int>();
    m_dimension.rowCount = rootNode["height"].get<int>();

    // Load tilesets
    for (auto&& tilesetNode : rootNode["tilesets"])
    {
        const auto firstgid = tilesetNode["firstgid"].get<int>();

        if (tilesetNode.contains("source"))
        {
            // External tileset
            auto tilesetPath = tilesetNode["source"].get<std::string>();
            auto externalTilesetNodeOp =
                json::load((tiledMapPath.parent_path() / tilesetPath).generic_string());
            if (!externalTilesetNodeOp.has_value())
            {
                std::cerr << "Can't open tileset file: " << m_name << "\n";
                return false;
            }
            // Start parsing
            nlohmann::json externalTilesetNode = externalTilesetNodeOp.value();

            auto tileset = loadTileset(externalTilesetNode, tiledMapPath.parent_path());
            if (!tileset.has_value()) { return false; }
            tileset->firstGid = firstgid;
            m_tilesets.emplace_back(tileset.value());
        }
        else
        {
            // Embed tileset
            auto tileset = loadTileset(tilesetNode, tiledMapPath.parent_path());
            if (!tileset.has_value()) { return false; }
            tileset->firstGid = firstgid;
            m_tilesets.emplace_back(tileset.value());
        }
    }

    // Load layers
    for (auto&& layerNode : rootNode["layers"])
    {
        auto kind = layerNode["type"].get<std::string>();

        if (kind == "tilelayer")
        {
            int height = layerNode["height"].get<int>();
            int width = layerNode["width"].get<int>();
            loadTileLayer(height * width, layerNode["data"], tilesInfo);
        }
    }

    m_name = relativePath;
    return true;
}

bool Map::loadTileLayer(int tileCount, const nlohmann::json& dataNode,
                        const std::vector<TileInfo>& tilesInfo)
{
    m_tiles.reserve(tileCount);
    m_data.reserve(tileCount);

    int rowIndex = 0;
    int colIndex = 0;
    int i = 0;
    for (auto& [_, gid] : dataNode.items())
    {
        // Store graphic data inside the drawable map
        const auto& tileset = findTileset(gid);
        rowIndex = i / m_dimension.columCount;
        colIndex = i % m_dimension.columCount;
        const auto pos = sf::Vector2f(static_cast<float>(colIndex * m_dimension.tileWidth),
                                      static_cast<float>(rowIndex * m_dimension.tileHeight));
        
        auto data = GraphicsInfo(tileset.texture);
        data.setBounds(tileset.rect(gid));
        data.setPosition(pos);
        m_data.emplace_back(std::move(data));

        // Use tiled stored id to find correct tileinfo
        int tileInfoId = tileset.tileInfoId(gid);
        auto searchResult =
            std::find_if(tilesInfo.begin(), tilesInfo.end(),
                         [&tileInfoId](const TileInfo& info) { return info.id == tileInfoId; });

        if (searchResult == tilesInfo.end())
        {
            std::cerr << "Tile info (ID=" << tileInfoId
                      << ") does not exists. Falling back to default type (ID=" << 0 << ")."
                      << "\n";
            m_tiles.emplace_back(tilesInfo[0]);
        }
        else
        {
            m_tiles.emplace_back(*searchResult);
        }

        i++;
    }

    return true;
}

MapCoords Map::coordinates(const sf::Vector2i& position) const
{
    if (position.x < 0 || position.y < 0) { return {-1, -1}; }
    // Use center as reference to check entity location
    // Top left is used as origin for rendering
    const int delta = m_dimension.tileWidth / 2;

    // Map starts at 0
    const int xCoord = (position.x + delta) / m_dimension.tileWidth;
    const int yCoord = (position.y + delta) / m_dimension.tileHeight;

    return {xCoord, yCoord};
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    for (const auto& tileData : m_data) { target.draw(tileData.sprite); }
}

const Tileset& Map::findTileset(int globalId) const
{
    const auto result =
        std::find_if(m_tilesets.begin(), m_tilesets.end(), [globalId](const Tileset& tileset) {
            int upperBound = tileset.tileCount + tileset.firstGid;
            int lowerBound = tileset.firstGid;
            return globalId >= lowerBound && globalId < upperBound;
        });

    if (result == std::end(m_tilesets))
    {
        std::string msg("No tileset found for id = ");
        msg += std::to_string(globalId);
        ASSERT_DEBUG(false, msg.c_str()); // NOLINT debug mode only
        return m_tilesets[0];
    }

    return *result;
}

} // namespace cppRogue::environment
