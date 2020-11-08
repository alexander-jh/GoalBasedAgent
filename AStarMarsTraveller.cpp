/* AStarMarsTraveller.cpp
 *
 * Main implementation for the A* agent.
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
    char *file = *(args + 3);
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
    // Instantiate A* class
    auto astr = new AstarMarsTraveller(file, start);
    // Run class
    bool status = astr->run_rover();
    astr->rover_report(status);
    // Delete instance to clear memory
    delete astr;

    return 0;
}