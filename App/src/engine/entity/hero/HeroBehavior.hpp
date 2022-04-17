#ifndef CPPROGUE_HEROBEHAVIOR_HPP
#define CPPROGUE_HEROBEHAVIOR_HPP

// Forward declaration
namespace cppRogue::entity {
class Hero;
} // namespace cppRogue::entity

namespace cppRogue::entity {

/**
 * @brief      Common interface shared among all hero behavior. A behavior is an action
 *             generator allowing meta state for the hero. Generated action is
 *             implementation dependant and not restricted in any ways.
 */
class HeroBehavior
{
  public:
    HeroBehavior() = default;
    virtual ~HeroBehavior() = default;

    //
    // VIRTUAL INTERFACE
    //
    [[nodiscard]] virtual HeroBehavior* cloneBehavior() const = 0;

    virtual bool isPossible(const entity::Hero& hero) = 0;

    virtual void interrupt(entity::Hero& hero) = 0;
};

/**
 * @brief      This class describes a behavior where player is resting to restore its life
 *             force
 */
class RestBehavior : public HeroBehavior
{
  public:
    RestBehavior() = default;
    ~RestBehavior() override = default;

    //
    // HeroBehavior INTERFACE
    //
    [[nodiscard]] RestBehavior* cloneBehavior() const override;
    bool isPossible(const entity::Hero& hero) override;
    void interrupt(entity::Hero& hero) override;
};

} // namespace cppRogue::entity

#endif
