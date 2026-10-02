#pragma once
#include <string>
#include <map>
#include <vector>

namespace SnakeGame
{
    struct Record
    {
        std::string name;
        int score = 0;
    };

    std::map<int, Record> CreateInitialLeaderboard();
    void UpdateLeaderboard(std::map<int, Record>& lb, const std::string& name, int score);
    std::vector<Record> GetSortedLeaderboard(const std::map<int, Record>& lb);
    std::string GetLeaderboardString(const std::map<int, Record>& lb,
        const std::string& playerName, int topCount);
}