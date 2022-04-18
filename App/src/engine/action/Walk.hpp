#ifndef CPPROGUE_WALKACTION_HPP
#define CPPROGUE_WALKACTION_HPP

#include "Action.hpp"
#include "engine/core/Direction.hpp"

#include <SFML/System/Vector2.hpp>

// @TODO
// @TODO Implémenter l'action "Walk" pour les entités 
// @TODO Utiliser les fonctions de `core/Direction.hpp` et l'attribut "StepSize"
// @TODO

namespace cppRogue::action {

/**
 * @brief      This describe the walk action.
 */
class Walk : public IAction
{
  public:
    //
    // IACTION INTERFACE
    //
    Walk() = delete;
    explicit Walk(entity::Entity& owner, direction::Ordinal dir);
    ~Walk() override = default;
    Result onMake() override;

    static constexpr int StepSize{32};

  private:
    entity::Entity& m_owner;
    direction::Ordinal m_dir;
};

} // namespace cppRogue::action

#endif
