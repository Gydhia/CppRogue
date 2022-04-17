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
// @TODO Compléter les tests dans `BreedTests.cpp`
// @TODO

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
    explicit Breed(BreedInfo characteristics);
    ~Breed() = default;

    /* GETTERS */
    [[nodiscard]] inline std::string_view name() const;
    [[nodiscard]] inline const BreedInfo& data() const;

    [[nodiscard]] const std::vector<AttackInfo>& attacks() const;
    [[nodiscard]] const std::vector<DefenseInfo>& defenses() const;

    /* SETTERS */
    Breed& add(DefenseInfo defense);
    Breed& add(AttackInfo attack);

  private:
    // Special private access to Monster for convenience
    friend class Monster;
};

} // namespace cppRogue::entity

#endif
