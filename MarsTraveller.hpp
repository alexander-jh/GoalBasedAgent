/* MarsTraveller.hpp
 *
 * Header file for Goal Based Agent implemented by Breadth First Search and
 * A* Search algorithms. Testing framework is Catch, which is a header
 * based testing framework.
 *
 * Author:      Alex Hoke
 * Date:        10/22/2020
 * C++ Version: C++11
 * Compiler:    GNU G++
 */
#include <unordered_set>
#include <map>
#include <queue>
#include <stack>
#include <cmath>
#include "MarsMap.hh"

using namespace std;


// Typedef tuple for priority (depth for BFS/f(n)+h(n) for A*,
// true cost at node, and pointer to MarsPlace
typedef struct Node {
    float       priority;
    float       cost;
    MarsPlace   *location;
    struct Node *parent;
}Node;

// Comparator for priority queue based on Node.priority
struct node_comparator {
    bool operator()(const Node *a, const Node *b) {
        return a->priority < b->priority;
    }
};

//  Helper class that contains useful and common stores to both
//  classes.
class History {
// Public class members and functions
public:
    // Tree of search moves takes
    Node *path;
    // Reports if a node has been previously visited and expanded
    bool observed(Node *loc);
    // Updates path to reflect most recent node visited, adds node to path tree
    void update_history(Node *loc);
    // Updates the contents of frontier
    inline void update_frontier(Node *loc) {if(!observed(loc)) seen.insert(
                                    {string(loc->location->name()), loc});};
    // Reports the number of samples collected so far, 3 implies they're all collected
    inline int samples(){return sample_set.size();};

// Private class members and functions
private:
    // Stores current state of the frontier
    multimap<string, Node *> seen;
    // Set of collected samples
    unordered_set<int> sample_set;
    // Helper function to quickly verify if a sample exists without
    // accessing member of Node
    bool check_samples(Node *sample);
};

// Abstract superclass implementation for use in both search algorithms
class MarsTraveller {
// Public class members and objects
public:
    // History route object stores relevant path and state variables
    // publicly accessible so subclasses can have access
    History route;
    // Mars maps object for instantiation. Holds relevant percept and
    // current world states
    MarsMap mars_map;
    // Number of nodes evaluated
    int nodes_evaluated;
    // Number of nodes enqueued
    int nodes_queued;
    // Constructor is inherited by both subclasses
    //      char *file:     File location for mars_map input
    //      char *start     Starting position character
    MarsTraveller(const char *file, char *start);
    // Destructor for MarsTraveller, both subclasses also follow default
    // Neither subclass instance nor the super class invoke the new creator
    // or dynamically allocate memory outside their scope so the default
    // destructor is sufficient
    ~MarsTraveller();
    // Default run sequence for general search algorithm, overloaded
    // functions from subclasses implement specific search type. Upon
    // completion of function returns to calling program.
    bool run_rover();
    // Reports the results from the entire run to console
    void rover_report(bool state) const;
    // Returns the cost between child and parent node
    float get_cost(Node *root, Node *child);

// Private class members and objects
private:
    vector<Node *> solution;
    // Read all children at the current node invoking subclass' insert
    // methods
    void read_children(Node *root);
    // Make solution route from the traversed path
    void build_solution(Node *goal);
    // Conducts goal test
    bool goal_test(Node *loc);
    // Pure virtual placeholders overridden in subclasses
    virtual int size() = 0;
    virtual void insert(Node *child) = 0;
    virtual Node *pop() = 0;
    virtual const char *front() = 0;
    virtual void update_priority(Node *root, Node *child) = 0;
};

// Breadth First Search agent, extends MarsTraveller
class BFSMarsTraveller : public MarsTraveller {
// Public member functions
public:
    // Inherits MarsTraveller Constructor
    BFSMarsTraveller(const char *file, char *start) : MarsTraveller(file, start){};
    // Default destructor handled by superclass
    ~BFSMarsTraveller()= default;
// Private member functions and class variables
private:
    queue<Node *> frontier;
    // Reports size of queue
    inline int size() override {return frontier.size();};
    // Insets a new node to the queue
    inline void insert(Node *child) override{frontier.push(child); nodes_queued++;};
    // Removes top element from the queue returning the node
    inline Node *pop() override {Node *n = frontier.front(); frontier.pop(); return n;};
    // Returns the top of frontier
    inline const char *front() override {if(!frontier.empty()) return frontier.front()->location->name();
                                            return "None";};
    // Updates the cost of the current Node
    void update_priority(Node *root, Node *child) override;
};

// A* Search agent, extends MarsTraveller
class AstarMarsTraveller : public MarsTraveller {
// Public member functions
public:
    // Inherits MarsTraveller Constructor
    AstarMarsTraveller(const char *file, char *start) : MarsTraveller(file, start){};
    ~AstarMarsTraveller()= default;
// Private member functions and class variables
private:
    priority_queue<Node*, vector<Node*>, node_comparator> frontier;
    // Reports the current size of the priority queue
    inline int size() override {return frontier.size();};
    // Inserts element into the queue, calculating distance and applying the
    // heuristic to the first element of the node tuple
    inline void insert(Node *child) override{frontier.push(child); nodes_queued++;};
    // Removes highest element in priority queue
    inline Node *pop() override{Node *top = frontier.top(); frontier.pop(); return top;};
    // Returns the top of frontier
    inline const char *front() override {if(!frontier.empty()) return frontier.top()->location->name();
                                        return "None";};
    // Updates the cost and heuristic of the current node
    void update_priority(Node *root, Node *child) override;
    // Gets the straight line distance between a node and base
    float distance_to_base(Node *child);
};