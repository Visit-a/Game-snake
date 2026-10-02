#include "Wall.h"
#include "Constants.h"

namespace SnakeGame
{
    static void AddWall(std::vector<Wall>& walls, sf::Texture& texture,
        int gridX, int gridY)
    {
        Wall wall;
        wall.position.x = gridX * SEGMENT_SIZE + SEGMENT_SIZE / 2.f;
        wall.position.y = gridY * SEGMENT_SIZE + SEGMENT_SIZE / 2.f;

        wall.sprite.setTexture(texture);
        sf::FloatRect rect = wall.sprite.getLocalBounds();
        if (rect.width > 0 && rect.height > 0)
        {
            wall.sprite.setScale(SEGMENT_SIZE / rect.width,
                SEGMENT_SIZE / rect.height);
        }
        wall.sprite.setOrigin(0.f, 0.f);
        wall.sprite.setPosition(wall.position.x - SEGMENT_SIZE / 2.f,
            wall.position.y - SEGMENT_SIZE / 2.f);
        walls.push_back(wall);
    }

    void InitWalls(std::vector<Wall>& walls, sf::Texture& texture)
    {
        walls.clear();

        int cols = SCREEN_WIDTH / (int)SEGMENT_SIZE;
        int rows = SCREEN_HEIGHT / (int)SEGMENT_SIZE;

        for (int x = 0; x < cols; ++x)
        {
            AddWall(walls, texture, x, 0);
            AddWall(walls, texture, x, rows - 1);
        }
        for (int y = 1; y < rows - 1; ++y)
        {
            AddWall(walls, texture, 0, y);
            AddWall(walls, texture, cols - 1, y);
        }
    }

    bool IsCellInWalls(const std::vector<Wall>& walls, Position2D cell)
    {
        for (const auto& wall : walls)
        {
            if (IsSameCell(wall.position, cell, SEGMENT_SIZE))
                return true;
        }
        return false;
    }
}