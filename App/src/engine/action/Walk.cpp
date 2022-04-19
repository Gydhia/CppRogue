#include "Walk.hpp"

#include "engine/action/Attack.hpp"
#include "engine/action/Rest.hpp"
#include "engine/core/GameArena.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/entity/hero/Hero.hpp"
#include "engine/entity/monster/Monster.hpp"

#include <SFML/System/Vector2.hpp>

#include <memory>


namespace cppRogue::action {

Walk::Walk(entity::Entity& owner, direction::Ordinal dir) : m_owner(owner), m_ordinalDir(dir) {}
/* Walk */
Result Walk::onMake()
{
    auto ownerPos = m_owner.position();
    sf::Vector2i movement = sf::Vector2i{StepSize * direction::numeric(m_ordinalDir).x,
                                         StepSize * direction::numeric(m_ordinalDir).y};
    movement += ownerPos;

    Result result{};
    Motilities motilities = m_owner.motilities();

    if (m_ordinalDir != direction::Ordinal::None)
    {
        result.fallback = action::Action{action::Rest()};
    }
    entity::Entity* entity = m_owner.gameArena()->entityAt(movement);
    if (entity != nullptr /*Has to attack enemy*/)
    {
        action::Action attackAction = action::Action{action::Attack(*entity)};
        attackAction->bindMonster(*static_cast<entity::Monster*>(entity));

        result.fallback = attackAction;
        result.status = Result::Status::Failure;
        return result;
    }

    if (!m_owner.gameArena()->canEnter(m_owner, movement)) { return failure(); }
    m_owner.move(movement);

    return success();
};
} // namespace cppRogue::action
