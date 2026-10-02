#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"
#include "Math.h"

namespace SnakeGame
{
    enum class Direction
    {
        Right = 0,
        Up,
        Left,
        Down
    };

    struct SnakeSegment
    {
        Position2D position;
        sf::Sprite sprite;
    };

    struct Snake
    {
        std::vector<SnakeSegment> segments;
        Direction direction = Direction::Right;
        Direction pendingDirection = Direction::Right;
        float speed = EASY_SPEED;
        float moveTimer = 0.f;
        int scorePerApple = EASY_SCORE;
        bool isAlive = true;

        sf::Texture headRight, headLeft, headUp, headDown;
        sf::Texture bodyHorizontal, bodyVertical;
        sf::Texture bodyTopLeft, bodyTopRight;
        sf::Texture bodyBottomLeft, bodyBottomRight;
        sf::Texture tailRight, tailLeft, tailUp, tailDown;
    };

    void LoadSnakeTextures(Snake& snake);
    void InitSnake(Snake& snake);
    void UpdateSnake(Snake& snake, float deltaTime);
    void SetSnakeDirection(Snake& snake, Direction newDirection);
    void GrowSnake(Snake& snake);
    void DrawSnake(Snake& snake, sf::RenderWindow& window);

    bool IsSnakeCollideWithSelf(const Snake& snake);
    bool IsSnakeCollideWithWall(const Snake& snake, Position2D wallPos);

    sf::Texture& GetHeadTexture(Snake& snake, Direction dir);
    sf::Texture& GetTailTexture(Snake& snake, Direction dir);
    sf::Texture& GetBodyTexture(Snake& snake, int index);
}