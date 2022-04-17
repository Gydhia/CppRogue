#ifndef CPPROGUE_CONTENTMANAGER_HPP
#define CPPROGUE_CONTENTMANAGER_HPP

#include "engine/collectable/Item.hpp"
#include "engine/core/utility/Algorithm.hpp"
#include "engine/core/utility/IO.hpp"
#include "engine/entity/hero/Hero.hpp"
#include "engine/entity/hero/Race.hpp"
#include "engine/entity/monster/Breed.hpp"

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <optional>
#include <vector>

// @TODO
// @TODO Ajouter le chargement des "Breed" lors de la lecture du JSON
// @TODO

namespace cppRogue {

class ContentManager
{
  public:
    ContentManager() = default;
    ~ContentManager() = default;

    bool load(const std::string& path);

    [[nodiscard]] inline std::vector<std::shared_ptr<collectable::Item>>& items()
    {
        return m_items;
    }

    [[nodiscard]] inline const std::vector<std::shared_ptr<entity::Breed>>& breeds() const
    {
        return m_breeds;
    }

    [[nodiscard]] inline const std::vector<std::shared_ptr<entity::Race>>& races()
    {
        return m_races;
    }

    [[nodiscard]] inline std::shared_ptr<entity::Hero>& hero() { return m_hero; }

  private:
    bool loadHero(const std::string& relativePath);
    bool loadItems(const std::string& relativePath);
    bool loadRaces(const std::string& relativePath);
    bool loadBreeds(const std::string& relativePath);

    /**
     * @brief      Searches for the first match in a specific container
     *
     * @param[in]  container   The container used for the search
     * @param[in]  identifier  The unique identifier used to search for the object
     *
     * @tparam     T           Type of the object to be found
     *
     * @return     Reference to found item or nothing
     */
    template <typename T> T find(std::vector<T>& container, const std::string& identifier)
    {
        const auto searchedName = algorithm::lower(identifier);
        auto result =
            std::find_if(container.begin(), container.end(), [&searchedName](const auto& current) {
                const std::string objectName = algorithm::lower(std::string(current->name()));
                return objectName == searchedName;
            });

        if (result == container.end()) { return nullptr; }

        // Get address of race by first deferencing the iterator
        return *result;
    }

    // Game data storage
    std::vector<std::shared_ptr<sf::Texture>> m_textures{};
    std::vector<std::shared_ptr<entity::Race>> m_races{};
    std::vector<std::shared_ptr<collectable::Item>> m_items{};
    std::vector<std::shared_ptr<entity::Breed>> m_breeds{};

    std::shared_ptr<entity::Hero> m_hero{};

    std::filesystem::path m_rootPath{};
};

} // namespace cppRogue

#endif
