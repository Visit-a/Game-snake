#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Math.h"

namespace SnakeGame
{
    struct Wall
    {
        Position2D position;
        sf::Sprite sprite;
    };

    void InitWalls(std::vector<Wall>& walls, sf::Texture& texture);

    // Проверка: находится ли клетка в стене
    bool IsCellInWalls(const std::vector<Wall>& walls, Position2D cell);
}