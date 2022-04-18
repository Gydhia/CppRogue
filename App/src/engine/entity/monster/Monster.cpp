#include "Monster.hpp"

#include "MonsterState.hpp"
#include "engine/core/utility/Debug.hpp"
#include <random>

#include <SFML/Graphics/RenderTarget.hpp>

#include <iomanip>

namespace cppRogue::entity {

Monster::Monster(sf::Vector2i initialPos, Breed breed)
    : Entity{initialPos}, m_breed{ std::move(breed) }
{
    m_state = std::make_unique<RestState>(RestState{*this});
    increaseHealth(m_breed.m_infos.maxHealth);
}

Monster::Monster(const Monster& other) : Entity{other}, m_breed {other.m_breed}
{
    m_state = std::unique_ptr<MonsterState>(other.m_state->cloneState());
}

Monster::~Monster() { m_state.release(); }

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

void Monster::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{
    target.draw(m_breed.m_graphics.sprite);
}

} // namespace cppRogue::entity
