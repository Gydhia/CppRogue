#ifndef CPPROGUE_ENERGY_HPP
#define CPPROGUE_ENERGY_HPP

namespace cppRogue {

/**
 * @brief      Energy is used to control the rate at which an entity can take an action.
 *             At each turn, energy is accumulated and when a specific threshold is
 *             reached an action can be performed.
 */
class Energy
{
  public:
    Energy() = default;
    explicit Energy(int startAmount);
    ~Energy() = default;

    [[nodiscard]] inline int amount() const { return m_amount; }

    /**
     * @brief      Determines the ability to take a turn.
     *
     * @return     True is energy level is higher than required level to take a turn, else
     *             false.
     */
    [[nodiscard]] bool canTakeTurn() const;

    /**
     * @brief      Called to update the entity energy level based on its speed.
     *
     * @param[in]  speed  The speed associated with the entity
     *
     * @return     True if added energy is enough to perform an action.
     */
    void accumulate(int speed);

    /**
     * @brief      Spend energy required to take a turn. Should not be called when
     *             canTakeTurn is false.
     */
    void spend();

    /// Required level of energy to perform an action
    static constexpr int ActionThreshold{250};

    /// Reference gain at reference speed level
    static constexpr int ReferenceGain{60};

    /// Minimal allowed speed level (accounting for bonus / malus)
    static constexpr int MinSpeed{0};

    /// Reference speed level
    static constexpr int NormalSpeed{5};

    /// Maximal allowed speed level (accounting for bonus / malus)
    static constexpr int MaxSpeed{10};

    /// Mapper from speed to energy amount.
    static constexpr int SpeedTable[MaxSpeed - MinSpeed + 1]{
        ReferenceGain / 4,                     // / 4
        ReferenceGain / 3,                     // / 3
        static_cast<int>(ReferenceGain / 2.5), // / 2.5
        ReferenceGain / 2,                     // / 2
        static_cast<int>(ReferenceGain / 1.5), // / 1.5
        ReferenceGain,                         // ReferenceGain
        static_cast<int>(ReferenceGain * 1.5), // x 1.5
        ReferenceGain * 2,                     // x 2
        static_cast<int>(ReferenceGain * 2.5), // x 2.5
        ReferenceGain * 3,                     // x 3
        ReferenceGain * 4                      // x 4
    };

  private:
    int m_amount{0};
};

} // namespace cppRogue

#endif
