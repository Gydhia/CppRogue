#ifndef CPPROGUE_BREED_HPP
#define CPPROGUE_BREED_HPP

#include "engine/core/AttackInfo.hpp"
#include "engine/core/Energy.hpp"
#include "engine/core/GraphicsInfo.hpp"
#include "engine/core/Motility.hpp"

#include "nlohmann/json.hpp"

#include <string>
#include <string_view>
#include <vector>

// @TODO
// @TODO Compléter l'implémentation de "Breed"
// @TODO L'API publique ne doit pas être modifiée
// @TODO
// 
// @TODO
// @TODO Ajouter le/les attributs manquants pour pouvoir charger toutes les données du JSON
// @TODO Voir `data/Monster/breeds.json`
// @TODO

namespace cppRogue::entity {

/**
 * @brief      Data container for a Breed.
 */
struct BreedInfo
{
  public:
    std::string name{};
    std::string description{};
    /// Experience rewarded when a monster of this breed is killed
    int experience{0};

    // Statistics
    int maxHealth{0};
    /// Affect the rate of action
    int speed = {Energy::NormalSpeed};
    int dodge{0};
    int armor{0};
    Motilities motilities{};
    int trackingDistance{};
};

/**
 * @brief      Additional protection that can be associated to a breed.
 */
struct DefenseInfo
{
  public:
    std::string name{};
    int melee{};
    int ranged{};
    int magic{};
};

/**
 * @brief      A breed is a kind of monster (Human, Spider, ...) that describe the generic
 *             data / interface shared among different monsters.
 *
 * @note       Breed and Monster classes if an application of the TypeObject pattern.
 */
class Breed
{
  public:
    explicit Breed(BreedInfo characteristics, GraphicsInfo graphics);
    ~Breed() = default;

    /* GETTERS */
    [[nodiscard]] inline std::string_view name() const { return m_infos.name; };
    [[nodiscard]] inline const BreedInfo& data() const { return m_infos; };

    [[nodiscard]] inline const std::vector<AttackInfo>& attacks() const { return this->m_attacks; };
    [[nodiscard]] const std::vector<DefenseInfo>& defenses() const { return this->m_defenses; };

    /* SETTERS */
    Breed& add(DefenseInfo defense);
    Breed& add(AttackInfo attack);

  private:
    std::vector<AttackInfo> m_attacks;
    std::vector<DefenseInfo> m_defenses;

    BreedInfo m_infos;
    GraphicsInfo m_graphics;
    // Special private access to Monster for convenience
    friend class Monster;
};

} // namespace cppRogue::entity

#endif
