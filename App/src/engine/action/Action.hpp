#ifndef CPPROGUE_ACTION_HPP
#define CPPROGUE_ACTION_HPP

#include "engine/core/utility/Archetype.hpp"

#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <memory>
#include <optional>

// Forward declaration
namespace cppRogue {
class GameArena;

namespace entity {
class Entity;
class Hero;
class Monster;
} // namespace entity

namespace action {
struct Result;
} // namespace action
} // namespace cppRogue

namespace cppRogue::action {
class IAction;

/// Action archetype allowing value semantic for IAction inheritance tree.
using Action = Archetype<IAction>;

/**
 * @brief      This is the required interface that each action must implement. An action
 *             is a container that allow to delayed its execution. This is an application of
 *             Command pattern.
 */
class IAction
{
  public:
    enum class Bound
    {
        None,
        Hero,
        Monster
    };
    IAction() = default;
    virtual ~IAction() = default;

    // Pointer data members are not owned [-Weffc++]
    IAction(const IAction&) noexcept = default;
    IAction(IAction&&) noexcept = default;
    IAction& operator=(const IAction&) = default;
    // Pointer data members are not owned [-Weffc++]

    entity::Entity* entity();
    entity::Hero* asHero();
    entity::Monster* asMonster();

    [[nodiscard]] bool isEntity() const { return m_entity != nullptr; }
    [[nodiscard]] bool isHero() const { return m_owner == Bound::Hero; }
    [[nodiscard]] bool isMonster() const { return m_owner == Bound::Monster; }

    void bindHero(entity::Hero& hero);
    void bindMonster(entity::Monster& monster);

    virtual Result make();

    // Factory
    static Result success();
    static Result failure();
    static Result inProgress();

    Result alternative(Action&& other);

    // Interface
    virtual Result onMake() = 0;

  private:
    Bound m_owner{Bound::None};
    entity::Entity* m_entity{};
    GameArena* m_arena{};
    sf::Vector2i m_position{};
};

/**
 * @brief      An result is created when an action is performed. It contains the action
 *             state and optionnaly an alternative action.
 */
struct Result
{
  public:
    enum class Status
    {
        InProgress,
        Success,
        Failure
    };

    Result() = default;
    explicit Result(Status state) : status(state){};
    explicit Result(Action other) : status(Status::InProgress), fallback(std::move(other)){};
    ~Result() = default;

    [[nodiscard]] bool isDone() const { return status != Status::InProgress; }
    [[nodiscard]] bool isSuccess() const { return status == Status::Success; }

    Status status{Status::Failure};
    std::optional<Action> fallback{std::nullopt};
};

} // namespace cppRogue::action

#endif
