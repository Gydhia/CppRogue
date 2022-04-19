#include "Action.hpp"

#include "engine/core/GameArena.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/entity/hero/Hero.hpp"
#include "engine/entity/monster/Monster.hpp"

#include "engine/core/utility/Debug.hpp"

namespace cppRogue::action {

void IAction::bindHero(entity::Hero& hero)
{
    m_entity = &hero;
    m_owner = Bound::Hero;
    m_arena = m_entity->gameArena();
}

void IAction::bindMonster(entity::Monster& monster)
{
    m_entity = &monster;
    m_owner = Bound::Monster;
    m_arena = m_entity->gameArena();
}

entity::Entity* IAction::entity() { return m_entity; };

entity::Hero* IAction::asHero()
{
    if (m_owner != Bound::Hero) return nullptr;

    return static_cast<entity::Hero*>(m_entity);
}

entity::Monster* IAction::asMonster()
{
    if (m_owner != Bound::Monster) return nullptr;

    return static_cast<entity::Monster*>(m_entity);
}

Result IAction::make() { return onMake(); }

Result IAction::success() { return Result(Result::Status::Success); }

Result IAction::failure() { return Result(Result::Status::Failure); }

Result IAction::inProgress() { return Result(Result::Status::InProgress); }

Result IAction::alternative(Action&& other)
{
    switch (m_owner)
    {
    case (Bound::Monster): other->bindMonster(*asMonster()); break;
    case (Bound::Hero): other->bindHero(*asHero()); break;
    default: ASSERT_DEBUG(false, "Action is not correctly bounded"); break;
    }

    return Result(std::move(other));
}

} // namespace cppRogue::action
