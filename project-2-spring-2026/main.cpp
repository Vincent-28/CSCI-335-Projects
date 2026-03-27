#include "Compare.hpp"
#include "HashInventory.hpp"
#include "Inventory.hpp"
#include "Item.hpp"
#include "ItemAVL.hpp"
#include "TreeInventory.hpp"

#include <vector>
#include <chrono>
#include <iostream>
#include "ItemGenerator.hpp"

#include <list>

template <typename Comparator, typename Container>
double timeContains(size_t n)
{
    Inventory<Comparator, Container> inv; //create the inventory
    ItemGenerator gen(42);
    for (size_t i = 0; i < n; i++) { //insert n random items into the inventory
        inv.pickup(gen.randomItem());
    }
    std::vector<std::string> contained; //store names that are in the inventory
    std::vector<std::string> missing; //store names not contained in the inventory
    for (int i = 0; i < 100; i++) { //generate 100 names that exist in the inventory
        contained.push_back(gen.randomUsedName());
    }
    for (int i = 0; i < 100; i++) { //generate 100 names thata ren't in the inventory
        missing.push_back(gen.randomItem().name_);
    }
    double total = 0; //this will track the total time accumulated
    for (const std::string& name : contained) { //this is a measure of contains() for the items that exists
        auto start = std::chrono::high_resolution_clock::now(); //start the timer
        inv.contains(name); //call the function
        auto end = std::chrono::high_resolution_clock::now(); //stop the timer
        total += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); //add the elapsed time
    }
    for (const std::string& name : missing) { //measure of contains() that do not exist
        auto start = std::chrono::high_resolution_clock::now();
        inv.contains(name);
        auto end = std::chrono::high_resolution_clock::now();

        total += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    return total / 200.0; //return the average time after 200 calls
}

template <typename Comparator, typename Container>
double timeQueryName(size_t n)
{
    Inventory<Comparator, Container> inv;
    ItemGenerator gen(42);
    for (size_t i = 0; i < n; i++) { //insert n random items
        inv.pickup(gen.randomItem());
    }
    double total = 0;
    for (int i = 0; i < 10; i++) { //run it 10 times
        std::string a = gen.randomUsedName();
        std::string b = gen.randomUsedName(); //get 2 random names that exist
        Item start(a, 0, NONE);
        Item end(b, 0, NONE); //creat start and end where only the name matters
        if (Comparator::lessThan(end, start)) {
            std::swap(start, end); //make sure that start is less than or equal to end
        }
        auto t1 = std::chrono::high_resolution_clock::now(); //time it
        inv.query(start, end);
        auto t2 = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }
    return total / 10.0; //get the average over 10 runs
}

template <typename Comparator, typename Container>
double timeQueryWeight(size_t n)
{
    Inventory<Comparator, Container> inv;
    ItemGenerator gen(42);
    for (size_t i = 0; i < n; i++) { //insert n of random items
        inv.pickup(gen.randomItem());
    }
    double total = 0;
    for (int i = 0; i < 10; i++) { //run it 10 times
        float w = gen.randomFloat(ItemGenerator::MIN_WEIGHT, ItemGenerator::MAX_WEIGHT);
        Item start("a", w, NONE); //create a start and end items based on the weight range
        Item end("b", w + 0.1f, NONE);
        auto t1 = std::chrono::high_resolution_clock::now(); //time it
        inv.query(start, end);
        auto t2 = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }
    return total / 10.0; //return average over 10 runs
}

int main()
{
    std::vector<int> sizes = {1000, 2000, 4000, 8000}; //these are the different input sizes in which we are testing
    std::cout << "Contains\n";
    for (int n : sizes) {
        std::cout << "n = " << n << "\n";
        std::cout << "Vector: " << timeContains<CompareItemName, std::vector<Item>>(n) << "\n";
        std::cout << "List: " << timeContains<CompareItemName, std::list<Item>>(n) << "\n";
        std::cout << "Hash: " << timeContains<CompareItemName, std::unordered_set<Item>>(n) << "\n";
        std::cout << "Tree: " << timeContains<CompareItemName, Tree>(n) << "\n";
    }

    std::cout << "\nQuery (Name)\n";
    for (int n : sizes) {
        std::cout << "n = " << n << "\n";
        std::cout << "Vector: " << timeQueryName<CompareItemName, std::vector<Item>>(n) << "\n";
        std::cout << "List: " << timeQueryName<CompareItemName, std::list<Item>>(n) << "\n";
        std::cout << "Hash: " << timeQueryName<CompareItemName, std::unordered_set<Item>>(n) << "\n";
        std::cout << "Tree: " << timeQueryName<CompareItemName, Tree>(n) << "\n";
    }

    std::cout << "\nQuery (Weight)\n";
    for (int n : sizes) {
        std::cout << "n = " << n << "\n";
        std::cout << "Vector: " << timeQueryWeight<CompareItemWeight, std::vector<Item>>(n) << "\n";
        std::cout << "List: " << timeQueryWeight<CompareItemWeight, std::list<Item>>(n) << "\n";
        std::cout << "Hash: " << timeQueryWeight<CompareItemWeight, std::unordered_set<Item>>(n) << "\n";
        std::cout << "Tree: " << timeQueryWeight<CompareItemWeight, Tree>(n) << "\n";
    }
    return 0;
}