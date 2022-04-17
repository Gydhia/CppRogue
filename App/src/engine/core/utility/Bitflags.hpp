#ifndef CPPROGUE_BITFLAGS_HPP
#define CPPROGUE_BITFLAGS_HPP

#include <bitset>
#include <limits>
#include <type_traits>

namespace cppRogue {

template <typename EnumT, std::size_t Count, bool IsEnum = std::is_enum<EnumT>::value>
class Bitflags;

/**
 * @brief      Type safe enum Bitflags for any enum class. These implementation avoid
 *             pitfalls of mixing flag and collection of flag:
 *               - each flag does not need to be a power of two
 *               - unlimited number of flags
 *               - clear distinction between a flag and a collection of flags
 *               - type safe manipulation forced at compile time
 *               - convenience methods / operators
 * @note       There is two type of enum in C++ type safe enum (enum class) and C style
 *             enum (enum). Bitwise operations can be used directly with C style enum and
 *             template and overloading allows to add support for type safe enum too.
 *
 * @tparam     EnumT  Type of the enum class used for the collection of flag
 * @tparam     Count  Number of different flags allowed for this @p EnumT
 */
template <typename EnumT, std::size_t Count> class Bitflags<EnumT, Count, true>
{
  public:
    using UnsignedType =
        typename std::make_unsigned<typename std::underlying_type<EnumT>::type>::type;

    constexpr Bitflags() = default;
    constexpr explicit Bitflags(EnumT value) { set(value, true); }
    constexpr Bitflags(std::initializer_list<EnumT> enumList)
    {
        for (auto&& flag : enumList) { m_bits.set(asInternal(flag), true); }
    }

    [[nodiscard]] constexpr bool any() const { return m_bits.any(); }

    [[nodiscard]] constexpr bool all() const { return m_bits.all(); }

    [[nodiscard]] constexpr bool none() const { return m_bits.none(); }

    [[nodiscard]] constexpr std::size_t size() const { return m_bits.size(); }

    [[nodiscard]] constexpr std::size_t count() const { return m_bits.count(); }

    explicit operator bool() const { return m_bits.any(); }

    /**
     * @brief      Test if a specific enum value is active (on) or not
     *
     * @param[in]  value  The enum value to be tested.
     *
     * @return     True if @p value is active (on) else false.
     */
    [[nodiscard]] constexpr bool enabled(EnumT value) const
    {
        auto index = asInternal(std::move(value));
        return m_bits.test(index);
    }

    /**
     * @brief      Test if a specific enum value is inactive (off) or not
     *
     * @param[in]  value  The enum value to be tested.
     *
     * @return     True if @p value is inactive (off) else false.
     */
    [[nodiscard]] constexpr bool disabled(EnumT value) const { return !enabled(value); }

    constexpr Bitflags& set(EnumT value, bool state = true)
    {
        m_bits.set(asInternal(std::move(value)), state);
        return *this;
    }

    constexpr Bitflags& unset(EnumT value)
    {
        m_bits.reset(asInternal(std::move(value)));
        return *this;
    }

    Bitflags& flip(EnumT value)
    {
        m_bits.flip(asInternal(std::move(value)));
        return *this;
    }

    [[nodiscard]] constexpr bool overlaps(const Bitflags& wrapper)
    {
        return (this->m_bits & wrapper.m_bits) != 0;
    }

    // Operators using wrapped Enum
    [[nodiscard]] constexpr Bitflags operator|(EnumT value) const
    {
        Bitflags result = *this;
        result.m_bits |= asInternal(std::move(value));
        return result;
    }
    [[nodiscard]] constexpr Bitflags operator&(EnumT value) const
    {
        Bitflags result = *this;
        result &= value;
        return result;
    }
    [[nodiscard]] constexpr Bitflags operator^(EnumT value) const
    {
        Bitflags result = *this;
        result.m_bits ^= asInternal(std::move(value));
        return result;
    }
    [[nodiscard]] constexpr Bitflags operator+(EnumT value) const
    {
        Bitflags result = *this;
        result += value;
        return result;
    }
    [[nodiscard]] constexpr Bitflags operator-(EnumT value) const
    {
        Bitflags result = *this;
        result -= value;
        return result;
    }

    constexpr Bitflags& operator|=(EnumT value)
    {
        m_bits.set(asInternal(std::move(value)), true);
        return *this;
    }
    constexpr Bitflags& operator&=(EnumT value)
    {
        auto underlying(asInternal(std::move(value)));
        bool tmp = m_bits.test(underlying);
        m_bits.reset();
        m_bits.set(underlying, tmp);
        return *this;
    }
    constexpr Bitflags& operator+=(EnumT value) { return operator|=(value); }
    constexpr Bitflags& operator-=(EnumT value)
    {
        m_bits.set(asInternal(std::move(value)), false);
        return *this;
    }

    bool operator[](EnumT value) { return m_bits[asInternal(std::move((value)))]; }

    // Operators using wrapper class
    [[nodiscard]] constexpr Bitflags operator~() const
    {
        Bitflags result = *this;
        result.m_bits.flip();
        return result;
    }

    constexpr Bitflags& operator|=(Bitflags wrapper)
    {
        m_bits |= wrapper.m_bits;
        return *this;
    }
    constexpr Bitflags& operator&=(Bitflags wrapper)
    {
        m_bits &= wrapper.m_bits;
        return *this;
    }
    constexpr Bitflags& operator+=(Bitflags wrapper) { return operator|=(wrapper); }
    constexpr Bitflags operator-=(Bitflags wrapper)
    {
        wrapper.m_bits.flip();
        this->m_bits &= std::move(wrapper.m_bits);
        return *this;
    }

    friend constexpr Bitflags<EnumT, Count> operator|(Bitflags<EnumT, Count> lhs,
                                                      const Bitflags<EnumT, Count>& rhs)
    {
        lhs |= rhs;
        return lhs;
    }
    friend constexpr Bitflags<EnumT, Count> operator&(Bitflags<EnumT, Count> lhs,
                                                      const Bitflags<EnumT, Count>& rhs)
    {
        lhs &= rhs;
        return lhs;
    }
    friend constexpr Bitflags<EnumT, Count> operator+(Bitflags<EnumT, Count> lhs,
                                                      const Bitflags<EnumT, Count>& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Bitflags<EnumT, Count> operator-(Bitflags<EnumT, Count> lhs,
                                                      const Bitflags<EnumT, Count>& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend constexpr bool operator==(const Bitflags<EnumT, Count>& lhs,
                                     const Bitflags<EnumT, Count>& rhs)
    {
        return lhs.m_bits == rhs.m_bits;
    }
    friend constexpr bool operator!=(const Bitflags<EnumT, Count>& lhs,
                                     const Bitflags<EnumT, Count>& rhs)
    {
        return lhs.m_bits != rhs.m_bits;
    }

    // Operator for outputting to std::ostream.
    friend std::ostream& operator<<(std::ostream& stream, const Bitflags& me)
    {
        return stream << me.m_bits;
    }

  private:
    static constexpr UnsignedType asInternal(EnumT value)
    {
        return static_cast<UnsignedType>(value);
    }

    std::bitset<Count> m_bits{};
};

template <typename EnumT, std::size_t Count>
constexpr typename std::enable_if<std::is_enum<EnumT>::value, Bitflags<EnumT, Count>>::type
operator|(EnumT left, EnumT right)
{
    return Bitflags<EnumT, Count>(left) | right;
}
template <typename EnumT, std::size_t Count>
constexpr typename std::enable_if<std::is_enum<EnumT>::value, Bitflags<EnumT, Count>>::type
operator&(EnumT left, EnumT right)
{
    return Bitflags<EnumT, Count>(left) & right;
}
template <typename EnumT, std::size_t Count>
constexpr typename std::enable_if<std::is_enum<EnumT>::value, Bitflags<EnumT, Count>>::type
operator^(EnumT left, EnumT right)
{
    return Bitflags<EnumT, Count>(left) ^ right;
}
template <typename EnumT, std::size_t Count>
constexpr typename std::enable_if<std::is_enum<EnumT>::value, Bitflags<EnumT, Count>>::type
operator+(EnumT left, EnumT right)
{
    return Bitflags<EnumT, Count>(left) | right;
}

} // namespace cppRogue

#endif
