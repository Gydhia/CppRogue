#include "Walk.hpp"

#include "engine/action/Attack.hpp"
#include "engine/action/Rest.hpp"
#include "engine/core/GameArena.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/entity/hero/Hero.hpp"

#include <SFML/System/Vector2.hpp>

#include <memory>

namespace cppRogue::action {
action::Walk::Walk(entity::Entity& owner, direction::Ordinal dir) : m_owner(owner), m_dir(dir) 
{}
/* Walk */
Result Walk::onMake()
{
    // @TODO
    // @TODO Impl�menter la r�solution de l'action "Walk" 
    //   1) se reposer si la direction est "None" (voir "direction::Ordinal::None")
    //   2) attaquer si la nouvelle position contient une entit� 
    //   3) avancer si la tuile est accessible pour l'entit� (voir "canEnter" de "GameArena")
    // @TODO
    // @TODO
    auto ownerPos = m_owner.position();
    sf::Vector2i movement = sf::Vector2i{StepSize * direction::numeric(m_dir).x,
                                         StepSize * direction::numeric(m_dir).y};
    movement += ownerPos;

    Result result{};
    Motilities motilities = m_owner.motilities();
    if (m_dir != direction::Ordinal::None) {
        result.fallback = action::Action{action::Rest()};
    }
    entity::Entity* entity = m_owner.gameArena()->entityAt(movement);
    if (entity != nullptr /*Has to attack enemy*/)
    {
        action::Action attackAction = action::Action{action::Attack(m_owner)};
        attackAction->bindMonster(dynamic_cast<entity::Monster>(entity));
        
        result.fallback = attackAction;
        result.status = Result::Status::Failure;
        return result;
    }
    if (false && motilities[Motility::Swim] && motilities[Motility::Fly] /*is water tile*/)
    { 
        return failure();
    }
    if (false && motilities[Motility::Fly] /*is void tile*/)
    {
        return failure();
    }
    
    m_owner.move(movement);
    
    return success();
};

} // namespace cppRogue::action
