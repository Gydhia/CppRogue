#ifndef CPPROGUE_ARCHETYPE_HPP
#define CPPROGUE_ARCHETYPE_HPP

#include <any>
#include <type_traits>
#include <utility>

namespace cppRogue {

/**
 * @brief      An archetype that can be used to build a single inheritance tree with value
 *             semantics and no heap allocation for small object.
 *
 * @note       Concrete wrapped type should have move semantic for maximum efficiency.
 *             Construction is done using the idiom pass by value then move to avoid
 *             perfect forwarding idiom pitfalls. More at
 *             https://mpark.github.io/programming/2014/06/07/beware-of-perfect-forwarding-constructors/
 *
 * @tparam     InterfaceType  Public interface that all concrete implementation must
 *                            expose.
 */
template <typename InterfaceType> class Archetype
{
  public:
    template <typename ConcreteType>
    explicit Archetype(ConcreteType manager)
        : m_storage{std::move(manager)}, m_getter{[](std::any& storage) -> InterfaceType& {
              return std::any_cast<ConcreteType&>(storage);
          }}
    {
    }

    InterfaceType* operator->() { return &m_getter(m_storage); }

    InterfaceType& get() { return m_getter(m_storage); }

  private:
    std::any m_storage;
    InterfaceType& (*m_getter)(std::any&);
};

} // namespace cppRogue

#endif
