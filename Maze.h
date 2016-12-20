/* 
 * File:   Maze.h
 * Author: JuliusHigiro
 *
 * Created on September 26, 2016, 9:24 PM
 */

#ifndef MAZE_H
#define MAZE_H

#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <utility>
#include <list>
#include <set>
#include <cstdio>
#include "Node.h"

typedef std::pair<int, int> State;

/**
 * Class to build a maze and execute a search given an initial start state
 * and a goal state.  
 */
class Maze {
    
public:

    /*
     * Maze constructor that takes a file name as the parameter.
     */
    Maze(std::string inputFile);

    /*
     * Determines if the step taken is along the border.
     */
    bool onBorder(Node n);
    
    /*
     * Determines if the node has been explored.
     */
    bool isInExplored(std::set<Node, TotCompare>explored, Node child);
    
    /*
     * Returns a path from goal state to initial state.
     */
    std::vector<Parent>
    backTrack(Node child, std::set<Node, TotCompare>explored);
    
    /* 
     * Performs a search using the depth-first-search algorithm.
    */
    void depthFirstSearch();
    
    /* 
     * Performs a search using the breadth-first-search algorithm.
    */
    void breadthFirstSearch();
    
    /* 
     * Performs a search using the uniform-cost-search algorithm.
    */
    void uniformCostSearch();
     
    /*
     * Initiates recursive depth-first-search algorithm with the root node; 
     */
    void startRecursiveDFS();
    
    /* 
     * Performs a search using the recursive depth-first-search algorithm.
    */
    void
    recursiveDFS(Node node, std::set<Node, TotCompare> explored); 
        
    /*
     * Print the action sequence and the cost using DFS. 
     */
    void printDFSresults(std::vector<Parent> traverse);
    
     /*
     * Print the action sequence and the cost using BFS. 
     */
    void printBFSresults(std::vector<Parent> traverse);
    
     /*
     * Print the action sequence and the cost using UCS. 
     */
    void printUCSresults(std::vector<Parent> traverse);
    
     /*
     * Print the action sequence and the cost using recursive DFS. 
     */
    void printRecursiveDFS(std::vector<Parent> traverse,
    std::set<Node, TotCompare> explored, Parent p);
        
    // Display maze for debugging purposes.
    void print();

    // Display a location in the maze for debugging purposes.
    void printLocation(int row, int col);
    
    
private:
    int numRows, numCols;
    State start;
    State goal;
    std::vector<std::string> maze;

};



#endif /* MAZE_H */

