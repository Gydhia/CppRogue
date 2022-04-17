#ifndef CPPROGUE_ACTION_REST_HPP
#define CPPROGUE_ACTION_REST_HPP

#include "Action.hpp"

namespace cppRogue::action {

/**
 * @brief      This is the action of resting to regenerate some health.
 */
class Rest : public IAction
{
  public:
    Rest() = default;
    ~Rest() override = default;

    //
    // IACTION INTERFACE
    //
    Result onMake() override;
};

} // namespace cppRogue::action

#endif
