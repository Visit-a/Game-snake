#include "Snake.h"

namespace SnakeGame
{
    void LoadSnakeTextures(Snake& snake)
    {
        snake.headRight.loadFromFile(RESOURCES_PATH + "head_right.png");
        snake.headLeft.loadFromFile(RESOURCES_PATH + "head_left.png");
        snake.headUp.loadFromFile(RESOURCES_PATH + "head_up.png");
        snake.headDown.loadFromFile(RESOURCES_PATH + "head_down.png");

        snake.bodyHorizontal.loadFromFile(RESOURCES_PATH + "body_horizontal.png");
        snake.bodyVertical.loadFromFile(RESOURCES_PATH + "body_vertical.png");
        snake.bodyTopLeft.loadFromFile(RESOURCES_PATH + "body_topleft.png");
        snake.bodyTopRight.loadFromFile(RESOURCES_PATH + "body_topright.png");
        snake.bodyBottomLeft.loadFromFile(RESOURCES_PATH + "body_bottomleft.png");
        snake.bodyBottomRight.loadFromFile(RESOURCES_PATH + "body_bottomright.png");

        snake.tailRight.loadFromFile(RESOURCES_PATH + "tail_right.png");
        snake.tailLeft.loadFromFile(RESOURCES_PATH + "tail_left.png");
        snake.tailUp.loadFromFile(RESOURCES_PATH + "tail_up.png");
        snake.tailDown.loadFromFile(RESOURCES_PATH + "tail_down.png");
    }

    sf::Texture& GetHeadTexture(Snake& snake, Direction dir)
    {
        switch (dir)
        {
        case Direction::Right: return snake.headRight;
        case Direction::Left:  return snake.headLeft;
        case Direction::Up:    return snake.headUp;
        case Direction::Down:  return snake.headDown;
        }
        return snake.headRight;
    }

    sf::Texture& GetTailTexture(Snake& snake, Direction dir)
    {
        switch (dir)
        {
        case Direction::Right: return snake.tailRight;
        case Direction::Left:  return snake.tailLeft;
        case Direction::Up:    return snake.tailUp;
        case Direction::Down:  return snake.tailDown;
        }
        return snake.tailRight;
    }

    static Direction GetDirectionFromTo(Position2D from, Position2D to)
    {
        if (to.x > from.x) return Direction::Right;
        if (to.x < from.x) return Direction::Left;
        if (to.y > from.y) return Direction::Down;
        return Direction::Up;
    }

    static void ApplyTexture(sf::Sprite& sprite, sf::Texture& texture)
    {
        sprite.setTexture(texture);
        sf::FloatRect rect = sprite.getLocalBounds();
        if (rect.width > 0 && rect.height > 0)
        {
            sprite.setScale(SEGMENT_SIZE / rect.width,
                SEGMENT_SIZE / rect.height);
        }
        sprite.setOrigin(0.f, 0.f);
    }

    static void ApplySegmentVisual(SnakeSegment& seg)
    {
        seg.sprite.setPosition(seg.position.x - SEGMENT_SIZE / 2.f,
            seg.position.y - SEGMENT_SIZE / 2.f);
    }

    sf::Texture& GetBodyTexture(Snake& snake, int index)
    {
        Position2D prev = snake.segments[index - 1].position;
        Position2D cur = snake.segments[index].position;
        Position2D next = snake.segments[index + 1].position;

        bool prevHorizontal = (prev.y == cur.y);
        bool nextHorizontal = (next.y == cur.y);

        if (prevHorizontal && nextHorizontal) return snake.bodyHorizontal;
        if (!prevHorizontal && !nextHorizontal) return snake.bodyVertical;

        Direction dirPrev = GetDirectionFromTo(cur, prev);
        Direction dirNext = GetDirectionFromTo(cur, next);

        if ((dirPrev == Direction::Left && dirNext == Direction::Down) ||
            (dirPrev == Direction::Down && dirNext == Direction::Left))
            return snake.bodyTopLeft;

        if ((dirPrev == Direction::Right && dirNext == Direction::Down) ||
            (dirPrev == Direction::Down && dirNext == Direction::Right))
            return snake.bodyTopRight;

        if ((dirPrev == Direction::Left && dirNext == Direction::Up) ||
            (dirPrev == Direction::Up && dirNext == Direction::Left))
            return snake.bodyBottomLeft;

        if ((dirPrev == Direction::Right && dirNext == Direction::Up) ||
            (dirPrev == Direction::Up && dirNext == Direction::Right))
            return snake.bodyBottomRight;

        return snake.bodyHorizontal;
    }

    void InitSnake(Snake& snake)
    {
        LoadSnakeTextures(snake);

        snake.segments.clear();
        snake.direction = Direction::Right;
        snake.pendingDirection = Direction::Right;
        snake.moveTimer = 0.f;
        snake.isAlive = true;

        int gridX = (SCREEN_WIDTH / (int)SEGMENT_SIZE) / 2;
        int gridY = (SCREEN_HEIGHT / (int)SEGMENT_SIZE) / 2;

        float startX = gridX * SEGMENT_SIZE + SEGMENT_SIZE / 2.f;
        float startY = gridY * SEGMENT_SIZE + SEGMENT_SIZE / 2.f;

        for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i)
        {
            SnakeSegment segment;
            segment.position.x = startX - i * SEGMENT_SIZE;
            segment.position.y = startY;

            if (i == 0)
                ApplyTexture(segment.sprite, GetHeadTexture(snake, Direction::Right));
            else if (i == INITIAL_SNAKE_LENGTH - 1)
                ApplyTexture(segment.sprite, GetTailTexture(snake, Direction::Left));
            else
                ApplyTexture(segment.sprite, snake.bodyHorizontal);

            ApplySegmentVisual(segment);
            snake.segments.push_back(segment);
        }
    }

    void SetSnakeDirection(Snake& snake, Direction newDirection)
    {
        bool isOpposite =
            (snake.direction == Direction::Right && newDirection == Direction::Left) ||
            (snake.direction == Direction::Left && newDirection == Direction::Right) ||
            (snake.direction == Direction::Up && newDirection == Direction::Down) ||
            (snake.direction == Direction::Down && newDirection == Direction::Up);

        if (!isOpposite)
            snake.pendingDirection = newDirection;
    }

    void UpdateSnake(Snake& snake, float deltaTime)
    {
        if (!snake.isAlive) return;

        snake.moveTimer += deltaTime;
        float moveInterval = 1.f / snake.speed;

        if (snake.moveTimer >= moveInterval)
        {
            snake.moveTimer -= moveInterval;
            snake.direction = snake.pendingDirection;

            Position2D newHead = snake.segments[0].position;
            switch (snake.direction)
            {
            case Direction::Right: newHead.x += SEGMENT_SIZE; break;
            case Direction::Left:  newHead.x -= SEGMENT_SIZE; break;
            case Direction::Up:    newHead.y -= SEGMENT_SIZE; break;
            case Direction::Down:  newHead.y += SEGMENT_SIZE; break;
            }

            for (int i = (int)snake.segments.size() - 1; i > 0; --i)
                snake.segments[i].position = snake.segments[i - 1].position;

            snake.segments[0].position = newHead;

            ApplyTexture(snake.segments[0].sprite, GetHeadTexture(snake, snake.direction));

            for (int i = 1; i < (int)snake.segments.size() - 1; ++i)
                ApplyTexture(snake.segments[i].sprite, GetBodyTexture(snake, i));

            if (snake.segments.size() > 1)
            {
                int last = (int)snake.segments.size() - 1;
                Direction tailDir = GetDirectionFromTo(
                    snake.segments[last - 1].position,
                    snake.segments[last].position);
                ApplyTexture(snake.segments[last].sprite,
                    GetTailTexture(snake, tailDir));
            }

            for (auto& seg : snake.segments)
                ApplySegmentVisual(seg);
        }
    }

    void GrowSnake(Snake& snake)
    {
        SnakeSegment newSegment = snake.segments.back();
        snake.segments.insert(snake.segments.end() - 1, newSegment);
    }

    void DrawSnake(Snake& snake, sf::RenderWindow& window)
    {
        for (auto& seg : snake.segments)
            window.draw(seg.sprite);
    }

    bool IsSnakeCollideWithSelf(const Snake& snake)
    {
        Position2D headPos = snake.segments[0].position;
        for (int i = 1; i < (int)snake.segments.size(); ++i)
            if (IsSameCell(headPos, snake.segments[i].position, SEGMENT_SIZE))
                return true;
        return false;
    }

    bool IsSnakeCollideWithWall(const Snake& snake, Position2D wallPos)
    {
        return IsSameCell(snake.segments[0].position, wallPos, SEGMENT_SIZE);
    }
}