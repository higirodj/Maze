/* 
 * File:   Node.h
 * Author: JuliusHigiro
 *
 * Created on September 26, 2016, 9:24 PM
 */

#ifndef NODE_H
#define NODE_H

#include <ostream>
#include <vector>
#include <string>

class Parent {
public:
    /* Parent constructor that takes row and colum as the parameter.
     * The Parent will be used to perform a trace back from goal state
     * to the initial state.
     */
    Parent(int r, int c, int cost_, char action_) : row(r), col(c),
             cost(cost_), action(action_) {
    }
    inline int parentRow(){return row;}
    inline int parentCol(){return col;}
    inline int parentCost() const {return cost;}
    inline char parentAction(){return action;}
    void updateCost(int cost_) {this->cost = cost_;}
    
    // Prints the parent object for debugging purposes.
    friend std::ostream &operator<<(std::ostream& os, const Parent &p) {
        os << "(" << p.row << ", " << p.col << ")" << std::endl;
        return os;
    }
private:
    int row, col, cost;
    char action;
};

class Node {
    friend class ColCompare;
    friend class RowCompare;
    friend class TotCompare;
    friend class CostCompare;
public:
    /* Node constructor that takes row and column as the parameter.
       Node location (row, col) is equivalent to current node state.
     */
    Node(Parent p, int r, int c) : parent(p), row(r), col(c) {}

    // Returns the current row.
    inline int nodeRow() {return row;}

    // Returns the current column.
    inline int nodeCol() {return col;}
    
    inline Parent& parentState() {return parent;}
    
    // Returns the children of the parent node and set the action of the
    // parent.
    std::vector<Node> expansion(std::vector<std::string> map) {

        std::vector<Node>nodes;
        if (map[this->row - 1][this->col] == ' ') {
            // Parent state of northern child
            Parent nParent(this->row, this->col, 1, 'N');
            //Northern child
            Node northernChild(nParent, this->row - 2, this->col);
            // Store children in a vector.
            nodes.push_back(northernChild);
        }

        if (map[this->row + 1][this->col] == ' ') {
            // Parent state of southern child
            Parent sState(this->row, this->col, 1, 'S');
            //Southern child
            Node southernChild(sState, this->row + 2, this->col);
            // Store children in a vector.
            nodes.push_back(southernChild);

        }
        
        if (map[this->row][this->col - 1] == ' ') {
            // Parent state of western child
            Parent sParent(this->row, this->col, 1, 'W');
            // Western child
            Node westernChild(sParent, this->row, this->col - 2);
            // Store children in a vector.
            nodes.push_back(westernChild);
        }

        if (map[this->row][this->col + 1] == ' ') {
            // Parent state of eastern child
            Parent eParent(this->row, this->col, 1, 'E');
            // Eastern child
            Node easternChild(eParent, this->row, this->col + 2);
            // Store children in a vector.
            nodes.push_back(easternChild);
        }
        
        return nodes;
    }
    bool operator==(const Node &n) {
        return ( (this->row == n.row) && (this->col == n.col) );
    }
    // Prints the node object.
    friend std::ostream &operator<<(std::ostream& os, const Node &n) {
        os << "(" << n.row << ", " << n.col << ")" << std::endl;
        return os;
    }
private:
    Parent parent;
    int row, col;
};

/**
 * A custom comparator that compares state of two nodes. 
 * The comparator returns the node with the largest row value. 
 */
class RowCompare {
public:

    bool operator()(const Node &a, const Node &b) {
        return a.row < b.row;
    }
};

/**
 * A custom comparator that compares the state of two nodes. 
 * The comparator returns the node with the largest column value. 
 */
class ColCompare {
public:

    bool operator()(const Node &a, const Node &b) {
        return a.col < b.col;
    }
};

/**
 * A custom comparator that compares the state of two nodes. 
 * The comparator returns the node with the largest column value. 
 */
class TotCompare {
public:

    bool operator()(const Node &a, const Node &b) {
        return ((a.row < b.row) || (a.col < b.col));
    }
};

/**
 * A custom comparator that compares the state of two nodes based
 * on the cost. 
 * The comparator returns the node with the largest move cost. 
 */
class CostCompare {
public:

    bool operator()(const Node &a, const Node &b) {
        return (a.parent.parentCost() > b.parent.parentCost());
    }
};

#endif /* NODE_H */

