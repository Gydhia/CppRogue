#include "Breed.hpp"

#include "engine/core/Motility.hpp"
#include "engine/core/utility/Algorithm.hpp"

#include <utility>

namespace cppRogue::entity {

    Breed::Breed(BreedInfo characteristics, GraphicsInfo graphics)
    : m_infos{characteristics}, m_graphics{graphics}
    { 
    } // namespace cppRogue::entity

    Breed& Breed::add(DefenseInfo defense) {
        m_defenses.emplace_back(defense);
        return *this;
    }

    Breed& Breed::add(AttackInfo attack) { 
        m_attacks.emplace_back(attack);
        return *this;
    }

} // namespace cppRogue::entity