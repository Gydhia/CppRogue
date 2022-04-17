#ifndef CPPROGUE_MONSTERSTATE_HPP
#define CPPROGUE_MONSTERSTATE_HPP

// Forward declaration
namespace cppRogue::entity {
class Monster;
} // namespace cppRogue::entity

namespace cppRogue::entity {

/**
 * @brief      Define an abstract behavior for a monster. This is only the minimal
 *             required interface that must be specialized to create a concrete an
 *             usesable state.
 */
class MonsterState
{
  public:
    MonsterState() = delete;
    explicit MonsterState(Monster& monster);
    virtual ~MonsterState() = default;

    // Getters
    Monster& monster();

    //
    // VIRTUAL INTERFACE
    //
    [[nodiscard]] virtual MonsterState* cloneState() const = 0;

  private:
    Monster& m_monster;
};

/**
 * @brief      When out of sight or far enough from the Hero a monster just rest.
 */
class RestState : public MonsterState
{
  public:
    RestState() = delete;
    explicit RestState(Monster& monster);
    ~RestState() override = default;

    //
    // MonsterState INTERFACE
    //
    [[nodiscard]] RestState* cloneState() const override;
};

} // namespace cppRogue::entity

#endif
