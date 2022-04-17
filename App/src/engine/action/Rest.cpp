#include "Rest.hpp"

#include "engine/entity/Entity.hpp"

namespace cppRogue::action {

Result action::Rest::onMake()
{
    // Only an entity can rest
    if (isEntity()) { entity()->increaseHealth(1); }

    // This action cannot be a failure
    return success();
}

} // namespace cppRogue::action
