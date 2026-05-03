[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=9916143&assignment_repo_type=AssignmentRepo)
# Project 3

Name: Vincent Sang
Course: CSCI 335 
Semester: Spring 2026

## Information about the repository

This repository contains the following:
- Starter code: `Leaderboard.cpp`, `Leaderboard.hpp`, `Player.cpp`, `Player.hpp`, `PlayerStream.cpp`, `PlayerStream.hpp`, `Makefile`, and `README.md`
- Assignment specification: `Project3.pdf`

Parts Completed:
Task 1 Part A
heapRank() implementation using early-stopping heapsort

Task 1 Part B
partitionPlayers() helper
quickSortRange() helper
hybridSelectSort() helper
quickSelectRank() implementation using quickselect/quicksort hybrid

Task 2 Part A
VectorPlayerStream constructor
nextPlayer()
remaining()

Task 2 Part B
replaceMin() implementation for min-heap replacement and percolate down

Task 2 Part C
rankIncoming() implementation for continuous stream processing
cutoff tracking at reporting milestones
final sorting of heap contents for leaderboard output

Bugs Encountered:
Missing standard library includes in Leaderboard.cpp caused compilation issues
Uninitialized id_ field in Player caused unsafe behavior
Heap comparator issues while maintaining min-heap behavior for the online algorithm
Formatting and header include consistency needed to ensure Gradescope compatibility
All issues were eventually resolved and everything compiles and runs correctly on gradescope

How to compile:
make
make clean
make all        

To run:
./main

Thoughts:
The online namespace part was definitely the most interesting because it combined many topics that we learned in class such as streams, heaps, and tracking milestone cutoffs. The offline ranking was the most straightforward for me to understand, while the quickselect and quicksort hybrid took more time to do since I had to carefully understand it slowly on when to recurse with quickselect logic and when to fully sort part of the vector. The replaceMin() helper was also pretty difficult at first because of the iterator-based heap indexing but eventually once I understood that the root starts at the first valid position of the vector, it started to make more sense.

## Running the assignment

To compile on terminal, type:

```bash
make clean
make all