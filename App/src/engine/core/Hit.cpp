#include "Hit.hpp"

#include "engine/core/GameArena.hpp"
#include "engine/core/utility/Random.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/entity/hero/Hero.hpp"

#include <algorithm>
#include <cmath>
#include <optional>
#include <random>

namespace cppRogue {

Hit::Hit(AttackInfo baseAttack) : m_attack{std::move(baseAttack)} {}

int Hit::make(const action::Action& action, entity::Entity* attacker, entity::Entity* defender)
{
    // Damage calculation
    const int damage = computeDamage(defender->armor());

    // Notify striker about damage done to the defender
    if (nullptr != attacker) { attacker->onGiveDamage(action, damage, defender); }

    // Notify damage taken by defender
    defender->takeDamage(action, damage, attacker);

    return damage;
}

int Hit::computeDamage(int opponentArmor) const
{
    // Account for chance
    double damage = rolledDamage();

    // Account for armor
    double finalDamage = (damage * damage) / (damage + opponentArmor);

    return static_cast<int>(std::round(finalDamage));
}

int Hit::averageDamage() const
{
    const double damage = m_attack.damage() * m_modifier.scale + m_modifier.bonus;
    return static_cast<int>(std::round(damage));
}

double Hit::rolledDamage() const
{
    int damage = averageDamage();

    auto gen = Random::generator();
    std::normal_distribution<> normalRng(damage, 2);
    double rolledDamage = normalRng(gen) + 0.2 * m_attack.damage(); // Right shifted distribution

    return rolledDamage;
}

Hit& Hit::operator+=(int bonus)
{
    m_modifier.bonus += bonus;
    return *this;
}

Hit& Hit::operator*=(double scale)
{
    m_modifier.scale *= scale;
    return *this;
}

} // namespace cppRogue
