#
# Makefile
#	Author: Alexander Hoke
#	Date:	10/22/20
#

CC		= g++
STD		= -std=c++11
LIBS   	= MarsTraveller.hpp MarsTraveller.cpp MarsMap.hh MarsMap.cc
BFS		= BFSMarsTraveller
AST		= AStarMarsTraveller

all: BFS AST

BFS: $(LIBS) $(BFS).cpp
	$(CC) $(STD) -o $(BFS) $(BFS).cpp $(LIBS)

AST: $(LIBS) $(AST).cpp
	$(CC) $(STD) -o $(AST) $(AST).cpp $(LIBS)

Test: $(LIBS) $(BFS).cpp $(AST).cpp
	$(CC) $(STD) -D TESTING -o $(BFS) $(BFS).cpp $(LIBS)
	$(CC) $(STD) -D TESTING -o $(AST) $(AST).cpp $(LIBS)
	@echo ""
	./$(BFS) -s A Testing/hw2-data1.txt
	@echo ""
	./$(BFS) -s B Testing/hw2-data1.txt
	@echo ""
	./$(BFS) -s C Testing/hw2-data1.txt
	@echo ""
	./$(BFS) -s D Testing/hw2-data1.txt
	@echo ""
	./$(BFS) -s E Testing/hw2-data1.txt
	@echo ""
	./$(BFS) -s F Testing/hw2-data1.txt
	@echo ""
	./$(BFS) -s base Testing/hw2-data1.txt
	@echo ""
	./$(AST) -s A Testing/hw2-data1.txt
	@echo ""
	./$(AST) -s B Testing/hw2-data1.txt
	@echo ""
	./$(AST) -s C Testing/hw2-data1.txt
	@echo ""
	./$(AST) -s D Testing/hw2-data1.txt
	@echo ""
	./$(AST) -s E Testing/hw2-data1.txt
	@echo ""
	./$(AST) -s F Testing/hw2-data1.txt
	@echo ""
	./$(AST) -s base Testing/hw2-data1.txt
	@echo ""
	make clean

clean:
	rm -rf $(AST) $(BFS) $(TEST) *.o