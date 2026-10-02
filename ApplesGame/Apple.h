#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Math.h"

namespace SnakeGame
{
    struct Apple
    {
        Position2D position;
        sf::Sprite sprite;
    };

    void InitApple(Apple& apple, sf::Texture& texture);
    void RespawnApple(Apple& apple,
        const std::vector<Position2D>& occupiedCells,
        const std::vector<Position2D>& wallCells);
}