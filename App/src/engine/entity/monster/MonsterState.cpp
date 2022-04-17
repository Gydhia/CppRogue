#include "MonsterState.hpp"

#include "Monster.hpp"

namespace cppRogue::entity {

MonsterState::MonsterState(Monster& monster) : m_monster{monster} {}

Monster& entity::MonsterState::monster() { return m_monster; }

// RestState

entity::RestState::RestState(Monster& monster) : MonsterState(monster) {}

entity::RestState* entity::RestState::cloneState() const { return new RestState(*this); }

} // namespace cppRogue::entity
