#ifndef CPPROGUE_MONSTER_HPP
#define CPPROGUE_MONSTER_HPP

#include "engine/entity/Entity.hpp"

#include <SFML/System/Vector2.hpp>

#include <iostream>
#include <string>
#include <string_view>
#include <engine/entity/monster/Breed.hpp>
#include <engine/entity/monster/MonsterState.hpp>

// Forward declaration
namespace cppRogue::entity {
class MonsterState;
}

namespace cppRogue::entity {

/**
 * @brief      A monster is a specialized version of an entity that is controlled by IA.
 */
class Monster : public Entity
{
  public:
    Monster(sf::Vector2i initialPos, Breed breed);

    //
    // Rule of three
    //
    // We want deep copy not shallow copy of state pointee
    Monster(const Monster& other);
    Monster& operator=(Monster);
    ~Monster() override;

    friend void swap(Monster& first, Monster& second) // nothrow
    {
        using std::swap;

        // Swap them all !
        swap(first.m_breed, second.m_breed);
        swap(first.m_state, second.m_state);
    }

    //
    // Entity INTERFACE
    //
    [[nodiscard]] std::string_view name() const override { return m_breed.name(); }

    [[nodiscard]] int maxHealth() const override { return m_breed.m_infos.maxHealth; }

    [[nodiscard]] int speed() const override { return m_breed.m_infos.speed; }

    [[nodiscard]] const Motilities& motilities() const override { return m_breed.m_infos.motilities; }

    [[nodiscard]] std::vector<Hit> onGenerateMeleeHits(const Entity& opponent) override;

    void onGiveDamage(const action::Action& action, int damage, Entity* defender) override;

    void onReceiveDamage(const action::Action& action, int damage, Entity* opponent) override;

    void onKilled(Entity* opponent) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    //
    // Misc
    //
    [[nodiscard]] inline const MonsterState* activeState() const { return m_state.get(); }

    friend std::ostream& operator<<(std::ostream& os, const Monster& monster)
    {
        os << "{ Monster | " << monster.name() << ", ID=" << monster.id() << "}";
        return os;
    }

  private:
    std::unique_ptr<MonsterState> m_state;
    cppRogue::entity::Breed m_breed;
};
} // namespace cppRogue::entity
#endif
