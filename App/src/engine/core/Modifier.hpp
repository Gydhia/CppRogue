#ifndef CPPROGUE_MODIFIER_HPP
#define CPPROGUE_MODIFIER_HPP

#include "nlohmann/json.hpp"

namespace cppRogue {

/**
 * @brief      An helper container avoiding cumbersome repetitive declaration.
 */
struct Modifier
{
  public:
    Modifier() = default;
    explicit Modifier(int modBonus, double modScale) : bonus(modBonus), scale(modScale) {}

    friend bool operator==(const Modifier& lhs, const Modifier& rhs)
    {
        return (lhs.bonus == rhs.bonus) && (lhs.scale == rhs.scale);
    }
    friend bool operator!=(const Modifier& lhs, const Modifier& rhs) { return !(lhs == rhs); }

    int bonus{0};
    double scale{1.0};
};

inline void to_json(nlohmann::json& j, const Modifier& mod)
{
    j = nlohmann::json{{"bonus", mod.bonus}, {"scale", mod.scale}};
}

inline void from_json(const nlohmann::json& j, Modifier& mod)
{
    j.at("bonus").get_to(mod.bonus);
    j.at("scale").get_to(mod.scale);
}

} // namespace cppRogue

#endif
