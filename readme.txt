Project:	Lab 2
Author:		Alexander Hoke
Date:		11/05/2020

------------------------------ INCLUDED ------------------------------
Makefile					-	Build file for the project.
AStarMarsTraveller.cpp		-	Source code with main for A* agent.
BFSMarsTraveller.cpp		-	Source code for the BFS agent.
MarsTraveller.hpp			-	Header file for implementation of both
								agents.
MarsTraveller.cpp     		-	Source code implementing the header.
MarsMap.hh					-	Header instructor provided for the
								input parser.
MarsMap.cc  				-	Source code for parser.

---------------------------- INSTRUCTIONS ----------------------------
Run the following for both StateRover and ReflexRover executables:
	make
	./BFSMarsTraveller -s <start location> <<file-name>>.txt
	./AStarMarsTraveller -s <start location> <<file-name>>.txt

To remove the executable files:
	make clean

To individually compile either file:
	make BFS
	make AST

All instances of the primary executable are copiled using C++ 11:
	g++ -std=c++11


---------------------------- UNIT TESTING -----------------------------
The testing philosophy was a little more simple this time around. Due 
to the dynamic nature of the alogirithms with extensively large 
search trees in the case of BFS only two solution paths were directly
verified.

For both A* and BFS the path starting from nodes A and B were used. The 
I drew out the correct paths manually and compared results. For the A* 
this was done by calculating path costs and for BFS because the search 
tree are a bit larger I solved for the lowest depth it finds a solution. 
In the file the average branching factor per node was roughly 3. So for 
example at a depth of 6 you'd expect the program to test in the neighbo-
rhood of 1093 nodes.

The other portion of the testing strategy was to exhaustively test all 
possible outcomes verifying it worked from every node. The one of main 
conern was being sure that starting from base still allowed the program 
to run.