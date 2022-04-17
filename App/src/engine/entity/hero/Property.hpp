#ifndef CPPROGUE_PROPERTY_HPP
#define CPPROGUE_PROPERTY_HPP

// @EXERCICE 1

#include "Statistic.hpp"

namespace cppRogue::entity {

/**
 * @brief      Base property used as a base class for allowed property.
 */
class Property
{
  public:
    Property(Statistic kind, std::string name, int maxValue) noexcept;
    virtual ~Property() = default;

    // Getters
    [[nodiscard]] Statistic kind() const;
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] int value() const;
    [[nodiscard]] int maxValue() const;

    // Setters
    void set(int baseValue);
    void setBonus(int bonus);
    void setOffset(int offset);

  private:
    Statistic m_kind;
    std::string m_name;
    int m_maxValue{};

    int m_baseValue{};
    int m_bonus{};
    int m_offset{};
};

/**
 * @brief      Strength property implementation.
 */
class Strength : public Property
{
  public:
    explicit Strength(int maxValue) noexcept;
    ~Strength() override = default;

    /**
     * @brief      A scale value based on weapon @p heft
     *
     * @param[in]  heft  The weapon heft
     *
     * @return     A scale to apply to final damage.
     */
    [[nodiscard]] double damageScale(int heft) const;
};

/**
 * @brief      Endurance property implementation.
 */
class Endurance : public Property
{
  public:
    explicit Endurance(int maxValue) noexcept;
    ~Endurance() override = default;

    /**
     * @brief      Maximum health value based on endurance level.
     *
     * @return     Maximum health level.
     */
    [[nodiscard]] int maxHealth() const { return value(); }
};

} // namespace cppRogue::entity

#endif
