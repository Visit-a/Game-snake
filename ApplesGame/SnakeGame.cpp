#include <SFML/Graphics.hpp>
#include <ctime>
#include "Game.h"

int main()
{
    using namespace SnakeGame;

    srand((unsigned)time(nullptr));

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake Game");

    Game game;
    InitGame(game);

    sf::Clock clock;
    float lastTime = clock.getElapsedTime().asSeconds();

    while (window.isOpen())
    {
        float currentTime = clock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        UpdateGame(game, deltaTime);

        window.clear();
        DrawGame(game, window);
        window.display();
    }

    DeinitializeGame(game);
    return 0;
}