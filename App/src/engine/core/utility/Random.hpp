#ifndef CPPROGUE_RANDOM_HPP
#define CPPROGUE_RANDOM_HPP

#include <algorithm>
#include <random>

namespace cppRogue {

/**
 * @brief      A static class helper to deal with randomness.
 */
class Random
{
  public:
    // Random with entropy defined seed
    [[nodiscard]] static std::mt19937 generator();

    // Random with user defined seed
    [[nodiscard]] static std::default_random_engine seededGenerator();

    static void seed(int seed) { m_seededGenerator.seed(seed); }

  private:
    Random();
    ~Random() = default;

    std::random_device m_device{};
    std::mt19937 m_engine{};

    static std::default_random_engine m_seededGenerator;
};

} // namespace cppRogue

#endif
