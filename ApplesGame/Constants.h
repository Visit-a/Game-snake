#pragma once
#include <string>

namespace SnakeGame
{
    const std::string RESOURCES_PATH = "Graphics/";

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const float SEGMENT_SIZE = 20.f;
    const int INITIAL_SNAKE_LENGTH = 3;

    const float EASY_SPEED = 5.f;
    const float MEDIUM_SPEED = 7.f;
    const float HARD_SPEED = 10.f;

    const int EASY_SCORE = 2;
    const int MEDIUM_SCORE = 6;
    const int HARD_SCORE = 10;

    const float START_DELAY = 3.f;
    const float PAUSE_LENGTH = 3.f;

    const int LEADERBOARD_POPUP_SIZE = 5;
    const int LEADERBOARD_MENU_SIZE = 10;
}