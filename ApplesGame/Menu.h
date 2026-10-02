#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace SnakeGame
{
    struct MenuItem
    {
        std::string text;
        bool isSelected = false;
    };

    struct Menu
    {
        std::vector<MenuItem> items;
        int selectedIndex = 0;
        sf::Font font;
        sf::Text titleText;
        std::vector<sf::Text> itemTexts;

        void Init(const std::string& title, const std::vector<std::string>& options);
        void MoveUp();
        void MoveDown();
        void Draw(sf::RenderWindow& window);

        // Обновление заголовка и пунктов без пересоздания меню
        void SetContent(const std::string& title, const std::vector<std::string>& options);
    };
}