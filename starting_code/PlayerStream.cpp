#include "PlayerStream.hpp"

VectorPlayerStream::VectorPlayerStream(const std::vector<Player>& players) : players_{players}, next_index_{0} {}
//define constructor outside of class

Player VectorPlayerStream::nextPlayer() { //this will return the next player
    if (next_index_ >= players_.size()) { //check whether or no the stream is exhausted 
        //if next_index_ has reached the size of the vector, there are no more players left so we throw
        throw std::runtime_error("No more players in stream");
    }
    Player next_player = players_[next_index_]; //this grabs the current player and copies that player into a local variable called next_player
    ++next_index_; //move forward to the next unread player
    return next_player; //return the player that we just got
}

size_t VectorPlayerStream::remaining() const { //computes how many are left
    return players_.size() - next_index_;
}
