#include "Monster.hpp"

#include "MonsterState.hpp"
#include "engine/core/utility/Debug.hpp"

#include <iomanip>

namespace cppRogue::entity {

Monster::Monster(sf::Vector2i initialPos, const std::string& name, int maxHeal, int speed)
    : Entity{initialPos},
      m_name{name},
      m_maxHeal{maxHeal},
      m_speed{speed},
      m_state{new RestState{*this}}
{
    ASSERT_DEBUG(m_maxHeal > 0, "maxHeal cannot be lower than 1");
    increaseHealth(m_maxHeal);
}

Monster::Monster(const Monster& other)
    : Entity{other}, m_name{other.m_name}, m_maxHeal{other.m_maxHeal}, m_speed{other.m_speed}
{
    m_state = other.m_state ? other.m_state->cloneState() : nullptr;
}

Monster::~Monster() { delete m_state; }

Monster& Monster::operator=(Monster other)
{
    swap(*this, other);
    return *this;
}

std::vector<Hit> Monster::onGenerateMeleeHits(const Entity& /*opponent*/)
{
    auto meleeAttacks = std::vector<AttackInfo>{};

    // @TODO
    // @TODO Remplir "meleeAttacks" avec les attaques au contact
    // @TODO 
   
    // @TODO
    // @TODO Sélectionner une unique attaque parmis les choix possibles de manière aléatoire
    // @TODO Possible d'utiliser "Random::generator" (`utility/Random.hpp`)
    // @TODO

    return {};
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
