extern "C" {
  #include <stdio.h>
  #include <stdlib.h>
  #include <strings.h>
}
#include <string.h>
#include <map>

#define MAXOUTDEGREE 10

/*
  SamplePercept: the object holding a soil sample perception
  
  Constructor: SamplePercept(char *text) converts text into integer percept
    representation of "text" should be ASCII
  value(): returns integer representation of percept

*/
class SamplePercept {
private:
  int myPercept;

public:
  explicit SamplePercept(char *text);
  inline int value() const { return myPercept;} ;
};


/*
  MarsPlace: holds information about a single place on mars
  
  Constructor: MarsPlace(char *text) converts a line of text into the
    appropriate MarsPlace
  
    float distanceTo(const char *node): returns distance to node from
      here if node is adjacent, -1 if not
      
    int neighborCount(): returns number of adjacent nodes    
    
    char **adjacent(): returns list of adjacent node names

    SamplePercept *getSamplePercept(): returns the SamplePercept for this node
    
    float xcoord(), float ycoord(): returns coordinates of this place
    
    const char *name(): returns the name of this node
*/


class MarsPlace {
  int myNeighborCount;
  char **myAdjacent;
  float *myDistance;
  float myXcoord;
  float myYcoord;
  SamplePercept *myPercept;
  const char *myName;

public:

  explicit MarsPlace(char *x);
  ~MarsPlace();
  float distanceTo(const char *n2);
  
  inline int neighborCount() const { return myNeighborCount; }
  char **adjacent() { return myAdjacent; }
  SamplePercept *getSamplePercept() { return myPercept; }
  float xcoord() const {return myXcoord; }
  float ycoord() const {return myYcoord; }
  const char *name() {return myName;}

};


/*
 helper class
*/
struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

/* 
   MarsMap: the object holding a series of MarsPlace objects

   Constructor: MarsMap(char *filename) converts file into MarsMap
   Constructor: MarsMap(): reads MarsMap from stdin

   float getDistance(MarsPlace *p1, MarsPlace *p2): returns distance
     between 2 adjacent places

   float getDistance(char *p1, char *p2): returns distance
     between 2 adjacent places
     
   MarsPlace *getPlace(char *place): returns pointer to a place
     MEMORY: the MarsMap owns the pointer, you don't have to free it

*/

class MarsMap {
private:
  std::map<const char *,MarsPlace *,ltstr> h;
  void initialize(FILE *fp);

public:
  MarsMap() = default;
  explicit MarsMap(const char *filename);
  ~MarsMap();
  MarsPlace *getPlace(const char *place);
  static float getDistance(MarsPlace *p1, MarsPlace *p2);
  float getDistance(const char *p1, const char *p2);
  
};


  
