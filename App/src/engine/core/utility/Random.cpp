#include "Random.hpp"

#include <array>


namespace cppRogue {

std::default_random_engine Random::m_seededGenerator{};

Random::Random()
{
    // Generate seed based on system entropy
    std::array<int, 5> seed{};
    std::generate_n(std::begin(seed), seed.size(), std::ref(m_device));
    std::seed_seq seedSeq(std::begin(seed), std::end(seed));

    m_engine = std::mt19937{seedSeq};
}

std::default_random_engine Random::seededGenerator() { return m_seededGenerator; }

std::mt19937 Random::generator()
{
    static Random generator = Random{}; // Initialized only once
    return generator.m_engine;
}

} // namespace cppRogue
