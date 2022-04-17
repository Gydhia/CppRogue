#include "IO.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>

namespace cppRogue::json {

std::optional<nlohmann::json> load(const std::string& path)
{
    std::ifstream mapStream(path, std::ifstream::binary);
    if (mapStream.fail())
    {
        std::cerr << "Can't open json file: " << path << "\n";
        return std::nullopt;
    }

    // Start parsing
    nlohmann::json rootNode = nlohmann::json::parse(mapStream);

    return rootNode;
}

} // namespace cppRogue::json

namespace cppRogue::texture {

std::optional<sf::Texture> load(const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cerr << "Can't open texture file: " << path << "\n";
        return std::nullopt;
    }

    return texture;
}

} // namespace cppRogue::texture
