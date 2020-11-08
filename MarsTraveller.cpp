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

int History::check_samples(Node *loc) {
    unordered_set<int> sample_set;
    int sample;
    // Transcend tree to ensure all locations are in path of state
    while(loc != nullptr) {
        // Get sample value
        sample = loc->location->getSamplePercept()->value();
        // If sample is a valid value has into set
        if(sample > 0 && sample < 4) sample_set.insert(sample);
        loc = loc->parent;
        //
        if(sample_set.size() == 3) break;
    }
    // Return number of samples
    return sample_set.size();
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
    route.path->samples = route.check_samples(route.path);
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
        //route.update_history(root);
        // Parse children
        read_children(root);
        // Check goal state
        goal_met = goal_test(root);
#ifndef TESTING
        // Print the next state
        printf("Current Node: %s\t\tNext State: %s\n", root->location->name(), front());
#endif
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
    int path = 0;
    // Reverse iterate over the solution vector
    auto it = solution.end()-1;
    while(it >= solution.begin()) {
        printf("Move #%5d:\tNode: %4s\tCost: %3.2f\tPriority: %3.2f\n", ++path,
               (*it)->location->name(), (*it)->cost, (*it)->priority);
        --it;
    }
    printf("Nodes Visited: %zu\t\tTotal Cost: %2.2f\t\tNodes Enqueued: %d\t\tNodes Tested: %d\n",
                solution.size(), (*solution.begin())->cost, nodes_queued, nodes_evaluated);
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
        // Update sample count at child state
        (root->samples == 3) ? (child->samples = root->samples)
                             : (child->samples = route.check_samples(child));
        // Get g(n) cost at child
        child->cost = root->cost + get_cost(root, child);
        // Updates priority as depth for BFS and f(n) for A*
        update_priority(root, child);
        insert(child);
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
        distance += (float) (100.0 * (3.0 - root->samples));
    }
    return distance;
}

bool MarsTraveller::goal_test(Node *root) {
    // Increment test count
    nodes_evaluated++;
    // BFS terminal state is when current node is base and samples are seen
    return strcmp(root->location->name(), BASE) == 0 && (root->samples == 3);
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
        child->priority = child->cost + distance_to_base(child);
    // Otherwise correct root node's priority
    } else {
        // If starting at base set to negative value
        float heuristic = distance_to_base(root);
        (heuristic == 0.0) ? (root->priority = -1.0) : (root->priority = heuristic);
    }
}

float AstarMarsTraveller::distance_to_base(Node *child) {
    // h = 0 if base
    if(strcmp(child->location->name(), BASE) == 0) return 0.0;
    float x_dist = child->location->xcoord() - mars_map.getPlace(BASE)->xcoord();
    float y_dist = child->location->ycoord() - mars_map.getPlace(BASE)->ycoord();
    return sqrt(x_dist*x_dist + y_dist*y_dist);
}
