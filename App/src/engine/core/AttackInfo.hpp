#ifndef CPPROGUE_ATTACKINFO_HPP
#define CPPROGUE_ATTACKINFO_HPP

#include <ostream>
#include <string>
#include <string_view>

namespace cppRogue {

class AttackInfo
{
  public:
    /**
     * @brief      Special property of an attack. Can be short range attack (Melee), a
     *             spell (Magic) or a long ranged attack (Ranged).
     */
    enum class Kind
    {
        Melee,
        Ranged,
        Magic
    };

    AttackInfo() = default;
    explicit AttackInfo(std::string name, int damage)
        : m_name{std::move(name)}, m_damage{damage} {};
    ~AttackInfo() = default;

    [[nodiscard]] inline Kind kind() const { return m_kind; }
    [[nodiscard]] inline int damage() const { return m_damage; }
    [[nodiscard]] inline int range() const { return m_range; }
    [[nodiscard]] inline std::string_view name() const { return m_name; }

    friend bool operator==(const AttackInfo& lhs, const AttackInfo& rhs)
    {
        return (lhs.m_name == rhs.m_name) && (lhs.m_damage == rhs.m_damage) &&
               (lhs.m_range == rhs.m_range);
    }
    friend bool operator!=(const AttackInfo& lhs, const AttackInfo& rhs) { return !(lhs == rhs); }

    friend std::ostream& operator<<(std::ostream& os, const AttackInfo& attack)
    {
        const char* kindAsString = [&attack]() {
            switch (attack.m_kind)
            {
            case Kind::Melee: return "Melee";
            case Kind::Ranged: return "Ranged";
            case Kind::Magic: return "Magic";
            default: return "Unknown";
            };
        }();

        os << "{Attack -- " << attack.m_name << ", Kind=" << kindAsString
           << ", Atq=" << attack.m_damage << "}";
        return os;
    }

  private:
    std::string m_name{"None"};
    int m_damage{};
    int m_range{};
    Kind m_kind{Kind::Melee};
};

} // namespace cppRogue

#endif
