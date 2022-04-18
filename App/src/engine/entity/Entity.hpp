#ifndef CPPROGUE_ENTITY_HPP
#define CPPROGUE_ENTITY_HPP

#include "engine/action/Action.hpp"
#include "engine/core/AttackInfo.hpp"
#include "engine/core/Energy.hpp"
#include "engine/core/Hit.hpp"
#include "engine/core/Motility.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm> // std::clamp
#include <iostream>  // std::ostream
#include <string_view>


// @TODO
// @TODO Adapter "Entity" afin de pouvoir conserver un pointeur vers un "GameArena"
// @TODO

// @INFO 
// @INFO Pensez à utiliser "onMove" dans les classes enfants
// @INFO 


namespace cppRogue::entity {

class Entity : public sf::Drawable
{
  public:
    explicit Entity(sf::Vector2i position);
    ~Entity() override = default;

    // We want an unique ID for each copy
    Entity(const Entity& other);

    // Getters
    [[nodiscard]] inline sf::Vector2i position() const { return m_position; }
    [[nodiscard]] inline int health() const { return m_health; }
    [[nodiscard]] inline unsigned int id() const { return m_id; }
    [[nodiscard]] bool isAlive() const { return m_health > 0; }

    // Energy access
    [[nodiscard]] inline bool canTakeTurn() const { return m_energy.canTakeTurn(); }
    inline void accumulate() { m_energy.accumulate(speed()); }

    // Setters
    inline void increaseHealth(int amount)
    {
        m_health += amount;
        m_health = std::clamp(m_health, 0, maxHealth());
    }
    inline void decreaseHealth(int amount)
    {
        m_health -= amount;
        m_health = std::clamp(m_health, 0, maxHealth());
    }

    /**
     * @brief      Can be called to generate a collection of hits this entity will
     *             performed on @opponent.
     *
     * @param[in]  opponent  The opponent to hit
     * @param[in]  hitType   The hit type
     *
     * @return     Collection of hits.
     */
    std::vector<Hit> generateHits(const Entity& opponent, AttackInfo::Kind hitType);

    /**
     * @brief      Move the entity to a new location.
     *
     * @param[in]  newPosition  The new position
     */
    void move(const sf::Vector2i& newPosition);

    /**
     * @brief      Can be called to apply some damage to this entity.
     *
     * @param[in]  action   The action currently performed
     * @param[in]  damage   The damage to apply
     * @param      striker  The striker entity
     */
    void takeDamage(const action::Action& action, int damage, Entity* striker);

    //
    // ENTITY INTERFACE
    //
    /**
     * @brief      String representation mainly for debug purpose.
     *
     * @return     Name of this entity.
     */
    [[nodiscard]] virtual std::string_view name() const = 0;

    /**
     * @brief      Get the entity maximum health level.
     *
     * @return     Maximum health level allowed for this entity.
     */
    [[nodiscard]] virtual int maxHealth() const = 0;

    /**
     * @brief      Get the entity current speed level.
     *
     * @return     Current entity speed.
     */
    [[nodiscard]] virtual int speed() const = 0;

    /**
     * @brief      Get the entity current armor level.
     *
     * @return     Current entity armor.
     */
    [[nodiscard]] virtual int armor() const { return 0; };

    /**
     * @brief      Get the entity current motilities. Motilities defined how the entity
     *             can move
     *
     * @return     Current entity motilities
     */
    [[nodiscard]] virtual const Motilities& motilities() const = 0;

    /**
     * @brief      Only hero is waiting for user inputs.
     *
     * @return     True if waiting for inputs, False otherwise.
     */
    virtual bool isWaitingForInputs() { return false; }

    /**
     * @brief      Called when a move occurs
     *
     * @param[in]  oldPos  The old position
     * @param[in]  newPos  The new position
     */
    virtual void onMove(const sf::Vector2i& oldPos, const sf::Vector2i& newPos) {};

    /**
     * @brief      Called when an entity should compute a list of melee hits.
     *
     * @param[in]  opponent  The opponent / target of hits
     *
     * @return     Collection of melee hits.
     */
    virtual std::vector<Hit> onGenerateMeleeHits(const Entity& opponent) = 0;

    /**
     * @brief      Called when this entity will wounded @p defender.
     *
     * @param[in]  action    The action
     * @param[in]  damage    The damage done to @p defender by this entity
     * @param      defender  The defender
     */
    virtual void onGiveDamage(const action::Action& /*action*/, int /*damage*/,
                              Entity* /*defender*/){};
    /**
     * @brief      Called when this entity has been hit by @p opponent.
     *
     * @param[in]  action    The action
     * @param[in]  damage    The damage done to this entity by @p opponent
     * @param      opponent  The opponent/striker
     */
    virtual void onReceiveDamage(const action::Action& /*action*/, int /*damage*/,
                                 Entity* /*opponent*/){};
    //
    // @brief      Called when this entity has been killed (by @p opponent if not null).
    //
    // @param      opponent  The opponent/striker that kill this entity
    //
    virtual void onKilled(Entity* /*opponent*/){};

    [[nodiscard]] inline GameArena* gameArena() const { return m_gameArena; }

    inline void setGameArena(GameArena* gameArena) { m_gameArena = gameArena; }

    //
    // Drawable INTERFACE
    //
    void draw(sf::RenderTarget& /*target*/, sf::RenderStates /*states*/) const override{};

    //
    // OPERATORS
    //
    friend bool operator==(const Entity& lhs, const Entity& rhs);
    friend bool operator!=(const Entity& lhs, const Entity& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Entity& entity)
    {
        os << entity.name() << " (ID=" << entity.id() << ") [" << entity.health() << '/'
           << entity.maxHealth() << "]";
        return os;
    }

  private:
    sf::Vector2i m_position;
    int m_health{};
    Energy m_energy{};

    // Unique instance ID used to quickly test if two entities are the same
    unsigned int m_id;
    static unsigned int m_sNextId;
    GameArena* m_gameArena;
};

} // namespace cppRogue::entity

#endif
