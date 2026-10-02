#include "Math.h"
#include <cstdlib>
#include <cmath>
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
    bool IsRectanglesCollide(Position2D rect1Position, Vector2D rect1Size,
        Position2D rect2Position, Vector2D rect2Size)
    {
        float dx = (float)fabs(rect1Position.x - rect2Position.x);
        float dy = (float)fabs(rect1Position.y - rect2Position.y);
        return (dx <= (rect1Size.x + rect2Size.x) / 2.f &&
            dy <= (rect1Size.y + rect2Size.y) / 2.f);
    }

    bool IsCirclesCollide(Position2D c1, float r1, Position2D c2, float r2)
    {
        float squareDistance = (c1.x - c2.x) * (c1.x - c2.x) +
            (c1.y - c2.y) * (c1.y - c2.y);
        float squareRadiusSum = (r1 + r2) * (r1 + r2);
        return squareDistance <= squareRadiusSum;
    }

    bool IsSameCell(Position2D a, Position2D b, float cellSize)
    {
        // Округляем, чтобы избежать ошибок float. Позиции — центры клеток,
        // поэтому делим и округляем к ближайшему целому.
        int ax = (int)floor(a.x / cellSize + 0.5f);
        int ay = (int)floor(a.y / cellSize + 0.5f);
        int bx = (int)floor(b.x / cellSize + 0.5f);
        int by = (int)floor(b.y / cellSize + 0.5f);
        return ax == bx && ay == by;
    }

    void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
    {
        sf::FloatRect rect = sprite.getLocalBounds();
        if (rect.width == 0.f || rect.height == 0.f) return;
        sf::Vector2f scale = { desiredWidth / rect.width, desiredHeight / rect.height };
        sprite.setScale(scale);
    }

    void SetSpriteOrigin(sf::Sprite& sprite, float originX, float originY)
    {
        sf::FloatRect rect = sprite.getLocalBounds();
        sprite.setOrigin(originX + rect.width, originY + rect.height);
    }
}