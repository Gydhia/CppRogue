#include "Hero.hpp"

#include "engine/core/AttackInfo.hpp"
#include "engine/core/GameArena.hpp"
#include "engine/core/Hit.hpp"
#include "engine/core/utility/Debug.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iomanip>

namespace cppRogue::entity {

Hero::Hero(sf::Vector2i initialPos, HeroInfo data)
    : Entity{std::move(initialPos)}, m_data{std::move(data)}, m_behavior{new RestBehavior{}}
{
    const auto initialHealth = maxHealth();
    ASSERT_DEBUG(initialHealth > 0, "Entity maxHeal cannot be lower than 1");
    increaseHealth(initialHealth);

    // Force graphic update
    onMove({}, initialPos);
}

Hero::~Hero() = default;

Hero::Hero(const Hero& other)
    : Entity(other),
      m_data(other.m_data),
      m_behavior(other.m_behavior ? other.m_behavior->cloneBehavior() : nullptr)
{
}

Hero& Hero::operator=(Hero other)
{
    swap(*this, other);
    return *this;
}

const HeroBehavior* Hero::activeBehavior() const { return m_behavior ? m_behavior.get() : nullptr; }

void Hero::markAsWaitingForInputs() { m_behavior = nullptr; }

bool Hero::isWaitingForInputs()
{
    // If current behavior is not possible reset it
    if (m_behavior != nullptr && !m_behavior->isPossible(*this)) { markAsWaitingForInputs(); }

    // If currently no behavior then user should do something
    return m_behavior == nullptr;
}

void Hero::onMove(const sf::Vector2i& oldPos, const sf::Vector2i& newPos)
{
    // Update graphics
    m_data.m_graphicsData.setPosition({static_cast<float>(newPos.x), static_cast<float>(newPos.y)});
}

std::vector<Hit> Hero::onGenerateMeleeHits(const Entity& /*opponent*/)
{
    // Generate a hit for each weapon
    std::vector<Hit> hits = {};
    const auto weapons = m_data.m_equipment.weapons();
    for (const auto* weapon : weapons)
    {
        const auto& attackInfo = weapon->attackInfo();
        if (attackInfo.kind() == AttackInfo::Kind::Melee)
        {
            auto hit = Hit(attackInfo);

            // Scale up/down based on weapon required heft
            hit *= m_data.m_strength.damageScale(weapon->heft());

            hits.emplace_back(std::move(hit));
        }
    }

    if (hits.empty())
    {
        // At least try !
        hits.emplace_back(AttackInfo{"Punch", PunchPower});
    }

    return hits;
}

void Hero::onGiveDamage(const action::Action& /*action*/, int, Entity* defender)
{
    if (defender != nullptr)
    {
        std::cout << std::setw(18) << std::left << "onGiveDamage"
                  << " :: " << *this << " damaged " << *defender << "\n";
    }
}

void Hero::onReceiveDamage(const action::Action& /*action*/, int, Entity* opponent)
{
    if (opponent != nullptr)
    {

        std::cout << std::setw(18) << std::left << "onReceiveDamage"
                  << " :: " << *this << " was hit by " << *opponent << "\n";
    }
}

void Hero::onKilled(Entity* opponent)
{
    if (opponent != nullptr)
    {
        std::cout << std::setw(18) << std::left << "onKilled"
                  << " :: " << *this << " was killed by " << *opponent << "\n ";
    }
}

void Hero::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{
    target.draw(m_data.m_graphicsData.sprite);
}

} // namespace cppRogue::entity
