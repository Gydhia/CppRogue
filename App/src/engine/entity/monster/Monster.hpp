#ifndef CPPROGUE_MONSTER_HPP
#define CPPROGUE_MONSTER_HPP

#include "engine/entity/Entity.hpp"

#include <SFML/System/Vector2.hpp>

#include <iostream>
#include <string>
#include <string_view>

// @TODO
// @TODO Refactoring de la classe pour maintenant accepter une "Breed"
// @TODO
// @TODO Supprimer l'ancien constructeur et conserver uniquement deux attributs
//   - "m_state"
//   - "m_breed"
// @TODO
// @TODO Mettre � jour les diff�rents tests unitaires pour correspondre � la nouvelle interface
//   - `ActionTests.cpp`
//   - `EntityTests.cpp`
// @TODO

// @TODO
// @TODO Compl�ter "onGenerateMeleeHits" en utilisant le nouvel attribut "m_breed"
// @TODO

// @TODO
// @TODO Utiliser un pointeur intelligent (std::unique_ptr) pour l'attribut "m_state"
// @TODO Adapter le reste du code
// @TODO

// @TODO
// @TODO Permettre d'afficher un monstre � l'�cran � utilisant les informations de la "Breed"
// @TODO

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
    // @TODO
    // @TODO � impl�menter
    // Monster(sf::Vector2i initialPos, Breed breed);
    // @TODO � impl�menter
    // @TODO
    // @TODO � supprimer
    Monster(sf::Vector2i initialPos, const std::string& name, int maxHeal, int speed);
    // @TODO � supprimer
    // @TODO

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
        swap(first.m_name, second.m_name);
        swap(first.m_maxHeal, second.m_maxHeal);
        swap(first.m_speed, second.m_speed);
        swap(first.m_state, second.m_state);
        swap(first.m_motilities, second.m_motilities);
    }

    //
    // Entity INTERFACE
    //
    [[nodiscard]] std::string_view name() const override { return m_name; }

    [[nodiscard]] int maxHealth() const override { return m_maxHeal; }

    [[nodiscard]] int speed() const override { return m_speed; }

    [[nodiscard]] const Motilities& motilities() const override { return m_motilities; }

    [[nodiscard]] std::vector<Hit> onGenerateMeleeHits(const Entity& opponent) override;

    void onGiveDamage(const action::Action& action, int damage, Entity* defender) override;

    void onReceiveDamage(const action::Action& action, int damage, Entity* opponent) override;

    void onKilled(Entity* opponent) override;

    //
    // Misc
    //
    [[nodiscard]] inline const MonsterState* activeState() const { return m_state; }

    friend std::ostream& operator<<(std::ostream& os, const Monster& monster)
    {
        os << "{ Monster | " << monster.name() << ", ID=" << monster.id() << "}";
        return os;
    }

  private:
    std::string m_name{};
    int m_maxHeal{};
    int m_speed{};
    Motilities m_motilities{Motility::Walk};

    MonsterState* m_state{};
};

} // namespace cppRogue::entity

#endif
