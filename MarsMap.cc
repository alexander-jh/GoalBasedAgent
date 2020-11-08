#include "MarsMap.hh"

SamplePercept::SamplePercept(char *line) {
  // ok, this is a bit bad; doesn't do any error checking
  
  myPercept=atoi(line);
}

MarsPlace::MarsPlace(char *line) {
  char *tok=strtok(line,",");
  myName=strdup(tok);
  tok=strtok(nullptr,",");
  myPercept=new SamplePercept(tok);
  tok=strtok(nullptr,",");
  myXcoord=atof(tok);
  tok=strtok(nullptr,",");
  myYcoord=atof(tok);

  myAdjacent=new char *[MAXOUTDEGREE];
  myDistance=new float[MAXOUTDEGREE];
  myNeighborCount=0;
  while((tok=strtok(nullptr,","))) {
    myAdjacent[myNeighborCount]=strdup(tok);
    tok=strtok(nullptr,",");
    myDistance[myNeighborCount++]=atof(tok);
  }
}

MarsPlace::~MarsPlace() {
  int i;
  for (i=0;i<myNeighborCount;i++) 
    free(myAdjacent[i]);
  delete myAdjacent;
  delete myDistance;
  delete myPercept;
  free((void *)myName);
}

float MarsPlace::distanceTo(const char *n) {
  for (int i=0;i<myNeighborCount;i++) {
    if (strcmp(myAdjacent[i],n)==0) {
      return myDistance[i];
    }
  }
  return -1;
}

MarsMap::MarsMap(const char *filename) {
  FILE *myFile;

  myFile=fopen(filename,"r");
  if (myFile==nullptr) {
    fprintf(stderr,"Error opening file %s, exiting\n",filename);
    exit(1);
  }
  initialize(myFile);
  fclose(myFile);
}

void MarsMap::initialize(FILE *fp) {
  char buf[1024];
  
  while((fgets(buf,1023,fp))) {
    auto newPlace = new MarsPlace(buf);
    h[newPlace->name()]=newPlace;
  }
}


MarsMap::~MarsMap() {
  h.clear();
}



MarsPlace *MarsMap::getPlace(const char *place) {
  if (h.find(place)==h.end()) {
    return nullptr;
  } else {
    return h[place];
  }
}

float MarsMap::getDistance(MarsPlace *p1, MarsPlace *p2) {
  if (p1==nullptr || p2==nullptr) {
    return -1;
  } else {
    return p1->distanceTo(p2->name());
  }
}

float MarsMap::getDistance(const char *p1, const char *p2) {
  return this->getDistance(this->getPlace(p1),this->getPlace(p2));
}


