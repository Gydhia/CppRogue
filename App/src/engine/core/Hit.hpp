#ifndef CPPROGUE_HIT_HPP
#define CPPROGUE_HIT_HPP

#include "engine/action/Action.hpp"
#include "engine/core/AttackInfo.hpp"
#include "engine/core/Modifier.hpp"

#include <ostream>

// Forward declaration
namespace cppRogue::entity {
class Entity;
}

namespace cppRogue {

class Hit
{
  public:
    Hit() = delete;
    /**
     * @brief      A hit is generated only using basic attack information. Additional data
     *             like item damage, element, side effect, ... can be added afterward only
     *             if required. Using this approach allows us to not tightly coupled Item
     *             and Hit and build a Hit out of something that is not an Item.
     *
     * @param[in]  baseAttack  The base attack
     */
    explicit Hit(AttackInfo baseAttack);
    ~Hit() = default;

    // Pointer data members are not owned [-Weffc++]
    Hit(const Hit&) noexcept = default;
    Hit(Hit&&) noexcept = default;
    Hit& operator=(const Hit&) = default;
    // Pointer data members are not owned [-Weffc++]

    // Getters
    [[nodiscard]] Modifier modifier() const { return m_modifier; }
    [[nodiscard]] AttackInfo::Kind kind() const { return m_attack.kind(); }

    /**
     * @brief      Average damage done by this Hit. Final damage could be a little more or
     *             less depending on rng.
     *
     * @return     Average damage done by this Hit
     */
    [[nodiscard]] int averageDamage() const;

    // Setters
    Hit& operator+=(int bonus);
    Hit& operator*=(double scale);

    /**
     * @brief      Compute the damage performs from @p attacker to @p defender in the
     *             course of @p action.
     *
     * @param[in]  action    The action leading to this hit
     * @param      attacker  The entity performing the action. Can be null if action is
     *                       from the environment.
     * @param[in]  defender  The entity receiving the attack.
     *
     * @return     Amount of damage actually done after taking account @defender defenses.
     */
    int make(const action::Action& action, entity::Entity* attacker, entity::Entity* defender);

    friend std::ostream& operator<<(std::ostream& os, const Hit& hit)
    {
        os << "{Hit | " << hit.m_attack << ", "
           << "(" << hit.m_modifier.bonus << ", " << hit.m_modifier.scale << ")}";
        return os;
    }

  private:
    [[nodiscard]] int computeDamage(int opponentArmor) const;
    [[nodiscard]] double rolledDamage() const;

    AttackInfo m_attack;
    Modifier m_modifier{};
};

} // namespace cppRogue

#endif
