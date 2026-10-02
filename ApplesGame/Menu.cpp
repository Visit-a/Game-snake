#include "Menu.h"
#include "Constants.h"

namespace SnakeGame
{
    void Menu::Init(const std::string& title, const std::vector<std::string>& options)
    {
        font.loadFromFile(RESOURCES_PATH + "arial.ttf");

        titleText.setFont(font);
        titleText.setString(title);
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(SCREEN_WIDTH / 2.f - 100.f, 60.f);

        items.clear();
        itemTexts.clear();
        selectedIndex = 0;

        for (size_t i = 0; i < options.size(); ++i)
        {
            MenuItem item;
            item.text = options[i];
            item.isSelected = (i == 0);
            items.push_back(item);

            sf::Text t;
            t.setFont(font);
            t.setString(options[i]);
            t.setCharacterSize(28);
            t.setPosition(SCREEN_WIDTH / 2.f - 100.f, 180.f + i * 50.f);
            itemTexts.push_back(t);
        }
    }

    void Menu::SetContent(const std::string& title, const std::vector<std::string>& options)
    {
        titleText.setString(title);

        for (size_t i = 0; i < items.size() && i < options.size(); ++i)
        {
            items[i].text = options[i];
            itemTexts[i].setString(options[i]);
        }
    }

    void Menu::MoveUp()
    {
        if (items.empty()) return;
        items[selectedIndex].isSelected = false;
        selectedIndex = (selectedIndex - 1 + (int)items.size()) % (int)items.size();
        items[selectedIndex].isSelected = true;
    }

    void Menu::MoveDown()
    {
        if (items.empty()) return;
        items[selectedIndex].isSelected = false;
        selectedIndex = (selectedIndex + 1) % (int)items.size();
        items[selectedIndex].isSelected = true;
    }

    void Menu::Draw(sf::RenderWindow& window)
    {
        window.draw(titleText);

        for (size_t i = 0; i < itemTexts.size(); ++i)
        {
            if (items[i].isSelected)
                itemTexts[i].setFillColor(sf::Color::Green);
            else
                itemTexts[i].setFillColor(sf::Color::White);

            window.draw(itemTexts[i]);
        }
    }
}