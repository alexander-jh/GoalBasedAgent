#
# Makefile
#	Author: Alexander Hoke
#	Date:	10/22/20
#

CC		= g++
STD		= -std=c++11
MT   	= MarsTraveller
MM      = MarsMap
OBJ		= MarsMap.o MarsTraveller.o
BFS		= BFSMarsTraveller
AST		= AStarMarsTraveller
CATCH	= Testing/catch.hpp
TEST   	= MarsTravellerUnitTesting

all: BFS AST

BFS: $(OBJ) $(BFS).o
	$(CC) $(STD) $(OBJ) $(BFS).o -o $(BFS)
	rm -rf *.o

AST: $(OBJ) $(AST).o
	$(CC) $(STD) $(OBJ) $(AST).o -o $(AST)
	rm -rf *.o

$(BFS).o: $(BFS).cpp $(MT).hpp
	$(CC) $(STD) -c $(BFS).cpp

$(AST).o: $(AST).cpp $(MT).hpp
	$(CC) $(STD) -c $(AST).cpp

$(MM).o: $(MM).cc $(MM).hh $(MT).hpp $(MT).cpp
	$(CC) $(STD) -c $(MM).cc

$(MT).o: $(MT).cpp $(MT).hpp $(AST).cpp $(BFS).cpp
	$(CC) $(STD) -c $(MT).cpp

Test: $(OBJ) $(BFS).o $(AST).o
	$(CC) $(STD) $(OBJ) $(BFS).o -o $(BFS)
	$(CC) $(STD) $(OBJ) $(AST).o -o $(AST)
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
	make clean

clean:
	rm -rf $(AST) $(BFS) $(TEST) *.o