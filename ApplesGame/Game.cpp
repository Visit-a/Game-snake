#include "Game.h"
#include <iostream>
#include <cstdlib>

namespace SnakeGame
{
    static void CenterTextOrigin(sf::Text& text)
    {
        sf::FloatRect r = text.getLocalBounds();
        text.setOrigin(r.left + r.width / 2.f, r.top + r.height / 2.f);
    }

    // ============ Языки ============

    void ApplyLanguage(Game& game)
    {
        if (game.language == Language::Russian)
        {
            game.mainMenu.SetContent("ЗМЕЙКА",
                { "Начать игру", "Сложность", "Рекорды", "Настройки", "Выход" });
            game.difficultyMenu.SetContent("СЛОЖНОСТЬ",
                { "Простой", "Средний", "Сложный", "Назад" });
            game.settingsMenu.SetContent("НАСТРОЙКИ",
                { game.soundEnabled ? "Звук: ВКЛ" : "Звук: ВЫКЛ",
                  "Язык: РУ",
                  "Назад" });
        }
        else
        {
            game.mainMenu.SetContent("SNAKE",
                { "Start Game", "Difficulty", "Leaderboard", "Settings", "Exit" });
            game.difficultyMenu.SetContent("DIFFICULTY",
                { "Easy", "Medium", "Hard", "Back" });
            game.settingsMenu.SetContent("SETTINGS",
                { game.soundEnabled ? "Sound: ON" : "Sound: OFF",
                  "Language: EN",
                  "Back" });
        }
    }

    // ============ Сложность ============

    void ApplyDifficulty(Game& game, Difficulty d)
    {
        game.difficulty = d;
        switch (d)
        {
        case Difficulty::Easy:
            game.snake.speed = EASY_SPEED;
            game.snake.scorePerApple = EASY_SCORE;
            break;
        case Difficulty::Medium:
            game.snake.speed = MEDIUM_SPEED;
            game.snake.scorePerApple = MEDIUM_SCORE;
            break;
        case Difficulty::Hard:
            game.snake.speed = HARD_SPEED;
            game.snake.scorePerApple = HARD_SCORE;
            break;
        }
    }

    void StartNewSession(Game& game)
    {
        InitSnake(game.snake);
        ApplyDifficulty(game, game.difficulty);

        std::vector<Position2D> occupied;
        for (auto& seg : game.snake.segments) occupied.push_back(seg.position);

        std::vector<Position2D> wallCells;
        for (auto& w : game.walls) wallCells.push_back(w.position);

        RespawnApple(game.apple, occupied, wallCells);

        game.score = 0;
        game.applesEaten = 0;
        game.startDelayTimer = START_DELAY;
        game.isDelayActive = true;
        game.state = GameState::Playing;
    }

    void EndSession(Game& game)
    {
        game.state = GameState::DeathScreen;
        game.pauseTimer = 0.f;
        UpdateLeaderboard(game.leaderboard, game.playerName, game.score);
        if (game.soundEnabled) game.deathSound.play();
    }

    void InitGame(Game& game)
    {
        if (!game.appleTexture.loadFromFile(RESOURCES_PATH + "apple.png"))
            std::cout << "Warning: apple.png not loaded\n";

        if (!game.wallTexture.loadFromFile(RESOURCES_PATH + "brick_wall.png"))
            std::cout << "Warning: brick_wall.png not loaded (wall)\n";

        if (!game.backgroundTexture.loadFromFile(RESOURCES_PATH + "brick_wall.png"))
            std::cout << "Warning: brick_wall.png not loaded (background)\n";
        else
        {
            game.backgroundSprite.setTexture(game.backgroundTexture);
            sf::Vector2u texSize = game.backgroundTexture.getSize();
            if (texSize.x > 0 && texSize.y > 0)
            {
                game.backgroundSprite.setScale(
                    SCREEN_WIDTH / (float)texSize.x,
                    SCREEN_HEIGHT / (float)texSize.y);
            }
            game.backgroundSprite.setPosition(0.f, 0.f);
        }

        InitApple(game.apple, game.appleTexture);
        InitWalls(game.walls, game.wallTexture);

        if (!game.font.loadFromFile(RESOURCES_PATH + "arial.ttf"))
            std::cout << "Warning: font not loaded\n";
        else
            std::cout << "Font loaded OK\n";

        game.hudText.setFont(game.font);
        game.hudText.setCharacterSize(24);
        game.hudText.setFillColor(sf::Color::White);
        game.hudText.setPosition(10.f, 10.f);

        game.deathText.setFont(game.font);
        game.deathText.setCharacterSize(24);
        game.deathText.setFillColor(sf::Color::White);

        game.pauseText.setFont(game.font);
        game.pauseText.setCharacterSize(40);
        game.pauseText.setFillColor(sf::Color::Yellow);

        game.leaderboardText.setFont(game.font);
        game.leaderboardText.setCharacterSize(24);
        game.leaderboardText.setFillColor(sf::Color::White);
        game.leaderboardText.setPosition(150.f, 150.f);

        game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav");
        game.eatSound.setBuffer(game.eatSoundBuffer);
        game.deathSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav");
        game.deathSound.setBuffer(game.deathSoundBuffer);
        game.clickSoundBuffer.loadFromFile(RESOURCES_PATH + "Click.wav");
        game.clickSound.setBuffer(game.clickSoundBuffer);

        game.playField.setSize(sf::Vector2f(SCREEN_WIDTH - 2 * SEGMENT_SIZE,
            SCREEN_HEIGHT - 2 * SEGMENT_SIZE));
        game.playField.setFillColor(sf::Color(20, 40, 20));
        game.playField.setPosition(SEGMENT_SIZE, SEGMENT_SIZE);

        game.leaderboard = CreateInitialLeaderboard();
        std::cout << "=== SNAKE GAME ===\n";
        game.playerName = "Player";
        UpdateLeaderboard(game.leaderboard, game.playerName, 0);

        // Начальный язык — русский. Инициализируем меню заглушками,
        // затем ApplyLanguage перезапишет все строки.
        game.mainMenu.Init("", { "", "", "", "", "" });
        game.difficultyMenu.Init("", { "", "", "", "" });
        game.settingsMenu.Init("", { "", "", "" });

        ApplyLanguage(game);

        game.state = GameState::MainMenu;
    }

    // ============ Меню ============

    static void HandleMenuKeys(Game& game, bool& up, bool& down, bool& enter)
    {
        bool w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        bool s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool e = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);

        up = w && !game.wPressed;
        down = s && !game.sPressed;
        enter = e && !game.enterPressed;

        game.wPressed = w;
        game.sPressed = s;
        game.enterPressed = e;
    }

    static void UpdateMainMenu(Game& game)
    {
        bool up, down, enter;
        HandleMenuKeys(game, up, down, enter);

        if (up) { game.mainMenu.MoveUp();   if (game.soundEnabled) game.clickSound.play(); }
        if (down) { game.mainMenu.MoveDown(); if (game.soundEnabled) game.clickSound.play(); }

        if (enter)
        {
            if (game.soundEnabled) game.clickSound.play();
            switch (game.mainMenu.selectedIndex)
            {
            case 0: StartNewSession(game); break;
            case 1:
                game.state = GameState::DifficultySelect;
                game.difficultyMenu.selectedIndex = (int)game.difficulty;
                for (size_t i = 0; i < game.difficultyMenu.items.size(); ++i)
                    game.difficultyMenu.items[i].isSelected = ((int)i == (int)game.difficulty);
                break;
            case 2: game.state = GameState::LeaderboardView; break;
            case 3:
                game.state = GameState::Settings;
                game.settingsMenu.selectedIndex = 0;
                for (size_t i = 0; i < game.settingsMenu.items.size(); ++i)
                    game.settingsMenu.items[i].isSelected = (i == 0);
                break;
            case 4: std::exit(0); break;
            }
        }
    }

    static void UpdateDifficultySelect(Game& game)
    {
        bool up, down, enter;
        HandleMenuKeys(game, up, down, enter);

        if (up) { game.difficultyMenu.MoveUp();   if (game.soundEnabled) game.clickSound.play(); }
        if (down) { game.difficultyMenu.MoveDown(); if (game.soundEnabled) game.clickSound.play(); }

        if (enter)
        {
            if (game.soundEnabled) game.clickSound.play();
            int idx = game.difficultyMenu.selectedIndex;
            if (idx >= 0 && idx <= 2) ApplyDifficulty(game, (Difficulty)idx);
            game.state = GameState::MainMenu;
            game.mainMenu.selectedIndex = 1;
            for (size_t i = 0; i < game.mainMenu.items.size(); ++i)
                game.mainMenu.items[i].isSelected = ((int)i == 1);
        }
    }

    static void UpdateSettings(Game& game)
    {
        bool up, down, enter;
        HandleMenuKeys(game, up, down, enter);

        if (up) { game.settingsMenu.MoveUp();   if (game.soundEnabled) game.clickSound.play(); }
        if (down) { game.settingsMenu.MoveDown(); if (game.soundEnabled) game.clickSound.play(); }

        if (enter)
        {
            if (game.soundEnabled) game.clickSound.play();
            int idx = game.settingsMenu.selectedIndex;

            if (idx == 0)
            {
                // Переключение звука
                game.soundEnabled = !game.soundEnabled;
                ApplyLanguage(game);
            }
            else if (idx == 1)
            {
                // Переключение языка
                if (game.language == Language::Russian)
                    game.language = Language::English;
                else
                    game.language = Language::Russian;
                ApplyLanguage(game);
            }
            else
            {
                // Назад
                game.state = GameState::MainMenu;
                game.mainMenu.selectedIndex = 3;
                for (size_t i = 0; i < game.mainMenu.items.size(); ++i)
                    game.mainMenu.items[i].isSelected = ((int)i == 3);
            }
        }
    }

    static void UpdateLeaderboardScreen(Game& game)
    {
        bool up, down, enter;
        HandleMenuKeys(game, up, down, enter);
        if (enter || up || down)
        {
            game.state = GameState::MainMenu;
            game.mainMenu.selectedIndex = 2;
            for (size_t i = 0; i < game.mainMenu.items.size(); ++i)
                game.mainMenu.items[i].isSelected = ((int)i == 2);
        }
    }

    // ============ Игра ============

    static void UpdatePlaying(Game& game, float deltaTime)
    {
        if (game.isDelayActive)
        {
            game.startDelayTimer -= deltaTime;
            if (game.startDelayTimer <= 0.f)
            {
                game.isDelayActive = false;
                game.snake.moveTimer = 0.f;
            }
            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            SetSnakeDirection(game.snake, Direction::Up);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            SetSnakeDirection(game.snake, Direction::Down);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            SetSnakeDirection(game.snake, Direction::Left);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            SetSnakeDirection(game.snake, Direction::Right);

        bool wasOnApple = IsSameCell(game.snake.segments[0].position,
            game.apple.position, SEGMENT_SIZE);

        UpdateSnake(game.snake, deltaTime);

        bool isOnApple = IsSameCell(game.snake.segments[0].position,
            game.apple.position, SEGMENT_SIZE);

        if (isOnApple && !wasOnApple)
        {
            GrowSnake(game.snake);
            game.score += game.snake.scorePerApple;
            ++game.applesEaten;
            if (game.soundEnabled) game.eatSound.play();

            std::vector<Position2D> occupied;
            for (auto& seg : game.snake.segments) occupied.push_back(seg.position);

            std::vector<Position2D> wallCells;
            for (auto& w : game.walls) wallCells.push_back(w.position);

            RespawnApple(game.apple, occupied, wallCells);
        }

        if (IsSnakeCollideWithSelf(game.snake))
        {
            EndSession(game);
            return;
        }

        for (auto& wall : game.walls)
        {
            if (IsSnakeCollideWithWall(game.snake, wall.position))
            {
                EndSession(game);
                return;
            }
        }
    }

    static void UpdateDeathScreen(Game& game, float deltaTime)
    {
        game.pauseTimer += deltaTime;
        if (game.pauseTimer >= PAUSE_LENGTH)
        {
            game.state = GameState::MainMenu;
            game.mainMenu.selectedIndex = 0;
            for (size_t i = 0; i < game.mainMenu.items.size(); ++i)
                game.mainMenu.items[i].isSelected = ((int)i == 0);
        }
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        switch (game.state)
        {
        case GameState::MainMenu:         UpdateMainMenu(game); break;
        case GameState::DifficultySelect: UpdateDifficultySelect(game); break;
        case GameState::Settings:         UpdateSettings(game); break;
        case GameState::LeaderboardView:  UpdateLeaderboardScreen(game); break;
        case GameState::Playing:          UpdatePlaying(game, deltaTime); break;
        case GameState::DeathScreen:      UpdateDeathScreen(game, deltaTime); break;
        }
    }

    // ============ Отрисовка ============

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        switch (game.state)
        {
        case GameState::MainMenu:
        case GameState::DifficultySelect:
        case GameState::Settings:
        case GameState::LeaderboardView:
        {
            window.draw(game.backgroundSprite);

            switch (game.state)
            {
            case GameState::MainMenu:         game.mainMenu.Draw(window); break;
            case GameState::DifficultySelect: game.difficultyMenu.Draw(window); break;
            case GameState::Settings:         game.settingsMenu.Draw(window); break;
            case GameState::LeaderboardView:
                game.leaderboardText.setString(GetLeaderboardString(game.leaderboard,
                    game.playerName,
                    LEADERBOARD_MENU_SIZE));
                window.draw(game.leaderboardText);
                break;
            default: break;
            }
            break;
        }

        case GameState::Playing:
        case GameState::DeathScreen:
        {
            window.draw(game.playField);

            for (auto& wall : game.walls)
                window.draw(wall.sprite);

            window.draw(game.apple.sprite);

            DrawSnake(game.snake, window);

            if (game.language == Language::Russian)
            {
                game.hudText.setString("Очки: " + std::to_string(game.score) +
                    "   Яблок: " + std::to_string(game.applesEaten));
            }
            else
            {
                game.hudText.setString("Score: " + std::to_string(game.score) +
                    "   Apples: " + std::to_string(game.applesEaten));
            }
            window.draw(game.hudText);

            if (game.isDelayActive)
            {
                std::string delayStr;
                if (game.language == Language::Russian)
                    delayStr = "Приготовьтесь... " +
                    std::to_string((int)(game.startDelayTimer) + 1);
                else
                    delayStr = "Get ready... " +
                    std::to_string((int)(game.startDelayTimer) + 1);

                game.pauseText.setString(delayStr);
                CenterTextOrigin(game.pauseText);
                game.pauseText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
                window.draw(game.pauseText);
            }

            if (game.state == GameState::DeathScreen)
            {
                std::string s;
                if (game.language == Language::Russian)
                {
                    s = "ИГРА ОКОНЧЕНА\n\n";
                    s += "Ваш счёт: " + std::to_string(game.score) + "\n\n";
                }
                else
                {
                    s = "GAME OVER\n\n";
                    s += "Score: " + std::to_string(game.score) + "\n\n";
                }
                s += GetLeaderboardString(game.leaderboard, game.playerName,
                    LEADERBOARD_POPUP_SIZE);
                game.deathText.setString(s);
                game.deathText.setPosition(200.f, 100.f);
                window.draw(game.deathText);
            }
            break;
        }
        }
    }

    void DeinitializeGame(Game& game)
    {
        std::cout << "Game deinitialized\n";
    }
}