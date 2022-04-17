#ifndef CPPROGUE_IO_HPP
#define CPPROGUE_IO_HPP

#include "SFML/Graphics/Texture.hpp"

#include "nlohmann/json.hpp"

#include <optional>

namespace cppRogue::json {

std::optional<nlohmann::json> load(const std::string& path);

} // namespace cppRogue::json

namespace cppRogue::texture {

std::optional<sf::Texture> load(const std::string& path);

} // namespace cppRogue::texture

#endif
