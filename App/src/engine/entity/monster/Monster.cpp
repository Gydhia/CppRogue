#include "Monster.hpp"

#include "MonsterState.hpp"
#include "engine/core/utility/Debug.hpp"
#include <random>

#include <iomanip>

namespace cppRogue::entity {

Monster::Monster(sf::Vector2i initialPos, Breed breed)
    : Entity{initialPos}, m_breed{breed}, m_state{this->m_state}
{
    ASSERT_DEBUG(m_maxHeal > 0, "maxHeal cannot be lower than 1");
    increaseHealth(m_maxHeal);
}

Monster::Monster(const Monster& other) : Entity{other}, m_breed {other.m_breed}
{
    m_state = other.m_state ? other.m_state->cloneState() : nullptr;
}

Monster::~Monster() { delete m_state; }

Monster& Monster::operator=(Monster other)
{
    swap(*this, other);
    return *this;
}

std::vector<Hit> Monster::onGenerateMeleeHits(const Entity& opponent)
{
    auto meleeAttacks = std::vector<AttackInfo>{};

    for (auto attack : m_breed.attacks()) { 
        if (attack.kind() == AttackInfo::Kind::Melee)
            meleeAttacks.emplace_back(attack);
    }

    int index = abs(rand()) % meleeAttacks.size();

    std::vector<Hit> hits;
    hits.emplace_back( Hit(meleeAttacks[index]));
    
    return hits;
}

void Monster::onGiveDamage(const action::Action& /*action*/, int, Entity* defender)
{
    if (defender != nullptr)
    {
        std::cout << std::setw(18) << std::left << "onGiveDamage"
                  << " :: " << *this << " damaged " << *defender << "\n";
    }
}

void Monster::onReceiveDamage(const action::Action& /*action*/, int, Entity* opponent)
{
    if (opponent != nullptr)
    {

        std::cout << std::setw(18) << std::left << "onReceiveDamage"
                  << " :: " << *this << " was hit by " << *opponent << "\n";
    }
}

void Monster::onKilled(Entity* opponent)
{
    if (opponent != nullptr)
    {
        std::cout << std::setw(18) << std::left << "onKilled"
                  << " :: " << *this << " was killed by " << *opponent << "\n ";
    }
}

} // namespace cppRogue::entity
