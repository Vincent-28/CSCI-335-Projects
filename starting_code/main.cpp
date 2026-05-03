#include "Leaderboard.hpp"
#include "PlayerStream.hpp"

#include <iostream>
#include <vector>

int main() {
    std::vector<Player> players = {
        Player("A", 10),
        Player("B", 40),
        Player("C", 20),
        Player("D", 50),
        Player("E", 30),
        Player("F", 60),
        Player("G", 70),
        Player("H", 80),
        Player("I", 90),
        Player("J", 100)
    };

    std::vector<Player> players_copy = players;

    RankingResult heap_result = Offline::heapRank(players);
    RankingResult quick_result = Offline::quickSelectRank(players_copy);

    VectorPlayerStream stream({
        Player("A", 10),
        Player("B", 40),
        Player("C", 20),
        Player("D", 50),
        Player("E", 30),
        Player("F", 60),
        Player("G", 70),
        Player("H", 80),
        Player("I", 90),
        Player("J", 100)
    });

    RankingResult online_result = Online::rankIncoming(stream, 3);

    std::cout << "heapRank top_:" << std::endl;
    for (const Player& p : heap_result.top_) {
        std::cout << p.name_ << " " << p.level_ << std::endl;
    }

    std::cout << "\nquickSelectRank top_:" << std::endl;
    for (const Player& p : quick_result.top_) {
        std::cout << p.name_ << " " << p.level_ << std::endl;
    }

    std::cout << "\nrankIncoming top_:" << std::endl;
    for (const Player& p : online_result.top_) {
        std::cout << p.name_ << " " << p.level_ << std::endl;
    }

    std::cout << "\nrankIncoming cutoffs_:" << std::endl;
    for (const auto& entry : online_result.cutoffs_) {
        std::cout << entry.first << " -> " << entry.second << std::endl;
    }

    return 0;
}