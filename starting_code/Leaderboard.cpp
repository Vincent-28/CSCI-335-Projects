#include "Leaderboard.hpp"
#include <algorithm>
#include <chrono>
#include <functional>
#include <utility>

/**
 * @brief Constructor for RankingResult with top players, cutoffs, and elapsed time.
 *
 * @param top Vector of top-ranked Player objects, in sorted order.
 * @param cutoffs Map of player count thresholds to minimum level cutoffs.
 *   NOTE: This is only ever non-empty for Online::rankIncoming().
 *         This parameter & the corresponding member should be empty
 *         for all Offline algorithms.
 * @param elapsed Time taken to calculate the ranking, in seconds.
 */
RankingResult::RankingResult(const std::vector<Player>& top, const std::unordered_map<size_t, size_t>& cutoffs, double elapsed)
    : top_ { top }
    , cutoffs_ { cutoffs }
    , elapsed_ { elapsed }
{
}

namespace Offline {
    RankingResult heapRank(std::vector<Player>& players) { 
        /*
        We are returning a RankingResult object in order to modify the original 
        vector to avoice wasting memory
        */
        using Clock = std::chrono::high_resolution_clock; //use a shorter nickname used to measure the time 
        const auto start = Clock::now(); //get the starting time
        const size_t n = players.size(); //store the number of players in the vector (we need the total number of players to calculate the top 10%)
        const size_t count_top = n / 10; //this computes how many players are actually in the top 10% (rounding down)

        if (count_top == 0) { //check if the top 10% contains 0 players (size is 0-9)
            const auto end = Clock::now(); //record the ending time for this case. We are gonna need the ending time now since we're about to return
            const double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
            /*
            this computes how much time has passed between start and end
            end - start gives the time difference and then the chrono converts 
            the difference into milliseconds and .count() takes the 
            numerical value
            */
            return RankingResult({}, {}, elapsed_ms); //returns a valid empty answer when there are not enough players to have a top 10%
        }
        std::make_heap(players.begin(), players.end()); 
        /*
        this turns the whole players vector into a max heap
        we are using heapsort and will remove the root
        */
        size_t heap_size = n; //this stores the current size of the heap
        for (size_t i = 0; i < count_top; ++i) { //this loop will go through the top 10% of players times
            std::pop_heap(players.begin(), players.begin() + heap_size);
            /*
            this will take the largest player in the heap and move that player to the end
            of the heap range and then fizes the remaining part so that it still remains a heap
            */
            --heap_size; 
            /*
            decrement the size because after pop_heap, the largest remaining player 
            is now at the end of the current heap range and is now accounted for
            */
        }
        std::vector<Player> top_players(players.end() - static_cast<long>(count_top), players.end());
        /*
        this range is exactly the last count_top players and we have this so 
        that we built the top part of the returned RankingResult
        */
        const auto end = Clock::now(); //record the ending time after all the heap work is finished
        double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
        /*
        this line computes the total elapsed time in milliseconds just like before
        */
        return RankingResult(top_players, {}, elapsed_ms); //return final answer
    }

    int partitionPlayers(std::vector<Player>& players, int left, int right) {
        Player pivot = players[right]; //this chooses the pivot. The pivo will be the player at the far right end of the current range
        int i = left; //this is the index that marks the position where the next smaller than pivot player should go
        for (int j = left; j < right; ++j) { //we scan through the range from left to right with this loop
            if (players[j] < pivot) { //check whether or not the current player is smaller than the pivot
                std::swap(players[i], players[j]); //if the player[j] is smaller than the pivot then it should go into the left partition
                ++i; //increment
            }
        }
        std::swap(players[i], players[right]); //swap the pivot into it's final correct position
        return i; //return the pivot's final index
    }

    void quickSortRange(std::vector<Player>& players, int left, int right) {
        if (left >= right) { //this is the base case if the range has 1 or 0 elements.
            return; //1 element meaning already sorted and zero elements means that there is nothing to sort.
        }
        const int pivot_index = partitionPlayers(players, left, right); 
        /*
        call partition function hat picks the pivot and rearranges the subarray from left to right
        after this line, everything to the left of the pivot_index is smaller than the pivot and the pivot
        belongs exactly where it is in sorted order (quicksort)
        */
        quickSortRange(players, left, pivot_index - 1); //recursively sort the left side of the pivot
        quickSortRange(players, pivot_index + 1, right); //recursively sort the right side of the pivot
    }

    void hybridSelectSort(std::vector<Player>& players, int left, int right, int target) { //mix of quickselect and quicksort
        //we want the top 10% of players to end up at the end of the vector
        if (left>= right) { //checks base case when here is 0 or 1 element
            return;
        }
        const int pivot_index = partitionPlayers(players, left, right);
        /*
        partitions the subarray around some pivot
        after partitioning, everything smaller than the pivot is on the left and the pivot is in the correct position
        */
        if (pivot_index < target) { //if the pivot landed too far left
            //this means the pivot is not in the top 10% and that. We do not care about the left side anymore
            hybridSelectSort(players, pivot_index + 1, right, target); //recurse only the right side
        }
        else if (pivot_index == target) { //pivot landed exactly where the top 10% should begin
            quickSortRange(players, pivot_index, right); //sort in ascending order
        }
        else { //pivot lands to the right of the target
            quickSortRange(players, pivot_index, right); //sort the part from pivot_index to right since we know that part is for sure in the group
            hybridSelectSort(players, left, pivot_index - 1, target); //recurse the left side because we know there is still some on the left
        }
    }

    RankingResult quickSelectRank(std::vector<Player>& players) {
        using Clock = std::chrono::high_resolution_clock; //shorter nickname
        const auto start = Clock::now(); //records the starting time
        const int n = static_cast<int>(players.size()); //stores the number of players in the vector
        const int top_count = n / 10; //computes how many players belong in the top 10%
        if (top_count == 0) { //checks whether or not the top 10% contains zero players
            const auto end = Clock::now(); //records the ending time for the special case where the top_count is 0
            const double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count(); //computes how much time passed between start and end
            return RankingResult({}, {}, elapsed_ms); //returns the empty result
        }
        const int cutoff_index = n - top_count; //calculates the first index where the top 10% should begin
        hybridSelectSort(players, 0, n - 1, cutoff_index); //calls helper function hybridSelectSort on the whole vector and used quickselect to figure out where the top region starts
        //and uses quicksort on the part that is definitely in the top 10%
        std::vector<Player> top_players(players.begin() + cutoff_index, players.end()); //creates a new vector named top_players and copies the range from the original players vector
        //basically it copies the last top_count players and this copied vector becomes the final leaderboard result
        const auto end = Clock::now(); //records the ending time now that all the sorting work is done
        const double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count(); //computes the total running time in milliseconds
        return RankingResult(top_players, {}, elapsed_ms); //returns the final answer
    }
}

namespace Online {
    void replaceMin(PlayerIt first, PlayerIt last, Player& target) {
        if (first == last) { //if the heap range is empty, then there would be nothing to replace
            return;
        }
        Player temp = std::move(target); //move the target into a local variable called temp
        size_t hole = 0; //hole is basically the current place where temp might eventually go
        const size_t heap_size = static_cast<size_t>(last - first); //last - first tells us how many elements are in the heap range
        while (2 * hole + 1 < heap_size) { //this checks whether or not the current hole has a left child
            size_t child = 2 * hole + 1; //assume the left child is the one to compare
            if (child + 1 < heap_size && *(first + child + 1) < *(first + child)) { 
                ++child;
            } 
            /*
            this for loop checks whether or not a right child exists and the right child is smaller than the left child
            in that case, then move the child to the right child index
            */
            if (*(first + child) < temp) { //if the smaller child is smaller than temp, then temp is too large to stay here
                *(first + hole) = std::move(*(first + child));
                hole = child;
                /*
                this moves the smaller child into the hole
                and moves the hole down to where that child used to be
                so basically the hole keeps traveling downward until the right place for temp is found
                */
            } 
            else {
                break; //if the smaller child is not smaller than temp, then temp is already in a valid place
            }
        }
        *(first + hole) = std::move(temp); //after the loop ends, we finally place temp into the last hole position
    }

    RankingResult rankIncoming(PlayerStream& stream, const size_t& reporting_interval) {
        using Clock = std::chrono::high_resolution_clock; //create nickname for clock
        std::vector<Player> heap; //create a vector that will store the current best players seen so far (min-heap)
        std::unordered_map<size_t, size_t> cutoffs; //creates a hashmap 
        double elapsed_ms = 0.0; //this keeps track of the total processing time in milliseconds
        size_t processed = 0; //this keeps track of the count on how many players have been read from the stream so far
        if (reporting_interval == 0) { //checks for when reporting_interval is 0, so can't really build a leaderboard normally
            while (stream.remaining() > 0) { //as long as the stream still has players left, keep looping
                stream.nextPlayer(); //read and discard the next player
            }
            return RankingResult({}, {}, 0.0); //return an empty result. there are no top players, the elapsed time is 0 
        }
        heap.reserve(reporting_interval); //this tells the vector to reserve enough memory for reporting_interval players
        //since we know the heap will never need to hold more than that many players
        while (stream.remaining() > 0) { //keeps running until the stream is empty
            Player incoming = stream.nextPlayer(); //get the next player from the stream and store it in a variable
            ++processed; //increment the number of processed players
            const auto start = Clock::now(); //start timing the processing work for this one player
            if (heap.size() < reporting_interval) { //check whether or not the heap is still not full yet
                heap.push_back(std::move(incoming)); //add the new player to the end of the vector.
                //right now, the vector has one new element but it is not automatically a valid heap yet
                std::push_heap(heap.begin(), heap.end(), std::greater<Player>()); //this restores the heap property after adding the new player to the heap
            }
            else if (incoming > heap.front()) { //check the case in which the heap is already full
                //heap.front() is the smallest player currently in the leaderboard
                replaceMin(heap.begin(), heap.end(), incoming); //replace the root of the min-heap with the new player and percolate down
                //this removes the current minimum and inserts the stronger new player
            }
            if (!heap.empty() && processed % reporting_interval == 0) { //this checks whether or not you should record a cutoff now
                cutoffs[processed] = heap.front().level_; //store the current cutoff in the map
                //the key is the number of processed players and the value is the root level of the minheap
            }
            const auto end = Clock::now(); //stop timing the processing work for this player
            elapsed_ms += std::chrono::duration<double, std::milli>(end - start).count(); //computer how many milliseconds passed from start to end and add that amount o elapsed_ms
        }
        if (!heap.empty() && processed > 0 && processed % reporting_interval != 0) { //this checks whether or not you still need to record one final cutoff after the stream ends
            cutoffs[processed] = heap.front().level_; //store the final cutoff
        }
        const auto sort_start = Clock::now(); //start timing the final sorting step
        std::sort(heap.begin(), heap.end()); //sort the heap vector in ascending order
        const auto sort_end = Clock::now(); //stop timing the final sorting step
        elapsed_ms += std::chrono::duration<double, std::milli>(sort_end - sort_start).count(); //add the sorting time to the total elapsed time
        return RankingResult(heap, cutoffs, elapsed_ms); //return the final result
    }
}