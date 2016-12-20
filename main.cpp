/* 
 * File:   main.cpp
 * Author: JuliusHigiro
 *
 * Created on September 26, 2016, 9:21 PM
 */

#include <iostream>
#include <fstream>
#include <string>
#include "Maze.h"

int main(int argc, char** argv) { 
    if (argc == 2) {
        Maze map(argv[1]);
        map.breadthFirstSearch();
        map.depthFirstSearch();
        map.uniformCostSearch();
        map.startRecursiveDFS();
        
    } else {
        std::cerr << "Specify the following parameters: ";
        std::cerr << "< program > < filename (i.e./home/mazes/maze.txt) >\n";
    }
    return 0;
}



