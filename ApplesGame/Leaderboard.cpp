#include "Leaderboard.h"
#include "Constants.h"
#include <cstdlib>

namespace SnakeGame
{
    std::map<int, Record> CreateInitialLeaderboard()
    {
        std::map<int, Record> lb;
        std::vector<std::string> names = {
            "Alice", "Bob", "Carol", "Dave", "Eve",
            "Frank", "Grace", "Henry", "Ivy", "Jack"
        };

        for (int i = 0; i < 10; ++i)
        {
            int score = 20 + (rand() % 80);
            lb[i + 1] = { names[i], score };
        }
        return lb;
    }

    void UpdateLeaderboard(std::map<int, Record>& lb, const std::string& name, int score)
    {
        int key = -1;
        for (auto& pair : lb)
        {
            if (pair.second.name == name) { key = pair.first; break; }
        }

        if (key != -1)
        {
            lb[key].score = score;
        }
        else
        {
            int newKey = 1;
            for (auto& pair : lb)
                if (pair.first >= newKey) newKey = pair.first + 1;
            lb[newKey] = { name, score };
        }
    }

    std::vector<Record> GetSortedLeaderboard(const std::map<int, Record>& lb)
    {
        std::vector<Record> result;
        for (auto& pair : lb) result.push_back(pair.second);

        int n = (int)result.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (result[j].score < result[j + 1].score)
                {
                    Record temp = result[j];
                    result[j] = result[j + 1];
                    result[j + 1] = temp;
                }
            }
        }
        return result;
    }

    std::string GetLeaderboardString(const std::map<int, Record>& lb,
        const std::string& playerName, int topCount)
    {
        std::string result = "===== LEADERBOARD =====\n";
        std::vector<Record> sorted = GetSortedLeaderboard(lb);

        int count = 0;
        for (const Record& r : sorted)
        {
            if (count >= topCount) break;
            ++count;
            std::string marker = (r.name == playerName) ? " *" : "";
            result += std::to_string(count) + ". " + r.name +
                " ...... " + std::to_string(r.score) + marker + "\n";
        }
        result += "=======================";
        return result;
    }
}