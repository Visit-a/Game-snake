#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <vector>
#include <string>
#include "Constants.h"
#include "Math.h"
#include "Snake.h"
#include "Apple.h"
#include "Wall.h"
#include "Menu.h"
#include "Leaderboard.h"

namespace SnakeGame
{
    enum class Difficulty
    {
        Easy = 0,
        Medium,
        Hard
    };

    enum class GameState
    {
        MainMenu,
        DifficultySelect,
        Settings,
        LeaderboardView,
        Playing,
        DeathScreen
    };

    enum class Language
    {
        English = 0,
        Russian
    };

    struct Game
    {
        Snake snake;
        Apple apple;
        std::vector<Wall> walls;

        sf::Texture appleTexture;
        sf::Texture wallTexture;
        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;

        GameState state = GameState::MainMenu;
        Difficulty difficulty = Difficulty::Easy;
        Language language = Language::English;

        int score = 0;
        int applesEaten = 0;

        float startDelayTimer = 0.f;
        float pauseTimer = 0.f;
        bool isDelayActive = false;

        std::map<int, Record> leaderboard;
        std::string playerName = "Player";

        Menu mainMenu;
        Menu difficultyMenu;
        Menu settingsMenu;

        bool soundEnabled = true;

        sf::Font font;
        sf::Text hudText;
        sf::Text deathText;
        sf::Text pauseText;
        sf::Text leaderboardText;

        sf::SoundBuffer eatSoundBuffer;
        sf::SoundBuffer deathSoundBuffer;
        sf::SoundBuffer clickSoundBuffer;
        sf::Sound eatSound;
        sf::Sound deathSound;
        sf::Sound clickSound;

        sf::RectangleShape playField;

        bool enterPressed = false;
        bool wPressed = false;
        bool sPressed = false;
    };

    void InitGame(Game& game);
    void UpdateGame(Game& game, float deltaTime);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void DeinitializeGame(Game& game);

    void StartNewSession(Game& game);
    void EndSession(Game& game);
    void ApplyDifficulty(Game& game, Difficulty d);

    void ApplyLanguage(Game& game);
}