#ifndef CPPROGUE_ACTION_ATTACK_HPP
#define CPPROGUE_ACTION_ATTACK_HPP

#include "Action.hpp"

namespace cppRogue::action {

class Attack : public IAction
{
  public:
    Attack() = delete;
    explicit Attack(entity::Entity& target);
    ~Attack() override = default;

    //
    // IACTION INTERFACE
    //
    Result onMake() override;

  private:
    entity::Entity& m_target;
};

} // namespace cppRogue::action

#endif
