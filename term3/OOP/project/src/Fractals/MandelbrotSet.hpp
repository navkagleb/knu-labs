#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include <cmath>
#include <vector>
#include <iostream>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <ThreadPool/ThreadPool.hpp>

namespace ng {

    class MandelbrotSet : public sf::Drawable {
    public:
        // enums
        enum class Bounds : short { MinX = 0, MaxX, MinY, MaxY };
        enum class ImplementationType : short {
            Pseudocode = 0,
            ByHand,
            AVX2,
            Threads,
            ThreadPool
        };

        // aliases
        using PointType       = double;
        using BoundsContainer = std::unordered_map<Bounds, PointType>;

        // constructor / destructor
        explicit MandelbrotSet(const sf::Vector2u& size);
        ~MandelbrotSet() override = default;

        // accessors
        [[nodiscard]] inline unsigned getIterations() const { return m_iterations; }
        [[nodiscard]] inline BoundsContainer getLocalBounds() const { return m_bounds; }
        [[nodiscard]] inline const sf::Vector2<PointType>& getSize() const { return m_size; }
        [[nodiscard]] inline ImplementationType getImplementation() const { return m_implementation; }
        [[nodiscard]] inline bool getColoring() const { return m_coloring; }

        // modifiers
        void setImplementation(const ImplementationType& type);
        void setColoring(bool coloring);

        // public methods
        void move(float offsetX, float offsetY);
        void zoom(float factor);
        void increaseIterations();
        void decreaseIterations();
        void update();

    private:
        // aliases
        using Implementation          = std::function<void(std::size_t, std::size_t, bool)>;
        using ImplementationContainer = std::unordered_map<ImplementationType, Implementation>;

        // member data
        sf::Image               m_image;
        sf::Texture             m_texture;
        sf::Sprite              m_sprite;
        sf::Vector2<PointType>  m_size;
        sf::Vector2<PointType>  m_scale;
        BoundsContainer         m_bounds;
        ImplementationType      m_implementation;
        ImplementationContainer m_implementations;
        ThreadPool              m_threadPool;
        unsigned                m_iterations;
        bool                    m_coloring;

        // member methods
        sf::Color getColor(int iterations) const;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    }; // class MandelbrotSet

} // namespace ng