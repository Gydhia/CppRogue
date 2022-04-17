#include "HeroBehavior.hpp"

#include "Hero.hpp"

namespace cppRogue::entity {

// RestBehavior

entity::RestBehavior* entity::RestBehavior::cloneBehavior() const
{
    return new RestBehavior(*this);
}

void RestBehavior::interrupt(entity::Hero& hero) { hero.markAsWaitingForInputs(); }

bool RestBehavior::isPossible(const Hero& hero)
{
    return hero.health() < hero.maxHealth();
}

} // namespace cppRogue::entity
