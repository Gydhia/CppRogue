#include "Attack.hpp"

#include "engine/core/AttackInfo.hpp"
#include "engine/core/Hit.hpp"
#include "engine/entity/Entity.hpp"

namespace cppRogue {

action::Attack::Attack(entity::Entity& target) : m_target(target) {}

action::Result action::Attack::onMake()
{
    entity::Entity* actor = entity();
    if (actor)
    {
        // Resolve hits
        auto hits = actor->generateHits(m_target, AttackInfo::Kind::Melee);
        for (auto&& hit : hits)
        {
            hit.make(action::Action(*this), actor, &m_target);

            // RIP ...
            if (!m_target.isAlive()) break;
        }
    }

    // This action cannot be a failure
    return success();
}

} // namespace cppRogue
