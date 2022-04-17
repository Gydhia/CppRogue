#include "Walk.hpp"

#include "engine/action/Attack.hpp"
#include "engine/action/Rest.hpp"
#include "engine/core/GameArena.hpp"
#include "engine/entity/Entity.hpp"

#include <SFML/System/Vector2.hpp>

#include <memory>

namespace cppRogue::action {

/* Walk */
Result Walk::onMake()
{
    // @TODO
    // @TODO Implémenter la résolution de l'action "Walk" 
    //   1) se reposer si la direction est "None" (voir "direction::Ordinal::None")
    //   2) attaquer si la nouvelle position contient une entité 
    //   3) avancer si la tuile est accessible pour l'entité (voir "canEnter" de "GameArena")
    // @TODO
    // @TODO


    return failure();
};

} // namespace cppRogue::action
