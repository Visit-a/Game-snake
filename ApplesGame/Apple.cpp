#include "Apple.h"
#include "Constants.h"
#include <cstdlib>
#include <iostream>

namespace SnakeGame
{
    void InitApple(Apple& apple, sf::Texture& texture)
    {
        apple.sprite.setTexture(texture);
        sf::FloatRect rect = apple.sprite.getLocalBounds();
        if (rect.width > 0 && rect.height > 0)
        {
            apple.sprite.setScale(SEGMENT_SIZE / rect.width,
                SEGMENT_SIZE / rect.height);
        }
        apple.sprite.setOrigin(0.f, 0.f);
    }

    void RespawnApple(Apple& apple,
        const std::vector<Position2D>& occupiedCells,
        const std::vector<Position2D>& wallCells)
    {
        int cols = SCREEN_WIDTH / (int)SEGMENT_SIZE;
        int rows = SCREEN_HEIGHT / (int)SEGMENT_SIZE;

        int attempts = 0;
        bool found = false;
        Position2D newPos;

        while (!found && attempts < 2000)
        {
            // Только внутренние клетки — [1, cols-2] × [1, rows-2]
            int gridX = 1 + rand() % (cols - 2);
            int gridY = 1 + rand() % (rows - 2);

            // Центр клетки
            newPos.x = gridX * SEGMENT_SIZE + SEGMENT_SIZE / 2.f;
            newPos.y = gridY * SEGMENT_SIZE + SEGMENT_SIZE / 2.f;

            found = true;

            // Не на змейке
            for (const auto& cell : occupiedCells)
            {
                if (IsSameCell(newPos, cell, SEGMENT_SIZE))
                {
                    found = false;
                    break;
                }
            }
            if (!found) { ++attempts; continue; }

            // Не на стене
            for (const auto& cell : wallCells)
            {
                if (IsSameCell(newPos, cell, SEGMENT_SIZE))
                {
                    found = false;
                    break;
                }
            }

            ++attempts;
        }

        if (!found)
        {
            std::cout << "Warning: could not find free cell for apple\n";
        }

        apple.position = newPos;
        apple.sprite.setPosition(apple.position.x - SEGMENT_SIZE / 2.f,
            apple.position.y - SEGMENT_SIZE / 2.f);
    }
}