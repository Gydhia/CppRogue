#ifndef CPPROGUE_HERO_HPP
#define CPPROGUE_HERO_HPP

#include "HeroBehavior.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/entity/hero/HeroInfo.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace cppRogue::entity {

/**
 * @brief      Concrete implementation of an entity controlled by the player.
 */
class Hero : public Entity
{
  public:
    Hero(sf::Vector2i initialPos, HeroInfo data);
    ~Hero() override;

    // Required because of unique_ptr polymorphic type
    Hero(const Hero& other);
    Hero& operator=(Hero);
    Hero(Hero&&) noexcept = default;

    /* GETTERS */
    [[nodiscard]] HeroInfo& info() { return m_data; };

    //
    // Entity INTERFACE
    //
    [[nodiscard]] std::string_view name() const override { return m_data.m_name; }
    [[nodiscard]] int armor() const override { return m_data.armor(); }
    [[nodiscard]] int maxHealth() const override { return m_data.m_endurance.maxHealth(); }
    [[nodiscard]] int speed() const override { return m_data.m_speed; }
    [[nodiscard]] const Motilities& motilities() const override { return m_data.m_motilities; }
    [[nodiscard]] Motilities& motilities() { return m_data.m_motilities; }

    [[nodiscard]] bool isWaitingForInputs() override;

    void onMove(const sf::Vector2i& oldPos, const sf::Vector2i& newPos) override;

    [[nodiscard]] std::vector<Hit> onGenerateMeleeHits(const Entity& opponent) override;

    void onGiveDamage(const action::Action& action, int damage, Entity* defender) override;

    void onReceiveDamage(const action::Action& action, int damage, Entity* opponent) override;

    void onKilled(Entity* opponent) override;

    //
    // Drawable INTERFACE
    //
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    //
    // Misc
    //
    [[nodiscard]] const HeroBehavior* activeBehavior() const;

    void markAsWaitingForInputs();

    friend std::ostream& operator<<(std::ostream& os, const Hero& hero)
    {
        os << hero.name() << " (Hero, ID=" << hero.id() << ") [" << hero.health() << '/'
           << hero.maxHealth() << "]";
        return os;
    }

    // Hardcoded punch power
    static constexpr int PunchPower{2};

  private:
    friend void swap(Hero& first, Hero& second) noexcept
    {
        // Utilisation par défaut du swap de la STL (utilisation de l'ADL)
        using std::swap;

        // Swap them all !
        swap(first.m_data, second.m_data);
        swap(first.m_behavior, second.m_behavior);
    }

    // Contains all hero associated data
    HeroInfo m_data;

    // Null when no behavior at all (wait for inputs)
    std::unique_ptr<HeroBehavior> m_behavior;
};

} // namespace cppRogue::entity

#endif
