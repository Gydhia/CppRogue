#ifndef CPPROGUE_TILEINFO_HPP
#define CPPROGUE_TILEINFO_HPP

#include "engine/core/Motility.hpp"

#include <string>
#include <utility>

namespace cppRogue::environment {

/**
 * @brief      Interface that must be implemented by every tile type.
 */
struct TileInfo
{
  public:
    TileInfo() = default;
    TileInfo(int typeId, std::string typeName) : id(typeId), name(std::move(typeName)){};
    ~TileInfo() = default;

    friend bool operator==(const TileInfo& lhs, const TileInfo& rhs) { return lhs.id == rhs.id; }

    const int id{0};
    const std::string name{"none"};
    Motilities motilityFlags{};
    int cost{};
};

} // namespace cppRogue::environment

namespace std {

/**
 * @brief      Custom hash function injected into std namespace.
 * @note       See https://en.cppreference.com/w/cpp/utility/hash
 */
template <> struct hash<cppRogue::environment::TileInfo>
{
    size_t operator()(const cppRogue::environment::TileInfo& tileInfo) const { return tileInfo.id; }
};

} // namespace std

#endif
