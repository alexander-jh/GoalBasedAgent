/* MarsTraveller.cpp
 *
 * Source code for header file implementation for Goal Based Agent
 * implemented by Breadth First Search and A* Search algorithms.
 * Testing framework is Catch, which is a header based testing
 * framework.
 *
 * Author:      Alex Hoke
 * Date:        10/22/2020
 * C++ Version: C++11
 * Compiler:    GNU G++
 */

#include "MarsTraveller.hpp"

using namespace std;

const char *BASE = "base";

//---------------------------- History Functions -------------------------//

void History::update_history(Node *loc) {
    // Update samples
    if(samples() < 3) check_samples(loc);
    // Add node to state to seen
}

bool History::check_samples(Node *loc) {
    // Get current node's percept value
    int sample = loc->location->getSamplePercept()->value();
    // Verify it isn't a duplicate or valid sample number and insert
    if(sample_set.find(sample) == sample_set.end() && sample > 0 && sample < 4) {
        sample_set.insert(sample);
    }
    // Return true or false to set number of samples
    return sample_set.size()==3;
}

bool History::observed(Node *loc) {
    bool pair_seen = false;
    // Iterate through all matching keys in seen
    for(auto it = seen.find(string(loc->location->name())); it != seen.end() && !pair_seen; it++) {
        // Compares all internal elements of the hashed node with location node
        pair_seen = it->second->cost == loc->cost &&
                    string(it->second->parent->location->name()) == string(loc->parent->location->name());
    }
    return pair_seen;
}

//---------------------------- MarsTraveller Functions -------------------------//

MarsTraveller::MarsTraveller(const char *file, char *start) {
    mars_map = MarsMap(file);
    MarsPlace *curr = mars_map.getPlace(start);
    // Verify starting position
    if(curr == nullptr) {
        fprintf(stderr,"Error: starting at position %s, exiting\n", start);
        exit(1);
    }
    // Create first node
    route.path = new Node;
    route.path->cost = 0.0;
    route.path->priority = 0.0;
    route.path->location = curr;
    nodes_queued = 0;
    nodes_evaluated = 0;
}

MarsTraveller::~MarsTraveller() {
    free(route.path);
}

bool MarsTraveller::run_rover() {
    Node *root;
    bool goal_met = false;
    // Update priority of first node
    update_priority(route.path, nullptr);
    // Add first node to the frontier
    insert(route.path);
    // Loop until goal is met
    while(!goal_met && size() > 0) {
        // Pop top element if frontier is non-empty
        root = pop();
        // Add to history
        route.update_history(root);
        // Parse children
        read_children(root);
        // Check goal state
        goal_met = goal_test(root);
        // Print the next state
        printf("Current Node: %s\tNext State: %s\n", root->location->name(), front());
    }
    // If goal state achieved build solution path
    if(goal_met) build_solution(root);
    return goal_met;
}

void MarsTraveller::rover_report(bool state) const {
    // If failure terminate reporting
    if(!state) {
        printf("\nFAILURE: No Path to Goal State Found \n");
        return;
    } else {
        printf("\nSUCCESS: Path to Goal State Found \n");
    }
    float total = 0.0;
    int path = 0;
    // Reverse iterate over the solution vector
    auto it = solution.end()-1;
    while(it >= solution.begin()) {
        total += (*it)->cost;
        printf("Move #%5d:\tNode: %4s\tCost: %3.2f\tPriority: %3.2f\tPath Cost: %3.2f\n", ++path,
               (*it)->location->name(), (*it)->cost, (*it)->priority, total);
        --it;
    }
    printf("Nodes Visited: %zu\t\tTotal Cost: %2.2f\t\tNodes Enqueued: %d\t\tNodes Tested: %d\n",
                solution.size(), total, nodes_queued, nodes_evaluated);
}

void MarsTraveller::read_children(Node *root) {
    // Get number of children
    int num_children = root->location->neighborCount();
    char **children = root->location->adjacent();
    // Loop over all children
    for(int i = 0; i < num_children; ++i) {
        // Build child
        Node *child = new Node;
        child->location = mars_map.getPlace(children[i]);
        child->parent = root;
        // Get g(n) cost at child
        child->cost = get_cost(root, child);
        // Updates priority as depth for BFS and f(n) for A*
        update_priority(root, child);
        // If child state is unknown, add to frontier
        if(!route.observed(child)) {
            route.update_frontier(child);
            insert(child);
        }
    }
}

void MarsTraveller::build_solution(Node *goal) {
    // Put the route from solution to goal into stack by traversing predecessors
    while(goal != nullptr) {
        solution.push_back(goal);
        goal = goal->parent;
    }
}

float MarsTraveller::get_cost(Node *root, Node *child) {
    float distance = root->location->distanceTo(child->location->name());
    if(strcmp(child->location->name(), BASE) == 0) {
        distance += (float) (100.0 * (3.0 - route.samples()));
    }
    return distance;
}

bool MarsTraveller::goal_test(Node *root) {
    // Increment test count
    nodes_evaluated++;
    // BFS terminal state is when current node is base and samples are seen
    return strcmp(root->location->name(), BASE) == 0 && (route.samples() == 3);
}

//---------------------------- BFSMarsTraveller Functions -------------------------//

void BFSMarsTraveller::update_priority(Node *root, Node *child) {
    // If child is not null update depth
    if(child) {
        child->priority = (float) (root->priority + 1.0);
    }
}

//---------------------------- AStarMarsTraveller Functions -------------------------//

void AstarMarsTraveller::update_priority(Node *root, Node *child) {
    // If child is not null update priority with heuristic
    if(child) {
        child->priority = distance_to_base(child);
    // Otherwise correct root node's priority
    } else {
        root->priority = distance_to_base(root);
    }
}

float AstarMarsTraveller::distance_to_base(Node *child) {
    float x_dist = child->location->xcoord() - mars_map.getPlace(BASE)->xcoord();
    float y_dist = child->location->ycoord() - mars_map.getPlace(BASE)->ycoord();
    return sqrt(x_dist*x_dist + y_dist*y_dist);
}
