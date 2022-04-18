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

RestBehavior* MoveBehavior::cloneBehavior() const { return nullptr; }

// Move behavior

bool MoveBehavior::isPossible(const entity::Hero& hero) {
    //bool possible = HeroBehavior::isPossible(hero);
    // can fly / swim ?
        
    return true; 
}

void MoveBehavior::interrupt(entity::Hero& hero) { 
    //HeroBehavior::interrupt(hero);
}

} // namespace cppRogue::entity
