[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=9916143&assignment_repo_type=AssignmentRepo)
# Assignment 1

Name: Vincent Sang
Course: CSCI 335 
Semester: Spring 2026

## Information about the repository

This repository contains the following:
- Starter code: `Compare.cpp`, `Compare.hpp`,HashInventory.cpp`, `HashInventory.hpp`, `Inventory.cpp` `Inventory.hpp`, `Item.cpp`, `Item.hpp`, `ItemAVL.cpp`, `ItemAVL.hpp`, `ItemGenerator.cpp`, `ItemGenerator.hpp`, `TreeInventory.cpp`, `TreeInventory.hpp`, `Makefile`, `335_Project2_Task4.pdf`, `main.cpp`, and `README.md`
- Assignment specification: `Project2.pdf`

Parts Completed:
Part 1
contains() for vector and list
discard() for vector and list

Part 2
pickup(), discard(), contains(), query() for unordered_set

Part 3
ItemAVL contains()
Inventory Tree contains()
Inventory Tree query()
queryHelper() implementation

Bugs Encountered:
Segementation fault in the Tree query due to incorrect recursion logic
Template redefinition errors caused by compiling .cpp files seperately
Incorrect placement of template implementations between .hpp and .cpp files
All issues were eventually resolved and everything compiles and runs correctly on gradescope

How to compile:
make
make clean
make all        

To run:
./program

Thoughts:
I think the big five was the most simple to implement and easiest to understand. However, I did find the 
overloading << and >> to support formatted input and output challenging due to the strict formatting requirements. You definitely had to take special care with the memory leaks and how to handle errors and aborting to match the output exactly like how the autograder wants it. 

## Running the assignment

To compile on terminal, type:

```bash
make clean
make all
```

To delete executables and object files, type:

```bash
make clean
```

To run, type:

```bash
./program
```

## Providing Input from Standard Input

To run with a given file that is redirected to standard input:

```bash
./main
```
