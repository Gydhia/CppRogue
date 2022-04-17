#include "Entity.hpp"

namespace cppRogue::entity {

unsigned int Entity::m_sNextId = 0;

Entity::Entity(sf::Vector2i position) : m_position(position), m_id{m_sNextId++} {}

Entity::Entity(const Entity& other)
    : m_position{other.m_position},
      m_health{other.m_health},
      m_energy{other.m_energy},
      m_id{m_sNextId++}
{
}

void Entity::move(const sf::Vector2i& newPosition)
{
    if (m_position != newPosition)
    {
        onMove(m_position, newPosition);
        m_position.x = newPosition.x;
        m_position.y = newPosition.y;
    }
}

void Entity::takeDamage(const action::Action& attackAction, int damage, Entity* striker)
{
    // Only account for damage is the entity is still alive
    if (!isAlive()) { return; }

    decreaseHealth(damage);

    // Let concrete entity react to damage
    onReceiveDamage(attackAction, damage, striker);

    if (!isAlive())
    {
        // Let concrete entity react to death
        onKilled(striker);
    }
}

std::vector<Hit> Entity::generateHits(const Entity& opponent, AttackInfo::Kind hitType)
{
    std::vector<Hit> hits{};

    // Only melee hits are supported for now
    if (hitType == AttackInfo::Kind::Melee)
    {
        // Delegate generation to concrete entity
        hits = onGenerateMeleeHits(opponent);
    }

    return hits;
}

bool operator==(const Entity& lhs, const Entity& rhs) { return lhs.m_id == rhs.m_id; }

bool operator!=(const Entity& lhs, const Entity& rhs) { return !operator==(lhs, rhs); }

} // namespace cppRogue::entity
