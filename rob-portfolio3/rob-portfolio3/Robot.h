#pragma once
#include "Image.hpp"
#include "graph.h"

using rw::sensor::Image;

#define VERTEX 255
#define EDGE 255
#define PATH 0

class Robot
{
public:
	Robot(Image* map);
    void mapEnviroment();
    void saveInternMaps();
	void colorPath(vector<Vertex*> path); // to color path
	void listNodes(); // list index and x,y position

	vector<Vertex*> findReturnPath(Vertex& start, Vertex& goal, Graph& g);
	void find();
	~Robot();

private:
    int sensorLength = 8; //sensor reaches 8x8 around robot
	int xPos, yPos;
	Image* img;
    Image* graphMap;
    Graph graph;
};

