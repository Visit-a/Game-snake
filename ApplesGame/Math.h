#pragma once

namespace sf { class Sprite; }

namespace SnakeGame
{
    struct Vector2D
    {
        float x = 0;
        float y = 0;
    };

    typedef Vector2D Position2D;

    bool IsRectanglesCollide(Position2D rect1Position, Vector2D rect1Size,
        Position2D rect2Position, Vector2D rect2Size);

    bool IsCirclesCollide(Position2D circle1Position, float circle1Radius,
        Position2D circle2Position, float circle2Radius);

    // Позиции — центры клеток. Возвращает true, если обе в одной клетке.
    bool IsSameCell(Position2D a, Position2D b, float cellSize);

    void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight);
    void SetSpriteOrigin(sf::Sprite& sprite, float originX, float originY);
}