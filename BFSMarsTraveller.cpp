/* BFSMarsTraveller.cpp
 *
 * Main implementation for the BFS agent.
 * Testing framework is Catch, which is a header based testing
 * framework.
 *
 * Author:      Alex Hoke
 * Date:        10/22/2020
 * C++ Version: C++11
 * Compiler:    GNU G++
 */
#include "MarsTraveller.hpp"

int main(int argv, char **args) {
    // Get file destination
    const char *file = *(args + 3);
    // Get starting position
    char *start = *(args + 2);
    // Verify proper arguements were received
    if(file == nullptr) {
        fprintf(stderr,"Error: no valid file, exiting\n");
        exit(1);
    } else if(start == nullptr) {
        fprintf(stderr,"Error: no valid starting position, exiting\n");
        exit(1);
    }
    // Instantiate BFS class
    BFSMarsTraveller bfs = BFSMarsTraveller(file, start);
    // Run class
    bool status = bfs.run_rover();
    bfs.rover_report(status);

    return 0;
}