#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.hpp"
#include "Fractals/MandelbrotSet.hpp"

namespace ng {

    class MandelbrotState : public State {
    public:
        // constructor / destructor
        MandelbrotState();
        ~MandelbrotState() override;

        // public methods
        void mouseWheelMoved(const sf::Event& event) override;
        void keyPressed(const sf::Event& event) override;

        void updateInput(const float& ftime);
        void updateText();

        void update(const float& ftime) override;
        void render(sf::RenderTarget& target) override;

    private:
        // enums
        enum class TextRole : short { Statistic = 0, Left, Right, Top, Bottom };

        // member data
        MandelbrotSet                          m_mandelbrotSet;
        sf::Font                               m_font;
        std::unordered_map<TextRole, sf::Text> m_text;
        bool                                   m_showCoordinates;

        // member methods
        void loadFonts();
        void initText();
        void initMandelbrotSet();

    }; // class MandelbrotState

} // namespace ng