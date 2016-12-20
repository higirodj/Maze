/* 
 * File:   Maze.cpp
 * Author: JuliusHigiro
 *
 * Created on September 26, 2016, 9:24 PM
 */

#include <fstream>
#include <iostream>
#include <string>
#include "Maze.h"

Maze::Maze(std::string inputFile) {
    std::string line;
    std::ifstream input;
    input.open(inputFile);
    if (input.is_open()) {
        input >> numRows >> numCols;
        std::getline(input, line);
        // Build maze and initialize start and goal states.
        for (int i = 0; i <= (numRows * 2) + 1; i++) {
            std::getline(input, line);
            maze.push_back(line);
            auto searchStart = std::find(maze[i].begin(), maze[i].end(), 'S');
            auto searchGoal = std::find(maze[i].begin(), maze[i].end(), 'F');
            if (searchStart != maze[i].end()) {
                auto start_index = std::distance(maze[i].begin(), searchStart);
                start = std::make_pair(i, start_index);
            }
            if (searchGoal != maze[i].end()) {
                auto goal_index = std::distance(maze[i].begin(), searchGoal);
                goal = std::make_pair(i, goal_index);
            }

        }

    } else {
        std::cout << "There was an error while opening the file." << std::endl;
    }
    input.close();
}

bool
Maze::onBorder(Node n) {
    return isOnBorder = ((n.nodeRow() - 1) == 0 ||
            (n.nodeRow() + 1) == numRows * 2 || (n.nodeCol() - 1 == 0) ||
            (n.nodeCol() + 1) == numCols * 2);
}

bool
Maze::isInExplored(std::set<Node,TotCompare> explored, Node child) {    
    for (auto it = explored.begin(); it != explored.end(); it++) {
        Node e = *it;
        if ((child.nodeRow() == e.nodeRow()) &&
                (child.nodeCol() == e.nodeCol())) {
            return true;
        }
    } 
    return false;
}

std::vector<Parent>
Maze::backTrack(Node node, std::set<Node, TotCompare> explored) {
    std::vector<Parent>path;
    Parent p = node.parentState();
    //Update cost of goal node.
    if (onBorder(node)) {
        p.updateCost(p.parentCost() + 10);
    }
    path.push_back(p);
    auto iter = explored.begin();
    // Recover the path of the traversal. 
    while (true) {
        Node node = *iter;
        if ((node.nodeRow() == p.parentRow()) && 
                (node.nodeCol() == p.parentCol())) {
            p = node.parentState();
            //Update costs for border nodes
            if (onBorder(node)) {
                p.updateCost(p.parentCost() + 10);
            }
            path.push_back(p);
            iter = explored.begin();
            // End backtrack from goal state to initial state
            if ((path.back().parentRow() == start.first) &&
                    (path.back().parentCol() == start.second)) {
                break;
            }
        } else {
            iter++;
        }
    }
    return path;
}

void
Maze::depthFirstSearch() {
    std::vector<Parent>path;
    Parent parent(start.first, start.second, 1, 'X');
    Node root(parent, start.first, start.second);
    // The frontier is the LIFO stack data structure used to store node
    // elements. A vector container is used because of flexibility.
    std::vector<Node>frontier;
    std::set<Node, TotCompare>explored;
    frontier.push_back(root);
    while (!frontier.empty()) {
        Node n = frontier.back();
        frontier.pop_back();
        explored.insert(n);
        // Expand and explore.
        std::vector<Node>children = n.expansion(maze);
        for (auto it = children.begin(); it!= children.end(); it++) {
            bool inFrontier = false;
            bool inExplored = false;
            Node child = *it;
            // Check if the goal is satisfied by the child.
            if ((child.nodeRow() == goal.first) &&
                    (child.nodeCol() == goal.second)) {
                // If goal is satisfied backtrack from the goal state
                // to initial state.
                path = backTrack(child, explored);
                goto label; // Finished search, go home.   
            }  
            // Check if child is on the stack.
            // std::vector::find would be useful here but not working as
            // expected despite operator overloading.
            for(auto start = frontier.begin(); start!= frontier.end(); start++) {
                Node f = *start;
                if( (child.nodeRow() == f.nodeRow()) &&
                        (child.nodeCol() == f.nodeCol()) ) {
                    inFrontier = true;
                    break;
                }
            }
            // Check if child has been explored.
            inExplored = isInExplored(explored, child);
            // Push child to the stack if the child has not been explored and 
            // if the child is not on the stack.
            if ( !(inExplored) && !(inFrontier)  ) {
                frontier.push_back(child);
            }
        } 
    }
    label: 
    printDFSresults(path);
}

void
Maze::printDFSresults(std::vector<Parent> traverse) {
   std::reverse(traverse.begin(), traverse.end());
   std::cout <<"DFS: "<<"";
   int totCost = 0;
   for(auto it = traverse.begin(); it!= traverse.end(); it++) {
        Parent states = *it;
        totCost+=states.parentCost();
        std::cout << states.parentAction() << "";
   } 
   std::cout<< " cost = " << totCost << std::endl;
}

void
Maze::breadthFirstSearch() {
    std::vector<Parent>path;
    Parent parent(start.first, start.second, 1, 'X');
    Node root(parent, start.first, start.second);
    // The frontier is the FIFO queue data structure used to store node
    // elements. A list container is used because of flexibility.
    std::list<Node>frontier;
    std::set<Node, TotCompare>explored;
    frontier.push_back(root);
    while(!frontier.empty()) {
        Node n = frontier.front();
        // Check if the goal is satisfied by the child.
        if ((n.nodeRow() == goal.first) &&
            (n.nodeCol() == goal.second)) {
            // If goal is satisfied backtrack from the goal state
            // to initial state.
            path = backTrack(n, explored);
            goto label; // Finished search, go home.
        }
        frontier.pop_front();
        explored.insert(n);
        // Expand and explore
        std::vector<Node>children = n.expansion(maze);
        for (auto it = children.begin(); it!= children.end(); it++) {
            bool inFrontier = false;
            bool inExplored = false;
            Node child = *it;
            // Check if child is in the queue.
            for(auto start = frontier.begin(); start!= frontier.end(); start++) {
                Node f = *start;
                if( (child.nodeRow() == f.nodeRow()) &&
                        (child.nodeCol() == f.nodeCol()) ) {
                    inFrontier = true;
                    break;
                }
            }
            // Check if child has been explored.
            // std::set::find would be useful here but not working as expected
            // despite operator overloading.
            inExplored = isInExplored(explored, child);
            // Push child into the queue if the child has not been explored and 
            // if the child is not on the stack.
            if ( !(inExplored) && !(inFrontier)  ) {
                frontier.push_back(child);
            }      
        }
    }
    label:
    printBFSresults(path);
}

void
Maze::printBFSresults(std::vector<Parent> traverse) {
   std::reverse(traverse.begin(), traverse.end());
   std::cout <<"BFS: "<<"";
   int totCost = 0;
   for(auto it = traverse.begin(); it!= traverse.end(); it++) {
        Parent states = *it;
        totCost+=states.parentCost();
        std::cout << states.parentAction() << "";
   } 
   std::cout<< " cost = " << totCost << std::endl;
}

void
Maze::uniformCostSearch() {
    std::vector<Parent>path;
    Parent parent(start.first, start.second, 1, 'X');
    Node root(parent, start.first, start.second);
    // The frontier is the priority queue data structure used to store node
    // elements. A vector maintained as a heap will be used because of
    // flexibility.
    std::vector<Node>frontier;
    std::set<Node, TotCompare>explored;
    frontier.push_back(root);
    std::make_heap(frontier.begin(), frontier.end(), CostCompare());
    while(!frontier.empty()) {
        Node n = frontier.front();
        std::pop_heap(frontier.begin(), frontier.end(), CostCompare());
        frontier.pop_back();
        // Check if the goal is satisfied by the child.
        if ((n.nodeRow() == goal.first) &&
            (n.nodeCol() == goal.second)) {
            Parent p = n.parentState();
            path.push_back(p);
            auto iter = explored.begin();
            // Recover the path of the traversal.
            while(true) {
                Node node = *iter;
                if( (node.nodeRow() == p.parentRow()) &&
                        (node.nodeCol() == p.parentCol()) ) {
                    p = node.parentState();
                    path.push_back(p); 
                    iter = explored.begin();
                    // End backtrack from goal state to initial state
                    if( (path.back().parentRow() == start.first) &&
                                (path.back().parentCol() == start.second) ) {
                            break;
                    }
                } else {
                    iter++;
                }
                
            } goto label;
        }
        explored.insert(n);
        // Expand and explore
        std::vector<Node>children = n.expansion(maze);
        for (auto it = children.begin(); it!= children.end(); it++) {
            bool inFrontier = false;
            bool inExplored = false;
            Node child = *it;
            // Check if child is in the frontier.
            for(auto start = frontier.begin(); start!= frontier.end(); start++) {
                Node f = *start;
                if( (child.nodeRow() == f.nodeRow()) &&
                        (child.nodeCol() == f.nodeCol()) ) {
                    inFrontier = true;
                    break;
                }
            }
            // Check if child has been explored.
            // std::set::find would be useful here but not working as expected
            // despite operator overloading.
            for(auto start_ = explored.begin(); start_!= explored.end();
                    start_++) {
                Node e = *start_;
                if( (child.nodeRow() == e.nodeRow()) &&
                        (child.nodeCol() == e.nodeCol()) ) {
                    inExplored = true;
                    break;    
                }
            } 
            // Push child into the set if the child has not been explored and 
            // if the child is not in the vector.
            if ( !(inExplored) && !(inFrontier)  ) {
                // Update the cost of nodes.
                Parent* p = &child.parentState();
                if(onBorder(child)) {
                    p->updateCost(p->parentCost()+10);
                }
                frontier.push_back(child);
                std::push_heap(frontier.begin(), frontier.end(), CostCompare()); 
            }    
        }      
    }
    label: 
    printUCSresults(path);
}

void
Maze::printUCSresults(std::vector<Parent> traverse) {
   std::reverse(traverse.begin(), traverse.end());
   std::cout <<"UCS: "<<"";
   int totCost = 0;
   for(auto it = traverse.begin(); it!= traverse.end(); it++) {
        Parent states = *it;
        totCost+=states.parentCost();
        std::cout << states.parentAction() << "";
   } 
   std::cout<< " cost = " << totCost << std::endl;
}

void
Maze::startRecursiveDFS() {
    Parent parent(start.first, start.second, 1, 'X');
    Node root(parent, start.first, start.second);
    std::set<Node, TotCompare>explored;
    recursiveDFS(root, explored);
}

void
Maze::recursiveDFS(Node node, std::set<Node, TotCompare>explored) {
    std::vector<Parent>path;
    explored.insert(node);
    // Check if the goal is satisfied by the node.
    if ((node.nodeRow() == goal.first) && (node.nodeCol() == goal.second)) {
        Parent p = node.parentState();
        // Update cost of goal node
        if (onBorder(node)) {
            p.updateCost(p.parentCost() + 10);
        }
        path.push_back(p);
        printRecursiveDFS(path, explored, p);
    } else {
        // Expand and recursively explore
        std::vector<Node>children = node.expansion(maze);
        for (auto it = children.begin(); it!= children.end(); it++) {
            Node child = *it;
            bool inExplored = false;
            // Check if child has been explored.
            // std::set::find would be useful here but not working as expected
            // despite operator overloading.
            for(auto iter = explored.begin(); iter!= explored.end();
                    iter++) {
                Node e = *iter;
                if( (child.nodeRow() == e.nodeRow()) &&
                        (child.nodeCol() == e.nodeCol()) ) {
                    inExplored = true;
                    break;    
                }
            }
            if(!inExplored) {
                recursiveDFS(child, explored);
            }
        }
    } 
}
 
void
Maze::printRecursiveDFS(std::vector<Parent> path, std::set<Node,
        TotCompare>explored, Parent p) {
    auto iter = explored.begin();
    // Recover the path of the traversal. 
    while (true) {
        Node n = *iter;
        if ((n.nodeRow() == p.parentRow()) &&
                (n.nodeCol() == p.parentCol())) {
            p = n.parentState();
            if(onBorder(n)){
                p.updateCost(p.parentCost()+10);
            }
            path.push_back(p);
            iter = explored.begin();
            // End backtrack from goal state to initial state
            if ((path.back().parentRow() == start.first) &&
                    (path.back().parentCol() == start.second)) {
                break;
            }
        } else {
            iter++;
        }

    }
   std::reverse(path.begin(), path.end());
   std::cout <<"Recursive DFS: "<<"";
   int totCost = 0;
   for(auto it = path.begin(); it!= path.end(); it++) {
        Parent states = *it;
        totCost+=states.parentCost();
        std::cout << states.parentAction() << "";
   } 
   std::cout<< " cost = " << totCost << std::endl;
}

void
Maze::print() {
    for (int i = 0; i <= (numRows * 2) + 1; i++) {
        std::cout << maze[i] << std::endl;
    }   
}

void
Maze::printLocation(int row, int col) {
    std::cout << maze[row][col] << std::endl;
}
